#include "headphonejack.h"
#include "gpio.h"
#include "clock.h"
#include "pwm.h"

extern void PUT32(unsigned int, unsigned int);

void headphoneOutputInit() {
    // Set GPIO 40 and GPIO 45 to Alternate PWM function 0
    pinMode(40, GPIO_ALT_FUNCTION_0);
    pinMode(45, GPIO_ALT_FUNCTION_0);

    // Set the PWM clock divisor
    setClockDivisor(CM_PWMDIV, 2, 0);

    // Set the PWM clock source
    setClockSource(CM_PWMCTL, CLK_SRC_PLLD_500MHZ);

    // Enable the PWM clock
    enableClock(CM_PWMCTL);

    // Set the PWM range
    setPwmRange(1, 0x2C48);
    setPwmRange(2, 0x2C48);

    // Set the PWM control flags
    setPwmControlFlags(PWM_USEF1 + PWM_USEF2 + PWM_PWEN1 + PWM_PWEN2 + PWM_CLRF1);
}

unsigned char headphoneOutputReady() {
    return pwmOutputReady();
}

void headphoneOutputWrite(unsigned int audio_sample) {
    // Convert from 16-bit to 14-bit, then output via PWM
    pwmOutputWrite(audio_sample >> 2);
}
