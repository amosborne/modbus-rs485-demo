# Embedded Software Overview

The target device is the [MSP430G2553](https://www.ti.com/lit/ds/symlink/msp430g2553.pdf).

## Custom Flash-Based Bootloader (BSL)

1. [MSP430 Flash Devices Bootloader (BSL)](https://www.ti.com/lit/ug/slau319ae/slau319ae.pdf)
2. [Creating a Custom Flash-Based Bootloader (BSL)](https://www.ti.com/lit/an/slaa450g/slaa450g.pdf?ts=1656821564806)
3. [MSP Flasher](https://www.ti.com/lit/ds/symlink/msp430g2553.pdf)

The MPS430G2553 in-built BSL is hardcoded in ROM and only provides for a software UART entry sequence at specific GPIO pins. The desired functionality is for the master to enter the BSL when the boot button is pressed following a device reset; device serial communication will be over hardware UART. Slaves will enter the BSL upon receiving a specific command over the UART.

To accomplish this functionality, a custom bootloader must be implemented in flash memory. Due to limited available space in the device information memory segment (which is write-protected), the custom bootloader will be written into a single 512 byte data memory segment.

Only a subset of the BSL430 command protocol is to be implemented (without password protection): RX data block (with online verification), erase segment (with online verification), and load PC.

The typical sequence for programming the device over the BSL is as follows:

1. Custom BSL entry sequence
2. Erase segment(s)
3. Verify segment erasure
4. Write segment(s)
5. Load PC

The application code must provide for software entry into the BSL. In addition to checking the BSL entry condition, the application code must disable the watchdog, setup the hardware UART, and disable all interrupts. With the interrupts disabled, receiving bytes over UART must be accomplished by polling. After exiting the BSL the master will provide for pressing of the boot button to toggle the master between master and slave mode such that the multi-drop UART bus may be driven by an external master.

Programming the BSL and bootstrapping the device application code must be done over SBW (MSP Flasher is used as the command line utility to send serial data to the MSP430 FET over USB).
