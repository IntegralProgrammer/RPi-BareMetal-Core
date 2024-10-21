Simple program to test reading from and writing to the SPI interface on the Raspberry Pi

Usage
-----

1. Connect a USB to 3v3 TTL UART adapter to the UART pins on the Raspberry Pi and open a serial console at 115200 baud

2. Press any key at the prompt to perform one test of the SPI interface

3. If SPI MISO (GPIO 9) is connected to SPI MOSI (GPIO 10) via 1k resistor, a counter variable counting from `0x0000` to `0xFFFF` will be displayed

4. If SPI MISO (GPIO 9) is connected to GND via 1k resistor, the value `0x0000` will always be displayed

5. If SPI MISO (GPIO 9) is connected to 3.3v via 1k resistor, the value `0xFFFF` will always be displayed

6. To visually see the output signals on the SPI MOSI (GPIO 10), SPI SCLK (GPIO 11), and SPI CE (GPIO 8) pins, LEDs can be connected to these pins via 1k resistors
