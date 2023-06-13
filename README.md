# RPi-BareMetal-Core

Arduino-inspired programming environment for bare-metal software development on the Raspberry Pi 1

### Warning

:construction: :construction: :construction: **This software is still in its experimental/early-development stage** :construction: :construction: :construction:

### Setup

On Debian/Ubuntu `apt install` the following packages:
- `make`
- `gcc-arm-none-eabi`

### Building

To build an example, enter the example's directory (eg. `cd examples/gpio_leds`) and run `make kernel.img`

To run the example, copy `kernel.img` to the _root_ directory of a FAT32 formatted SD card along with the
following files from https://github.com/raspberrypi/firmware/tree/master/boot

- `bootcode.bin`
- `start.elf`

Insert the SD card into the Raspberry Pi and power on.
