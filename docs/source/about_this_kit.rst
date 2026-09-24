.. _about_this_kit:

About This Kit
==============

LAFVIN 2.9 inch E-paper Module
------------------------------

.. figure:: ./Tutorial/img/main.jpg
   :align: center
   :width: 80%

   *LAFVIN 2.9 inch E-paper Module*

Introduction
------------

The LAFVIN 2.9 inch E-paper Module is a 2.9-inch E-Ink display module with a
Raspberry Pi 40-pin GPIO extension header, compatible with Raspberry Pi series
boards. It uses a 128 x 296 four-colour display (black, white, red, and
yellow) and uses SPI communication.

No backlight, keeps displaying last content for a long time even when power down.
Ultra low power consumption, basically power is only required for refreshing.

SPI interface, for connecting with controller boards like Arduino/ESP32, etc.
Onboard voltage translator, compatible with 3.3V / 5V MCUs.

Comes with online development resources and documentation, including pin
definitions and Raspberry Pi examples.

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
   * - Display dimensions
     - 29.056 mm x 66.896 mm
   * - Outline dimensions (panel only)
     - 36.7 mm (H) x 79.0 mm (V) x 1.2 mm (D)
   * - Communication interface
     - SPI
   * - Dot pitch
     - 0.226 mm (H) x 0.227 mm (V)
   * - Resolution
     - 128 x 296
   * - Display colour
     - Black, white, red, yellow
   * - Refresh time
     - Full: 26 s; fast: 11 s, at 25 °C
   * - Operating temperature
     - 0 to 40 °C
   * - Storage temperature
     - -25 to 70 °C

.. note::

   The resolution, dimensions, pixel pitch, display colours, refresh time, and
   temperature range are panel specifications. The remaining module-level
   values must follow the product hardware documentation.

Module Hardware
---------------

The GDEY029F52 panel's 24-pin FPC is connected **inside the module**. Users do
not wire the FPC directly. The module provides two host-side connections:

- **Raspberry Pi 40-pin header (J1):** plugs directly into the Raspberry Pi.
  The module is powered from the Raspberry Pi's 5 V rail and uses the Pi's 3.3 V
  rail as the logic reference.
- **External 8-pin header (H1):** ``VCC_INT``, ``GND``, ``DIN``, ``CLK``,
  ``CS``, ``D/C``, ``RST``, and ``BUSY`` for external hosts such as Arduino
  and ESP32.

The module includes a 3.3 V regulator and a TXB0108 level translator
between the host-side signals and the panel controller. This is the basis for
the module's 3.3 V / 5 V MCU compatibility; it does not change the raw panel's
own 3.3 V logic requirement.

Communication Protocol
----------------------

The GDEY029F52 supports both 3-wire and 4-wire serial interfaces. Its serial
engine operates as a slave; the external MCU supplies the clock and initiates
each transaction.

For the 4-wire interface:

- **CSB** selects the panel. It is active low; signals on SCL, D/C, and SDA are
  ignored while CSB is high.
- **D/C** selects the transfer type: low for a command and high for the command
  parameters or image data that follow.
- **SCL** is the serial clock from the external MCU.
- **SDA (DIN)** carries data from the MCU to the panel during write operations.
  For a read operation on the shared serial line, the MCU must release SDA
  during the specified high-impedance and data phases.
- **RST_N** is an active-low reset input and **BUSY_N** is the panel status
  output.

The 4-wire timing diagram in the data sheet shows SCL idle low. It does not
explicitly assign a CPHA value or an SPI mode number, so the host configuration
must follow the data-sheet timing diagram and the compatible driver.

Working Principle
-----------------

GDEY029F52 is a reflective electrophoretic active-matrix display. Its TFT
matrix and integrated driver circuitry update the black, white, red, and yellow
image using the panel's waveform data. As a reflective, bi-stable display, it
uses ambient light rather than a backlight and retains its displayed state when
the panel is not being refreshed.

Program Principle
-----------------

GDEY029F52 is a four-colour panel with a resolution of **128 x 296 px**.

This tutorial does not prescribe image-data packing, an initialisation command
sequence, or power-management commands. Those details can differ when a
different driver board or host controller is used, so use the documentation and
software that match the hardware being connected.

Precautions
-----------

1. Do not apply pressure to the panel. The display glass can break if it is
   dropped or bumped against a hard surface.

2. Do not connect or disconnect the interface while the panel is operating, and
   do not touch the IC bonding area; doing so can scratch the TFT leads or
   damage the IC.

3. Keep moisture from entering the panel. Avoid acid or alkali gases, do not
   disassemble the module, and observe normal ESD precautions for delicate
   electronic components.

4. In use, refresh the panel every 24 hours to reduce the risk of ghosting or
   image sticking. Before shipment or storage, display a completely white image.

5. Do not leave the unprotected panel exposed to high temperature, high
   humidity, sunlight, or fluorescent light for long periods; these conditions
   can degrade display performance.

6. If the display glass breaks, do not touch the electrophoretic material. If
   contact occurs, wash the affected area with soap and water.

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
