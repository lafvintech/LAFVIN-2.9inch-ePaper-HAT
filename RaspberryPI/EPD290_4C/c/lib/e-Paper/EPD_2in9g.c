#include "EPD_2in9g.h"

#include <stddef.h>
#include <string.h>

#include "Debug.h"

#define CMD_PANEL_SETTING 0x00U
#define CMD_POWER_ON 0x04U
#define CMD_DATA_START 0x10U
#define CMD_DISPLAY_REFRESH 0x12U
#define CMD_POWER_OFF 0x02U
#define CMD_DEEP_SLEEP 0x07U

#define BUSY_TIMEOUT_MS 120000U
#define BUSY_POLL_MS 5U

/*
 * 墨水屏的基本驱动流程：
 * 1. 通过 RST 引脚复位屏幕控制器；
 * 2. 等待 BUSY 引脚进入空闲状态；
 * 3. 发送初始化寄存器，设置面板、分辨率和驱动参数；
 * 4. 发送一帧图像数据，再执行 DISPLAY_REFRESH 刷新屏幕；
 * 5. 不再使用屏幕时发送休眠命令，以降低功耗。
 *
 * SPI 总线同时传输“命令”和“数据”，DC 引脚用来区分两者：
 * DC = 0 表示当前字节是命令，DC = 1 表示当前字节是数据。
 */
typedef struct {
    UBYTE command; /* 寄存器命令 */
    UBYTE length;  /* 该命令后面需要发送的数据个数 */
    UBYTE data[7]; /* 寄存器数据，本屏初始化命令最多使用 7 字节 */
} RegisterWrite;

/*
 * 公共初始化表，普通刷新和快速刷新都会使用。
 *
 * 例如 {0x4D, 1, {0x78}} 表示：
 *   先发送命令 0x4D，再发送 1 个数据 0x78。
 *
 * {0x06, 7, {...}} 表示：
 *   先发送命令 0x06，再连续发送后面的 7 个数据。
 *
 * 这些数值来自屏幕控制器的初始化时序。更换其他屏幕型号时，
 * 不能直接照搬，需要以对应屏幕的数据手册或初始化代码为准。
 */
static const RegisterWrite common_init[] = {
    {0x4D, 1, {0x78}},                                  /* 芯片内部参数 */
    {CMD_PANEL_SETTING, 2, {0x0F, 0x29}},               /* 面板工作方式 */
    {0x01, 2, {0x07, 0x00}},                            /* 电源参数 */
    {0x03, 3, {0x10, 0x54, 0x44}},                      /* 电源关闭时序 */
    {0x06, 7, {0x0F, 0x0A, 0x2F, 0x25, 0x22, 0x2E, 0x21}}, /* 升压软启动 */
    {0x41, 1, {0x00}},                                  /* 温度传感器设置 */
    {0x50, 1, {0x37}},                                  /* VCOM 和数据间隔 */
    {0x60, 2, {0x02, 0x02}},                            /* TCON 设置 */
    {0x61, 4, {                                          /* 屏幕分辨率：128 × 296 */
        (UBYTE)(EPD_2IN9G_WIDTH >> 8),
        (UBYTE)(EPD_2IN9G_WIDTH & 0xFF),
        (UBYTE)(EPD_2IN9G_HEIGHT >> 8),
        (UBYTE)(EPD_2IN9G_HEIGHT & 0xFF)
    }},
    {0x65, 4, {0x00, 0x00, 0x00, 0x00}},                /* 栅极/源极起始位置 */
    /* 以下是控制器的内部驱动参数，通常不建议由用户修改。 */
    {0xE7, 1, {0x1C}},
    {0xE3, 1, {0x22}},
    {0xB4, 1, {0xD0}},
    {0xB5, 1, {0x03}},
    {0xE9, 1, {0x01}},
    {0x30, 1, {0x08}},
};

static void reset_panel(void)
{
    /* 按控制器要求产生一次高—低—高的硬件复位脉冲。 */
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(2);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
}

static int write_bytes(UBYTE data_mode, const UBYTE *data, size_t length)
{
    /* data_mode=0 发送命令，data_mode=1 发送数据；CS 由硬件 SPI 自动控制。 */
    DEV_Digital_Write(EPD_DC_PIN, data_mode);
    return DEV_SPI_Write(data, length);
}

static int write_command(UBYTE command)
{
    return write_bytes(0, &command, 1);
}

static int write_data(const UBYTE *data, size_t length)
{
    return write_bytes(1, data, length);
}

static int write_register(const RegisterWrite *entry)
{
    if (write_command(entry->command) < 0) {
        return -1;
    }
    return entry->length == 0 ? 0 : write_data(entry->data, entry->length);
}

