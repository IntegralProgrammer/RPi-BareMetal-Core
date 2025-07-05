typedef struct {
	unsigned int pin;
	unsigned int debounceTime;
	unsigned int debounceWaiting;
} DebouncedButton;

void initDebouncedButton(DebouncedButton* db, unsigned int pin, unsigned int debounceTime, unsigned char edgeDirection);
unsigned char debouncedButtonRead(DebouncedButton* db);
