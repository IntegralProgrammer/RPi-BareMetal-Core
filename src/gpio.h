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

#define GPHEN0 0x20200064
#define GPHEN1 0x20200068
#define GPLEN0 0x20200070
#define GPLEN1 0x20200074
#define GPAREN0 0x2020007C
#define GPAREN1 0x20200080
#define GPAFEN0 0x20200088
#define GPAFEN1 0x2020008C
#define GPEDS0 0x20200040
#define GPEDS1 0x20200044
#define GPPUD 0x20200094
#define GPPUDCLK0 0x20200098
#define GPPUDCLK1 0x2020009C

#define INPUT 0x000
#define OUTPUT 0x001
#define GPIO_ALT_FUNCTION_0 0x004
#define GPIO_ALT_FUNCTION_1 0x005
#define GPIO_ALT_FUNCTION_2 0x006
#define GPIO_ALT_FUNCTION_3 0x007
#define GPIO_ALT_FUNCTION_4 0x003
#define GPIO_ALT_FUNCTION_5 0x002

#define MASK_GPIO_ENABLE_PULL_DOWN 0x01
#define MASK_GPIO_ENABLE_PULL_UP 0x02

#define HIGH 1
#define LOW 0

#define PULL_UP_DOWN_DISABLED 0
#define PULL_DOWN_ENABLED 1
#define PULL_UP_ENABLED 2


void pinMode(unsigned int pin, unsigned int mode);
void setPullUpDown(unsigned int pin, unsigned int mode);
unsigned int digitalRead(unsigned int pin);
void digitalWrite(unsigned int pin, unsigned int value);
void detectAsyncRisingEdge(unsigned int pin);
void detectAsyncFallingEdge(unsigned int pin);
void detectGpioHigh(unsigned int pin);
void detectGpioLow(unsigned int pin);
unsigned char eventDetected(unsigned int pin);
void clearEvent(unsigned int pin);
void waitForEvent(unsigned int pin);
