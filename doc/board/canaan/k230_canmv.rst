.. SPDX-License-Identifier: GPL-2.0+

K230 CanMV
==========

K230 RISC-V SoC
------------------
The K230 chip is the latest generation SoC product in Canaan Technology's
Kendryte series of AIOT chips.

Mainline support
----------------

The support for following drivers are already enabled:

1. ns16550 UART
2. DWC2 OTG USB controller

Building
~~~~~~~~

1. Get the RISC-V toolchain.
2. Setup cross compilation environment variable:
3. Get the `firmware_gen.py`_ from vendor.

.. code-block:: console

   export CROSS_COMPILE=riscv64-linux-gnu-
   cd <U-Boot-dir>
   make k230_canmv_defconfig
   make
   cp u-boot-gz.img u-boot-gz.img.tmp
   pip install gmssl pycryptodome
   python3 firmware_gen.py -i u-boot-gz.img.tmp -o u-boot-head.img -n

This will generate u-boot-head.img.

.. _firmware_gen.py: https://raw.githubusercontent.com/kendryte/k230_sdk/refs/tags/v1.8/src/little/uboot/tools/firmware_gen.py

Booting
~~~~~~~

Currently, we rely on vendor u-boot-spl to initialize the
ddr and load the u-boot image, then bootup from it.

1. Prepare a SD card with the vendor image burned on it.

2. Write the U-Boot image to the SD card at offset 2MB.

.. code-block:: console

   dd if=u-boot-head.img of=/path/to/your/sdcard bs=1M seek=2

3. Insert the SD card into the board and power it on.

Sample boot log from K230 CanMV board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: none

   U-Boot 2025.01-rc3-00006-g8c58d0cba5cc (Nov 27 2024 - 00:16:34 +0800)

   DRAM:  512 MiB
   Core:  15 devices, 11 uclasses, devicetree: separate
   Loading Environment from nowhere... OK
   In:    serial@91400000
   Out:   serial@91400000
   Err:   serial@91400000
   Net:   No ethernet found.
   K230# cpu detail
     0: cpu@0      thead,c908
           ID = 0, freq = 0 Hz: MMU
   K230# usb start
   starting USB...
   Bus usb@91540000: USB DWC2
   scanning bus usb@91540000 for devices...
   Warning: r8152_eth (eth0) using random MAC address - 5a:de:43:5a:ae:31
   2 USB Device(s) found
   K230#
