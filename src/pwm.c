#include "pwm.h"

extern void PUT32(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);

void setPwmRange(unsigned int pwm_channel, unsigned int range)
{
    if (pwm_channel == 1) {
        PUT32(PWM_RNG1, range);
    } else if (pwm_channel == 2) {
        PUT32(PWM_RNG2, range);
    }
}

void setPwmControlFlags(unsigned int control_flags)
{
    PUT32(PWM_CTL, control_flags);
}

unsigned char pwmOutputReady() {
    if (GET32(PWM_STA) & PWM_FULL1) {
        return 0;
    } else {
        return 1;
    }
}

void pwmOutputWrite(unsigned int value) {
    PUT32(PWM_FIF1, value);
}

void pwmDirectOutputWrite(unsigned int value) {
    PUT32(PWM_DAT1, value);
}
