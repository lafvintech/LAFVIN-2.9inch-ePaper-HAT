# LAFVIN 2.9-inch Four-Color E-Paper for Raspberry Pi

This directory contains the Raspberry Pi C demo for the LAFVIN 2.9-inch
black, white, yellow, and red e-paper module. The panel resolution is
128 x 296 pixels. Each pixel uses 2 bits, so one packed frame occupies
9,472 bytes.

The C implementation uses Raspberry Pi SPI0 and the `lgpio` library. It does
not use or control a `PWR/EN` pin. The module must remain powered from its VCC
and GND connections while the demo runs.

The drawing, BMP, and font modules retain Waveshare's public APIs. The GPIO,
SPI, display-controller, timeout, and error-handling layers are maintained by
LAFVIN.

## Project structure

| Directory | Purpose |
| --- | --- |
| `c/examples/EPD_2in9g_test.c` | Linear beginner demo; start here |
| `c/include/EPD_2in9g.h` | Public display API, compatible with Waveshare naming |
| `c/src` | `lgpio` hardware layer and display-controller driver |
| `c/lib/GUI` | Waveshare Paint and BMP APIs with LAFVIN fixes |
| `c/lib/Fonts` | Waveshare/ST font tables |
| `c/pic` | BMP files used by the demo |
| `c/tests` | Paint and BMP tests that do not access hardware |

`_waveshare_c_reference` contains the untouched upstream project for
comparison and is not compiled into the LAFVIN demo.

## Hardware wiring

Turn off the Raspberry Pi before connecting or disconnecting the module.
GPIO numbers below use BCM numbering.

| E-paper pin | Raspberry Pi signal | Physical pin |
| --- | --- | --- |
| VCC | Supply required by the module | Pin 1 or 17 for 3.3 V |
| GND | GND | Pin 6 |
| DIN / MOSI | GPIO10 / SPI0 MOSI | Pin 19 |
| CLK / SCK | GPIO11 / SPI0 SCLK | Pin 23 |
| CS | GPIO8 / SPI0 CE0 | Pin 24 |
| DC | GPIO25 | Pin 22 |
| RST | GPIO17 | Pin 11 |
| BUSY | GPIO24 | Pin 18 |

> **Warning:** Raspberry Pi GPIO uses 3.3 V logic. Never apply a 5 V signal
> to a GPIO pin. Confirm the required VCC voltage from the module label or
> product specification before powering the display.

## Enable SPI

Run:

```bash
sudo raspi-config
```

Select **Interface Options > SPI > Yes**, then reboot. Verify that SPI0 CE0
is available:

```bash
ls -l /dev/spidev0.0
```

