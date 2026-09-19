.. _about_this_kit:

About This Kit
==============

LAFVIN 2.9inch e-Paper HAT
--------------------------

.. figure:: ./Tutorial/img/main.jpg
   :align: center
   :width: 80%

   *LAFVIN 2.9inch e-Paper HAT*

Introduction
------------

This is 2.9inch E-Ink display HAT with Raspberry Pi 40PIN GPIO extension header,
compatible with Raspberry Pi series boards. It uses a 128 x 296 four-colour
display (black, white, red, and yellow), with embedded controller,
communicating via SPI interface.

No backlight, keeps displaying last content for a long time even when power down.
Ultra low power consumption, basically power is only required for refreshing.

SPI interface, for connecting with controller boards like Arduino/ESP32, etc.
Onboard voltage translator, compatible with 3.3V / 5V MCUs.

Comes with online development resources and manual (driver board circuit
diagram, examples for Raspberry Pi)

Parameters
----------

The display-related values below are updated from the GDEY029F52 Product
Specifications, revision 1.0 (2025-06-12). Module-level board dimensions,
external supply, and standby-current values remain module specifications.

.. list-table::
   :header-rows: 1
   :widths: 40 60
   :class: longtable

   * - Parameter
     - Specification
   * - Model
     - GDEY029F52
   * - Screen size
     - 2.9 inch
   * - Driver board dimensions
     - 69 x 30 mm
   * - Display dimensions
     - 29.056 mm x 66.896 mm
   * - Outline dimensions (panel only)
     - 36.7 mm (H) x 79.0 mm (V) x 1.2 mm (D)
   * - Operating voltage
     - 3.3V/5V (5V is required for power supply and signal)
   * - Communication interface
     - SPI
   * - Dot pitch
     - 0.226 mm (H) x 0.227 mm (V)
   * - Resolution
     - 128 x 296
   * - Display colour
     - Black, white, red, yellow
   * - Refresh time
     - Full: 26 s; fast: 11 s, at 25 deg C
   * - Refresh power
     - 26.4 mW (typ.)
   * - Standby current
     - < 0.01 uA (almost none)
   * - Operating temperature
     - 0 to 40 deg C
   * - Storage temperature
     - -25 to 70 deg C

.. note::

   The resolution, dimensions, pixel pitch, display colours, refresh time, and
   temperature range are panel specifications. The remaining module-level
   values must follow the product hardware documentation.

Module Hardware
---------------

The module schematic (``2.9inch e-paper_V1.4``) confirms that the
GDEY029F52 panel's 24-pin FPC is connected **inside the HAT**. Users do not
wire the FPC directly. The module provides two host-side connections:

- **Raspberry Pi 40-pin header (J1):** plugs directly into the Raspberry Pi.
  The HAT is powered from the Raspberry Pi's 5 V rail and uses the Pi's 3.3 V
  rail as the logic reference.
- **External 8-pin header (H1):** ``VCC_INT``, ``GND``, ``DIN``, ``CLK``,
  ``CS``, ``D/C``, ``RST``, and ``BUSY`` for external hosts such as Arduino
  and ESP32.

The schematic includes a 3.3 V regulator and a TXB0108 level translator
between the host-side signals and the panel controller. This is the basis for
the module's 3.3 V / 5 V MCU compatibility; it does not change the raw panel's
own 3.3 V logic requirement.

Communication Protocol
----------------------

.. figure:: ./Tutorial/img/spi_com.png
   :align: center
   :width: 70%
   
   *SPI Communication Interface*

- **CSB (CS)**: Slave chip select signal, active at low level. When it is at low level, the chip is enabled.
- **SCL (SCK/SCLK)**: Serial clock signal.
- **D/C (DC)**: Data/command control signal, write command (Command) when the level is low; write data (Data/parameter) when the level is high.
- **SDA (DIN)**: Serial data signal.
- **Timing**: CPHL=0, CPOL=0, i.e. SPI mode 0.

Note: For specific information about SPI communication, you can search for information online on your own.

