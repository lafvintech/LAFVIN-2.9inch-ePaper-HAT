Flash Raspberry Pi OS
=====================

This guide uses a Raspberry Pi 5 and the official Raspberry Pi imaging workflow.
Complete the operating system setup before connecting or configuring the e-Paper HAT.

What You Need
-------------

- Raspberry Pi 5 and a suitable USB-C power supply.
- A reliable microSD card (32 GB or larger is recommended) and a card reader.
- A Windows, macOS, or Linux computer connected to the internet.
- The official `Raspberry Pi Imager <https://www.raspberrypi.com/software/>`_.

Recommended Software
--------------------

The versions below were checked on 10 August 2026:

- **Raspberry Pi Imager 2.0.8** — the latest stable release. Do not use the 2.0.9 pre-release for a new setup.
- **Raspberry Pi OS (64-bit)** — the recommended current release for Raspberry Pi 5: Debian 13 (Trixie) with Linux kernel 6.18.

Raspberry Pi Imager automatically offers the recommended operating system for the selected device, so downloading an ``.img`` file manually is not necessary.

Install Raspberry Pi Imager
---------------------------

1. Open the official `Raspberry Pi software page <https://www.raspberrypi.com/software/>`_ and download Raspberry Pi Imager for your computer.
2. Run the installer, then start **Raspberry Pi Imager**.
3. Insert the microSD card into the computer's card reader.

Choose the Device, OS, and Storage
----------------------------------

1. Select **Choose Device**, then select **Raspberry Pi 5**.

   .. figure:: img/imager/device-tab.png
      :align: center
      :width: 85%
      :alt: Raspberry Pi Imager Device tab.

      In the Device tab, select **Raspberry Pi 5**, then click **Next**.

2. Select **Choose OS** and choose **Raspberry Pi OS (64-bit)**. This is the recommended desktop image for Raspberry Pi 5.

   .. figure:: img/imager/os-tab.png
      :align: center
      :width: 85%
      :alt: Raspberry Pi Imager OS tab.

      The recommended Raspberry Pi OS for the selected device appears at the top of the list.

3. Insert the microSD card into the reader. Select **Choose Storage**, then select the target microSD card by its capacity and name.

   .. figure:: img/imager/storage-tab.png
      :align: center
      :width: 85%
      :alt: Raspberry Pi Imager Storage tab.

      Keep **Exclude system drives** enabled and choose only the microSD card to be erased.

.. warning::

   Writing an image erases the selected storage device. Check its capacity and name carefully before continuing. Disconnect other removable drives if you are unsure which one is the microSD card.

Configure OS Customisation
--------------------------

Click **Next**, then choose **Edit Settings** when Imager offers OS customisation. Configure these settings before writing the card.

1. In **Hostname**, enter a short device name, such as ``lafvin-pi5``. Use only letters, numbers, and hyphens. The device will later be available at ``lafvin-pi5.local`` on most local networks.

   .. figure:: img/imager/hostname-subtab.png
      :align: center
      :width: 85%
      :alt: Raspberry Pi Imager hostname customisation tab.

2. In **Localisation**, select your city or set the time zone, keyboard layout, and wireless LAN country.

3. In **User**, set a lowercase username and a strong password. These are required for SSH and ``sudo`` commands.

   .. figure:: img/imager/user-subtab.png
      :align: center
      :width: 85%
      :alt: Raspberry Pi Imager user customisation tab.

4. In **Wi-Fi**, select the correct network, enter its password, and enable **Hidden SSID** only when the network name is not broadcast.

   .. figure:: img/imager/wifi-subtab.png
      :align: center
      :width: 85%
      :alt: Raspberry Pi Imager Wi-Fi customisation tab.

5. In **Remote Access**, enable **SSH** if you will use the Pi without a monitor. Choose password authentication for a simple local setup, or public-key authentication for stronger security.

   .. figure:: img/imager/ssh-subtab.png
      :align: center
      :width: 85%
      :alt: Raspberry Pi Imager remote access and SSH customisation tab.

Click **Save**, then choose **Yes** to apply the settings to the image.

Write and Verify the Image
--------------------------

1. Review the selected device, operating system, storage device, and customisation settings. Click **Write** only when every item is correct.

   .. figure:: img/imager/write.png
      :align: center
      :width: 85%
      :alt: Raspberry Pi Imager writing summary.

      The summary is the last opportunity to check the target storage device.

2. In the erase confirmation dialog, select **I understand, erase and write**.

   .. figure:: img/imager/are-you-sure.png
      :align: center
      :width: 85%
      :alt: Raspberry Pi Imager erase confirmation dialog.

3. Wait for Raspberry Pi Imager to finish both writing and verification. Do not remove the card during this process.
4. When Imager reports **Write Successful**, safely eject the microSD card from the computer.

First Boot on Raspberry Pi 5
----------------------------

1. Ensure the Raspberry Pi 5 is powered off.
2. Insert the prepared microSD card, then connect a display, keyboard, and network cable if required.
3. Connect power to boot the Pi. If OS customisation was used, the configured account and network settings are applied on first boot.
4. After you reach the desktop or connect through SSH, update the system:

   .. code-block:: bash

      sudo apt update
      sudo apt full-upgrade -y
      sudo reboot

5. Power off the Pi before connecting the e-Paper HAT, then continue with :doc:`../Tutorial/1.raspberrypi`.

Troubleshooting
---------------

- **The Pi does not boot**: confirm the card write completed verification, use a suitable power supply, and try writing the image again.
- **Wi-Fi is unavailable**: recheck the SSID, password, wireless country, and whether the network is hidden.
- **SSH connection fails**: confirm SSH was enabled during OS customisation and use the hostname or IP address shown by your router.

Official References
-------------------

- `Install an operating system with Raspberry Pi Imager <https://www.raspberrypi.com/documentation/computers/getting-started.html>`_
- `Raspberry Pi OS downloads and release information <https://www.raspberrypi.com/software/operating-systems/>`_
- `Raspberry Pi Imager releases <https://github.com/raspberrypi/rpi-imager/releases>`_
- `Official Raspberry Pi Imager interface screenshots <https://github.com/raspberrypi/documentation/blob/master/documentation/asciidoc/computers/getting-started/install.adoc>`_
