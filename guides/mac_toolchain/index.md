---
layout: page
title: Mac Installation Guide
---

For this class, you will need to install the following software:

1.  The `arm-none-eabi` toolchain, which includes a C x86 to ARM cross-compiler
    which compiles C code on your x86 machine to ARM machine code which can run
    on the Raspberry Pi.
2.  The driver and script for the CP2102 console cable which connects your
    computer and Pi, so you can load programs onto the Pi.

If you are on a Mac, follow all the instructions on this page.

### `arm-none-eabi` installation

To ease the installation process for the `arm-none-eabi` tools on OS X, we have
built and packaged them into a Homebrew formula for you.
[Homebrew](http://brew.sh/) is a popular [package
manager](https://en.wikipedia.org/wiki/Package_manager) for OS X. It is good
practice to install software using a package manager.

#### Install Homebrew

If you aren't yet using Homebrew, use these instruction to get Homebrew up and
running. If you are already using Homebrew, skip to the next section.

1.  Install the Xcode command line tools by running the command below. A dialog
    should pop up on your screen. Click "Install" when it appears.

    ```
$ xcode-select --install
    ```

2.  Run Homebrew's install script by using the command below. The script will
    guide you through the rest of the install process.

    ```
$ ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
    ```

#### Install the `arm-none-eabi` formula

1.  "Tap" into the CS107e Homebrew packages repository using the command below.
    This allows you to use the Homebrew formulas we have provided.

    ```
$ brew tap cs107e/cs107e
    ```

2.  Install the `arm-none-eabi` toolchain:

    ```
$ brew install arm-none-eabi
    ```

3.  Ensure it works by running the following command and checking that its
    output is identical to what is displayed below:

    ```
$ arm-none-eabi-gcc --version
arm-none-eabi-gcc (GCC) 4.8.3
Copyright (C) 2013 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    ```

### Console drivers and script installation

To use the console cable to send information from and to your Pi, you'll need
to install a few things. These instructions will guide you through those steps.

1.  Install the CP210x USB to UART Drivers

    [Download Version
    5](https://www.silabs.com/products/mcu/Pages/USBtoUARTBridgeVCPDrivers.aspx)
    of the drivers. Clicking on the download link will download the file
    `Mac_OSX_VCP_Driver.zip`. Opening this file will uncompress and you will
    see a new file, `SiLabsUSBDriverDisk.dmg`. Opening the dmg file, will mount
    the volume.

    You should see a volume named "Silicon Labs VCP Driver Install Disk" in the
    finder under devices. Going to that folder, you will see an installed named
    "Silicon Labs VCP Driver." Opening that file will launch the
    installer. Follow the instructions to complete the installation. You will need
    to restart the computer after the installation.

    **NOTE**: Users of the latest macOS (known issue on High Sierra Version
    10.13.1) may see a message saying "System Extension Blocked". Open the
    `Security & Privacy` menu in `Settings`. Under the `General` tab, click the
    `Allow` button next to the message indicating the blocked system extension.

2.  If you are in lab and got your kit, plug the USB breakout board
    into a USB port on your laptop. Check whether the serial port
    exists by looking for the device file:

    ```
$ ls /dev/tty.SLAB_USBtoUART
/dev/tty.SLAB_USBtoUART

    ```
    ![USB breakout board](../images/usb.breakout.board.JPG)

3.  Ensure you have `pip`, a Python package manager, installed. If you do not,
    run `sudo easy_install pip`.

4.  Install the `rpi-install.py` Python script using Homebrew:

    ```
$ sudo pip install pyserial
$ sudo pip install xmodem
$ brew install rpi-install
    ```

    If `sudo pip install xmodem` complains `OSError: [Errno 1]
    Operation not permitted:
    '/System/Library/Frameworks/Python.framework/Versions/2.7/doc'`,
    don't worry: just keep going with the `brew install rpi-install`.

    (If you have trouble with brew not recognizing your python has a valid pyserial/xmodem, try instead the command `brew install rpi-install --ignore-dependencies`)

5.  Check that everything is working by running `rpi-install.py -h`. You should
    see the output below:

~~~
$ rpi-install.py -h
usage: rpi-install.py [-h] [-v] [-q] [-t T] [-p | -s] [port] file

This script sends a binary file to the Raspberry Pi bootloader. Version 0.8.

positional arguments:
  port        serial port
  file        binary file to upload

optional arguments:
  -h, --help  show this help message and exit
  -v          verbose logging of serial activity
  -q          do not print while uploading
  -t T        timeout for -p
  -p          print output from the Pi after uploading
  -s          open screen on the serial port after uploading
~~~