Working Principle
-----------------

The e-paper used in this product uses "microcapsule electrophoresis display" technology for image display. The basic principle is that charged nanoparticles suspended in a liquid migrate under the action of an electric field. The e-paper display screen displays patterns by reflecting ambient light and does not require a backlight. Under ambient light, the e-paper display screen is clearly visible, with a viewing angle of almost 180 degrees. Therefore, e-paper displays are ideal for reading.

Program Principle
-----------------

GDEY029F52 is a four-colour panel. Its image data and update waveform are
panel-specific, so the former monochrome rule of one bit per pixel (black or
white) must not be used. Generate image data with the GDEY029F52-compatible
driver library, using its documented colour constants and buffer format for
black, white, red, and yellow. Set the image canvas to **128 x 296 pixels**
(or the rotated orientation explicitly supported by that library).

Typical update flow is: reset the panel, wait until ``BUSY_N`` is high, send
the panel-specific initialisation and image data, power on, trigger display
refresh, wait until ``BUSY_N`` is high again, then power off and enter deep
sleep. The specification identifies ``0xA5`` as the deep-sleep command.

Precautions
-----------

.. role:: red
   :class: red

1. :red:`Partial Refresh Limitation: For e-Paper displays that support partial refresh, please note that you cannot refresh them with the partial refresh mode all the time. After refreshing partially several times, you need to fully refresh EPD once. Otherwise, the display effect will be abnormal, which cannot be repaired!`

2. :red:`Power Management: Note that the screen cannot be powered on for a long time. When the screen is not refreshed, please set the screen to sleep mode or power off it. Otherwise, the screen will remain in a high voltage state for a long time, which will damage the e-Paper and cannot be repaired!`

3. :red:`Refresh Intervals: When using the e-Paper display, it is recommended that the refresh interval is at least 180s, and refresh at least once every 24 hours. If the e-Paper is not used for a long time, you should use the program to clear the screen before storing it. (Refer to the datasheet for specific storage environment requirements.)`

4. **Sleep Mode**: After the screen enters sleep mode, the sent image data will be ignored, and it can be refreshed normally only after initializing again.
      
5. **Border Color Adjustment**: Control the 0x3C or 0x50 (refer to the datasheet for details) register to adjust the border color. In the demo, you can adjust the Border Waveform Control register or VCOM AND DATA INTERVAL SETTING to set the border.
      
6. **Image Display Issues**: If you find that the created image data is displayed incorrectly on the screen, it is recommended to check whether the image size setting is correct, change the width and height settings of the image and try again.
      
7. **Voltage Compatibility**: The working voltage of the e-Paper display is 3.3V. If you buy the raw panel, you need to add a level convert circuit for compatibility with 5V voltage. The new version of the driver board (V2.1 and subsequent versions) has been added a level processing circuit, which can support both 3.3V and 5V. The old version only supports a 3.3V working environment. You can confirm the version before using it. (The one with the 20-pin chip on the PCB is generally the new version. And the version number is under the board name.)
      
8. :red:`Cable Handling: The FPC cable of the screen is fragile, Please note: Do not bend the cable along the vertical direction of the screen to avoid tearing the cable; Do not repeatedly excessive bending line to avoid line fracture; Do not bend the cable toward the front of the screen to prevent the cable from being disconnected from the panel. It is recommended to use after fixing the cable during debugging and development.`

9. :red:`Screen Fragility: The screen of e-Paper is relatively fragile, please try to avoid dropping, bumping and pressing hard.`

10. **Testing Recommendation**: We recommend that customers use the sample program provided by us to test with the corresponding development board.

.. raw:: html

   <style>
   .red {
     color: red;
     font-weight: bold;
     display: block;
     margin-bottom: 8px;
     line-height: 1.6;
   }
   .longtable td {
     padding: 8px;
   }
   ol li {
     margin-bottom: 15px;
     line-height: 1.5;
   }
   p {
     line-height: 1.5;
     margin-bottom: 10px;
   }
   </style>
