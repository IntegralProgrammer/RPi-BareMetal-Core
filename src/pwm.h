#define PWM_RNG1 0x2020C010
#define PWM_RNG2 0x2020C020

#define PWM_CTL 0x2020C000

#define PWM_DAT1 0x2020C014
#define PWM_FIF1 0x2020C018
#define PWM_STA 0x2020C004

#define PWM_USEF1 0x20
#define PWM_USEF2 0x2000
#define PWM_PWEN1 0x1
#define PWM_PWEN2 0x100
#define PWM_CLRF1 0x40
#define PWM_FULL1 0x1


void setPwmRange(unsigned int, unsigned int);
void setPwmControlFlags(unsigned int);
unsigned char pwmOutputReady();
void pwmOutputWrite(unsigned int);
void pwmDirectOutputWrite(unsigned int);
