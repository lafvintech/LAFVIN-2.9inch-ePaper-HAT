 #include "EPD_Panel.h"
#include "GUI_Paint.h"
#include "fonts.h"
#include "ImageData.h"

void setup()
{
    if(DEV_Module_Init()!=0){
        return;
    }

    Debug("EPD_test Demo\r\n");
    Debug("e-Paper Init and Clear...\r\n");
    EPD_Init();
    EPD_Clear(EPD_WHITE);
    DEV_Delay_ms(2000);

    //Create a new image cache
    UBYTE *BlackImage;
    UDOUBLE Imagesize = ((EPD_WIDTH % 4 == 0)? (EPD_WIDTH / 4 ): (EPD_WIDTH / 4 + 1)) * EPD_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        Debug("Failed to apply for black memory...\r\n");
    }

    // 绘制图片
#if 1
    Debug("Image4color.\r\n");
    EPD_Init_Fast(); // 快速刷新初始化
    //EPD_Init(); // 全部刷新初始化
    EPD_Display(gImage_img1); // 显示4色图片
    DEV_Delay_ms(5000);
    Paint_Clear(EPD_WHITE);
    EPD_Display(gImage_img2); // 显示4色图片
    EPD_Sleep();
    DEV_Delay_ms(5000);
#endif

#if 1   // Drawing on the image
    //1.Select Image
    //EPD_Init(); // 全部刷新初始化
    EPD_Init_Fast(); // 快速刷新初始化
    Debug("SelectImage:BlackImage\r\n");
    Paint_NewImage(BlackImage, EPD_WIDTH, EPD_HEIGHT, 90, EPD_WHITE);
    Paint_SetScale(4);
    Paint_SelectImage(BlackImage);
    Paint_Clear(EPD_WHITE);

    // 2. 绘制4色块和文字标签
    Debug("Drawing:BlackImage\r\n");
    Paint_DrawRectangle(10, 30, 50, 70, EPD_RED, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawString_EN(25, 45, "R", &Font16, EPD_RED, EPD_YELLOW);
    Paint_DrawRectangle(50, 30, 90, 70, EPD_YELLOW, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawString_EN(65, 45, "Y", &Font16, EPD_YELLOW, EPD_RED);
    Paint_DrawRectangle(10, 70, 50, 110, EPD_BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawString_EN(25, 85, "B", &Font16, EPD_BLACK, EPD_WHITE);
    Paint_DrawRectangle(50, 70, 90, 110, EPD_WHITE, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawString_EN(65, 85, "W", &Font16, EPD_WHITE, EPD_BLACK);
    // 绘制矩形
    Paint_DrawRectangle(10, 110, 90, 127, EPD_BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);

    // 绘制点 线 圆的示例
    Paint_DrawPoint(15, 120, EPD_BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(45, 120, EPD_BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(75, 120, EPD_BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawCircle(120, 100, 20, EPD_BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawLine(120, 84, 106, 108, EPD_BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(106, 108, 134, 108, EPD_BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(134, 108, 120, 84, EPD_BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

    // 绘制文字和数字
    Paint_DrawString_EN(95, 0, "Four color e-Paper", &Font16, EPD_RED, EPD_YELLOW);
    Paint_DrawString_EN(180, 20, "2.9 Inch e-Paper", &Font12, EPD_YELLOW, EPD_BLACK);
    Paint_DrawString_CN(170, 40, "你好世界!", &Font24CN, EPD_RED, EPD_WHITE);
    Paint_DrawString_EN(210, 80, "Hello World!", &Font12, EPD_YELLOW, EPD_BLACK);
    Paint_DrawNum(250, 35, 123456, &Font12, EPD_RED, EPD_WHITE);

    Debug("EPD_Display\r\n");
    EPD_Display(BlackImage);
    EPD_Sleep();
    DEV_Delay_ms(5000);
#endif

    Debug("Clear...\r\n");
    EPD_Init();
    EPD_Clear(EPD_WHITE);

    Debug("Goto Sleep...\r\n");
    EPD_Sleep();
    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    Debug("Module entered deep sleep.\r\n");

}
void loop() {


}
