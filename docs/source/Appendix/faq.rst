FAQ
===

Hardware and Connections
------------------------

Can I use the Raspberry Pi header and the 8-pin interface at the same time?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

No. Use only one module interface at a time. Do not connect the 40-pin
Raspberry Pi header and the external 8-pin interface simultaneously.

Do the mounting holes align with Raspberry Pi mounting holes?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

No. The screw size is the same, but the mounting-hole positions are not
designed for Raspberry Pi. They are intended for securing the module on a
desktop when using cable connections or designing a custom enclosure.

How do I use the reserved 8-pin header?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To connect the module to another controller with male header pins, solder a
2.54 mm 8-pin male or female header to the reserved interface and connect it
with Dupont wires. Wire length and quality can affect display stability. Use
short, good-quality Dupont wires where possible to avoid display failures.

Can an Arduino 5 V output drive the e-paper module?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Yes. The module includes a voltage-level translation circuit and supports 5 V
Arduino signals. Follow the Arduino wiring table when connecting the module.

Display and Refresh
-------------------

Why does a refresh take a long time or cause flashing?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

E-paper updates are slower than LCD updates and may visibly flash while the
panel changes state. At 25 deg C, the module documentation lists about 26 s
for a full refresh and about 11 s for a fast refresh. Wait for the update to
finish before disconnecting power.

Why does the display ignore data after sleep?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

After deep sleep, the display must be initialized again before a new image can
be sent. Run the driver initialization sequence before each update after sleep.

How often should I perform a full refresh when using partial refresh?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Reduce consecutive partial refreshes. After every five partial refreshes, run
a full clear refresh to maintain display quality.

Why is the first image after deep sleep not completely clean?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Waking from deep sleep powers the panel up again. Before refreshing the first
new image, run a clear-screen operation to reduce residual images as much as
possible.

Why can I not display an image after switching from fast refresh to full refresh?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When switching from fast refresh to full refresh, run the full-refresh
initialization routine before sending the image data.

Why does the display remain blank?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Check the VCC and GND connections first, then confirm the DIN, CLK, CS, DC,
RST, and BUSY connections match the wiring table. Make sure SPI is enabled on
Raspberry Pi and that the example is configured for the 2.9-inch four-colour
display.

Why is the program stuck at "e-Paper busy"?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This can be caused by an SPI driver or configuration problem. Check the wiring
first, then verify that SPI is enabled and that the SPI speed and mode are
configured correctly.

Can I use software SPI?
^^^^^^^^^^^^^^^^^^^^^^^

Yes. Ensure that the SPI timing and mode are configured correctly.

Why is the displayed image shifted?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The SPI clock may be too fast, the power supply may be unstable, or data may
be lost through poor or long wires. Try a lower SPI clock rate, use a stable
power supply, and keep the connection wires as short as possible, preferably
no longer than 20 cm.