Raspberry Pi also documents SPI enablement and pin functions in its
[official SPI documentation](https://www.raspberrypi.com/documentation/computers/raspberry-pi.html#spi-software).

## Install the C dependencies

```bash
sudo apt update
sudo apt install build-essential liblgpio-dev
```

## Build and test

Run the build commands from the `c` directory:

```bash
cd LAFVIN_EPD/RPI/EPD290_4C/c
make
make test
```

The tests validate 2-bit drawing rotation, ASCII text rendering, and BMP
palette conversion/scaling using a temporary 4 x 2 fixture. They do not
access GPIO or SPI.

## Run the demo

Run from the `c` directory to use the bundled BMP:

```bash
./bin/epd_2in9g_demo
```

The example is intentionally linear. Open `examples/EPD_2in9g_test.c` and
change each `#if 1` to `#if 0` if you want to skip that section:

```c
#if 1   /* Show the first BMP image. */
GUI_ReadBmp_RGB_4Color("./pic/img1.bmp", 0, 0);
#endif
```

Change the path directly in `GUI_ReadBmp_RGB_4Color()` to display another
image. To add another page, copy one complete BMP section and change its path.
Run `make` again after editing the example.

The BMP must be uncompressed 24-bit RGB. The loader scales it to the Paint
canvas, so a landscape image with approximately the display's `296:128`
aspect ratio gives the best result. The Paint layer then rotates it into the
controller's native `128 x 296` memory layout. Use black, white, yellow, and
red artwork for predictable color conversion.

If the current account cannot claim GPIO or SPI, add it to the appropriate
groups and sign in again:

```bash
sudo usermod -aG gpio,spi "$USER"
```

For a one-time hardware test, `sudo ./bin/epd_2in9g_demo` can help distinguish
a permission problem from a wiring problem. Do not run the application as
root permanently when group permissions are available.

The demo performs these operations from top to bottom:

1. Allocates one 9,472-byte image buffer.
2. Opens GPIO/SPI, initializes the panel, and clears it once.
3. Reads and displays `img1.bmp`.
4. Reads and displays `img2.bmp`.
5. Draws a Paint API page containing shapes, text, and a number.
6. Puts the controller into deep sleep while retaining the Paint page.

No C image array or image conversion utility is required at runtime.

## Default configuration

The defaults are defined near the top of `c/lib/Config/DEV_Config.h`:

| Setting | Value |
| --- | --- |
| RST | BCM GPIO17 |
| DC | BCM GPIO25 |
| BUSY | BCM GPIO24 |
| SPI bus/device | SPI0 CE0 |
| SPI mode | Mode 0 |
| SPI speed | 4 MHz |
| BUSY timeout | 120 seconds |

Raspberry Pi 5 normally uses a different GPIO chip from earlier models. The
driver detects Raspberry Pi 5 automatically. If GPIO discovery is incorrect
on a custom image, inspect `ls /dev/gpiochip*` and override the chip number:

```bash
LAFVIN_GPIOCHIP=0 ./bin/epd_2in9g_demo
```

## Paint and BMP APIs

The full Waveshare drawing interface is available through `GUI_Paint.h`,
including points, lines, rectangles, circles, English/Chinese strings,
numbers, decimals, time, rotation, and mirroring. Font tables are in
`c/lib/Fonts`.

Minimal BMP-to-display sequence:

```c
#include "EPD_2in9g.h"
#include "GUI_BMPfile.h"
#include "GUI_Paint.h"

UBYTE frame[EPD_2IN9G_FRAME_BYTES];

DEV_Module_Init();
EPD_2IN9G_Init();

Paint_NewImage(frame, EPD_2IN9G_WIDTH,
               EPD_2IN9G_HEIGHT, ROTATE_90,
               EPD_2IN9G_WHITE);
Paint_SetScale(4);
Paint_Clear(EPD_2IN9G_WHITE);

if (GUI_ReadBmp_RGB_4Color("picture.bmp", 0, 0) == GUI_BMP_SUCCESS) {
    EPD_2IN9G_Init_Fast();
    EPD_2IN9G_Display(frame);
}
EPD_2IN9G_Sleep();
DEV_Module_Exit();
```

Packed frame bytes store four pixels from left to right, most-significant
pixel first. Native color values are `0=black`, `1=white`, `2=yellow`, and
`3=red`.

## Troubleshooting

**The program cannot open SPI0.0**

- **Symptom:** The demo reports that GPIO/SPI cannot be opened.
- **Possible causes:** SPI is disabled, `/dev/spidev0.0` is missing, or the
  current account lacks permission.
- **Solutions:** Enable SPI, reboot, verify `/dev/spidev0.0`, and check the
  `gpio` and `spi` group membership.

**The demo reports a BUSY timeout**

- **Symptom:** Initialization or refresh fails after approximately 120
  seconds.
- **Possible causes:** BUSY or RST is wired incorrectly, the panel has no
  power, SPI wiring is incorrect, or the connected controller is not the
  supported 2.9-inch four-color model.
- **Solutions:** Turn off power and recheck GPIO24 BUSY, GPIO17 RST, GPIO25
  DC, SPI0 wiring, VCC, and GND. BUSY is low while this panel is busy and high
  when it is ready.

**The image has incorrect colors or orientation**

- **Possible causes:** The image uses a different e-paper palette, or the
  connected panel is a different 2.9-inch revision.
- **Solutions:** Confirm the black/white/yellow/red four-color panel model
  and save the file as an uncompressed 24-bit BMP. Use a landscape image with
  approximately a `296:128` aspect ratio and black, white, yellow, and red
  artwork.

**The BMP cannot be opened**

- **Symptom:** The program exits before GPIO/SPI initialization and prints
  `Cannot open BMP file`.
- **Solutions:** Run the default demo from the `c` directory, or pass an
  absolute path to a readable BMP file.
