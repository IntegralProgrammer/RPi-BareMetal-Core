// SD function return codes
#define SD_OK 0
#define SD_TIMEOUT 1
#define SD_ERROR 2
#define SD_INCORRECT_ALIGNMENT 3

typedef struct {
	unsigned char ccs;
} SDCard;

unsigned char sdInit(SDCard* sdCard);
unsigned char sdReadBlockSync(SDCard* sdCard, unsigned char* buffer, unsigned int address);
unsigned char sdWriteBlockSync(SDCard* sdCard, unsigned char* buffer, unsigned int address);
