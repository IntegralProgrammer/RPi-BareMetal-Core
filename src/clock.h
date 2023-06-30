#define CM_PASSWD 0x5A

#define CM_GP0CTL 0x20101070
#define CM_GP0DIV 0x20101074

#define CM_GP1CTL 0x20101078
#define CM_GP1DIV 0x2010107C

#define CM_GP2CTL 0x20101080
#define CM_GP2DIV 0x20101084

#define CM_PWMCTL 0x201010A0
#define CM_PWMDIV 0x201010A4

#define CLK_SRC_ROOT_19_2MHZ 0x1
#define CLK_SRC_PLLD_500MHZ 0x6


void setClockDivisor(unsigned int, unsigned int, unsigned int);
void setClockSource(unsigned int, unsigned int);
void enableClock(unsigned int);
