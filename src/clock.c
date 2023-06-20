#include "clock.h"

extern void PUT32(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);

void setClockDivisor(unsigned int clock_divider_register, unsigned int integer_part, unsigned int fractional_part)
{
    PUT32(clock_divider_register, (CM_PASSWD << 24) | (integer_part << 12) | (fractional_part));
}

void setClockSource(unsigned int clock_manager_register, unsigned int clock_source)
{
   unsigned int old_value = GET32(clock_manager_register);
   PUT32(clock_manager_register, old_value | (CM_PASSWD << 24) | clock_source);
}

void enableClock(unsigned int clock_manager_register)
{
   unsigned int old_value = GET32(clock_manager_register);
   PUT32(clock_manager_register, old_value | (CM_PASSWD << 24) | (1 << 4));
}
