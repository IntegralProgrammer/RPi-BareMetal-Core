#include "gpio.h"

extern void PUT32(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);

void pinMode(unsigned int pin, unsigned int mode)
{
    unsigned int register_contents;
    unsigned int gpfsel_address;
    unsigned int pin_offset;
    if ((pin >= 0) && (pin <= 9)) {
        gpfsel_address = GPFSEL0;
        pin_offset = 0;
    } else if ((pin >= 10) && (pin <= 19)) {
        gpfsel_address = GPFSEL1;
        pin_offset = 10;
    } else if ((pin >= 20) && (pin <= 29)) {
        gpfsel_address = GPFSEL2;
        pin_offset = 20;
    } else if ((pin >= 30) && (pin <= 39)) {
        gpfsel_address = GPFSEL3;
        pin_offset = 30;
    } else if ((pin >= 40) && (pin <= 49)) {
        gpfsel_address = GPFSEL4;
        pin_offset = 40;
    } else if ((pin >= 50) && (pin <= 53)) {
        gpfsel_address = GPFSEL5;
        pin_offset = 50;
    } else {
        return;
    }
    register_contents = GET32(gpfsel_address);
    register_contents &= ~(7 << (3*(pin - pin_offset)));
    register_contents |= (mode << (3*(pin - pin_offset)));
    PUT32(gpfsel_address, register_contents);
}

unsigned int digitalRead(unsigned int pin)
{
    unsigned int gplev_address;
    unsigned int pin_offset;
    if ((pin >= 0) && (pin <= 31)) {
        gplev_address = GPLEV0;
        pin_offset = 0;
    } else if ((pin >= 32) && (pin <= 53)) {
        gplev_address = GPLEV1;
        pin_offset = 32;
    } else {
        return 0;
    }
    return (GET32(gplev_address) >> (pin - pin_offset)) & 1;
}

void digitalWrite(unsigned int pin, unsigned int value)
{
    unsigned int pin_offset;
    if ((pin >= 0) && (pin <= 31)) {
        pin_offset = 0;
        if (value == 0) {
            PUT32(GPCLR0, 1 << (pin - pin_offset));
        } else if (value == 1) {
            PUT32(GPSET0, 1 << (pin - pin_offset));
        }
    } else if ((pin >= 32) && (pin <= 53)) {
        pin_offset = 32;
        if (value == 0) {
            PUT32(GPCLR1, 1 << (pin - pin_offset));
        } else if (value == 1) {
            PUT32(GPSET1, 1 << (pin - pin_offset));
        }
    }
}