static int wait_busy_level(UBYTE ready_level, const char *label)
{
    /*
     * BUSY 用于表示屏幕控制器是否完成内部处理。
     * 本型号在等待的位置以高电平表示可以继续执行。
     * 超时保护可以避免接线错误或屏幕异常时程序永久卡死。
     */
    Debug("e-Paper busy %s\n", label);
    DEV_Delay_ms(100);

    UDOUBLE elapsed = 100;
    while (DEV_Digital_Read(EPD_BUSY_PIN) != ready_level) {
        if (elapsed >= BUSY_TIMEOUT_MS) {
            Debug("e-Paper busy timeout (%s, %u ms)\n", label, BUSY_TIMEOUT_MS);
            return -1;
        }
        DEV_Delay_ms(BUSY_POLL_MS);
        elapsed += BUSY_POLL_MS;
    }

    Debug("e-Paper busy %s release\n", label);
    return 0;
}

static int write_common_init(void)
{
    /* 按表格顺序逐项发送，寄存器的先后顺序不能随意调整。 */
    for (size_t i = 0; i < sizeof(common_init) / sizeof(common_init[0]); ++i) {
        if (write_register(&common_init[i]) < 0) {
            Debug("EPD register write failed: 0x%02X\n", common_init[i].command);
            return -1;
        }
    }
    return 0;
}

static int initialize_panel(int fast)
{
    /* 普通初始化与快速初始化共用前面的面板配置。 */
    reset_panel();
    if (wait_busy_level(1, "H") < 0 || write_common_init() < 0) {
        return -1;
    }

    if (fast) {
        /* 快速刷新需要额外设置刷新模式和刷新频率。 */
        const RegisterWrite fast_init[] = {
            {0xE0, 1, {0x02}},
            {0xE6, 1, {90}},
            {0xA5, 0, {0}},
        };
        for (size_t i = 0; i < sizeof(fast_init) / sizeof(fast_init[0]); ++i) {
            if (write_register(&fast_init[i]) < 0) {
                return -1;
            }
        }
        if (wait_busy_level(1, "H") < 0) {
            return -1;
        }
    }

    if (write_command(CMD_POWER_ON) < 0) {
        return -1;
    }
    if (!fast) {
        DEV_Delay_ms(500);
    }
    return wait_busy_level(1, "H");
}

static int refresh_panel(void)
{
    /* 图像写入显存后，发送刷新命令才会真正更新到墨水屏。 */
    const UBYTE value = 0x00;
    if (write_command(CMD_DISPLAY_REFRESH) < 0 || write_data(&value, 1) < 0) {
        return -1;
    }
    return wait_busy_level(1, "H");
}

void EPD_2IN9G_Init(void)
{
    if (initialize_panel(0) < 0) {
        Debug("EPD full initialization failed\n");
    }
}

void EPD_2IN9G_Init_Fast(void)
{
    if (initialize_panel(1) < 0) {
        Debug("EPD fast initialization failed\n");
    }
}

void EPD_2IN9G_Clear(UBYTE color)
{
    /*
     * 四色屏每个像素占 2 bit，因此 1 字节可以保存 4 个像素：
     * [像素0][像素1][像素2][像素3]，每组各占 2 bit。
     * 例如白色编号为 1，打包后得到 01 01 01 01，即 0x55。
     */
    UBYTE packed = (UBYTE)((color & 0x03U) * 0x55U);
    UBYTE chunk[256];
    memset(chunk, packed, sizeof(chunk));

    if (write_command(CMD_DATA_START) < 0) {
        return;
    }

    size_t remaining = EPD_2IN9G_FRAME_BYTES;
    while (remaining != 0) {
        size_t length = remaining < sizeof(chunk) ? remaining : sizeof(chunk);
        if (write_data(chunk, length) < 0) {
            return;
        }
        remaining -= length;
    }
    (void)refresh_panel();
}

void EPD_2IN9G_Display(const UBYTE *image)
{
    if (image == NULL) {
        Debug("EPD display skipped: image is NULL\n");
        return;
    }
    /*
     * 一帧图像为 128 × 296 × 2 bit = 9472 字节。
     * 先将整帧写入屏幕显存，再调用 refresh_panel() 更新画面。
     */
    if (write_command(CMD_DATA_START) < 0 ||
        write_data(image, EPD_2IN9G_FRAME_BYTES) < 0) {
        return;
    }
    (void)refresh_panel();
}

void EPD_2IN9G_Sleep(void)
{
    /* 先关闭内部电源，等待操作完成，再进入深度休眠。 */
    const UBYTE zero = 0x00;
    const UBYTE sleep_key = 0xA5;

    if (write_command(CMD_POWER_OFF) < 0 || write_data(&zero, 1) < 0) {
        return;
    }
    if (wait_busy_level(1, "H") < 0) {
        return;
    }
    if (write_command(CMD_DEEP_SLEEP) < 0 || write_data(&sleep_key, 1) < 0) {
        Debug("EPD deep sleep command failed\n");
    }
}
