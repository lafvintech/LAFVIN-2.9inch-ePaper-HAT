.. _arduino_setup:

Arduino IDE Board Setup
=========================

Install Arduino IDE
---------------------

Install Arduino IDE 2 using :ref:`install_arduino_ide`, then complete the
ESP32 board package setup below before opening the e-paper example.

.. _install_esp32_board_package:

Install the ESP32 Board Package
---------------------------------

The **esp32** package by **Espressif Systems** provides the compiler,
upload tools, and board definitions for both ESP32 and ESP32-S3.
Install it once for either board.

Add the Package Index
^^^^^^^^^^^^^^^^^^^^^^^

1. Open Arduino IDE. On Windows or Linux, select **File > Preferences**;
   on macOS, select **Arduino IDE > Settings**.
2. Find **Additional boards manager URLs** and open its URL editor.
3. Add the official stable package index below on a new line. Keep any
   existing URLs.

   .. code-block:: text

      https://espressif.github.io/arduino-esp32/package_esp32_index.json

4. Click **OK** to close the URL editor, then **OK** again to save the settings.

Install with Boards Manager
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

1. Open **Tools > Board > Boards Manager**, or use the Boards Manager icon
   in the left sidebar.
2. Search for ``esp32``.
3. Find **esp32 by Espressif Systems**, select a stable release, and click
   **Install**. If it is already installed, continue to board selection.
4. Wait for installation to finish, then restart Arduino IDE.

.. raw:: html

   <details class="optional-library">
   <summary>Download Fails: Use the Official China Mirror</summary>

If the official index or package downloads fail, replace the ESP32 index
URL with Espressif's China mirror, save the settings, and reopen Boards Manager:

.. code-block:: text

   https://jihulab.com/esp-mirror/espressif/arduino-esp32/-/raw/gh-pages/package_esp32_index_cn.json

Choose the mirrored version with a ``-cn`` suffix. Espressif notes that
mirror updates must be selected manually rather than through automatic updates.

.. raw:: html

   </details>

Select the Board and Port
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Connect the board with a USB data cable. In **Tools > Board**, choose the
entry matching your board. The e-paper tutorial uses these selections:

.. list-table:: Board Selection
   :header-rows: 1
   :widths: 40 60

   * - Development board
     - Arduino IDE selection
   * - ESP32-S3
     - ESP32S3 Dev Module
   * - Classic ESP32 (DOIT DevKit V1)
     - DOIT ESP32 DEVKIT V1

Select the connected device under **Tools > Port**. Port names vary by
computer; use the port that appears when you connect the board.

.. note::
   If the ESP32 boards are missing from the menu, confirm that the
   Espressif package finished installing and restart the IDE.
   If no port appears, check the USB data cable and the board's USB driver.

Return to :doc:`../Tutorial/3.esp32` to select the matching wiring profile
and upload the e-paper example.

Official References
---------------------

- `Espressif: Install Arduino-ESP32 <https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html>`_
- `Arduino: Add Boards to Arduino IDE <https://support.arduino.cc/hc/en-us/articles/360016119519-Add-boards-to-Arduino-IDE>`_
- `Arduino: Add Third-Party Board Package URLs <https://support.arduino.cc/hc/en-us/articles/360016466340-Add-third-party-platforms-to-the-Boards-Manager-in-Arduino-IDE>`_
