#define GPFSEL0 0x20200000
#define GPFSEL1 0x20200004
#define GPFSEL2 0x20200008
#define GPFSEL3 0x2020000C
#define GPFSEL4 0x20200010
#define GPFSEL5 0x20200014

#define GPSET0 0x2020001C
#define GPSET1 0x20200020

#define GPCLR0 0x20200028
#define GPCLR1 0x2020002C

#define GPLEV0 0x20200034
#define GPLEV1 0x20200038

#define INPUT 0x000
#define OUTPUT 0x001

#define HIGH 1
#define LOW 0

void pinMode(unsigned int pin, unsigned int mode);
unsigned int digitalRead(unsigned int pin);
void digitalWrite(unsigned int pin, unsigned int value);
