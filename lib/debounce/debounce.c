#include "debounce.h"
#include "../../src/gpio.h"

void initDebouncedButton(DebouncedButton* db, unsigned int pin, unsigned int debounceTime, unsigned char edgeDirection) {
	db->pin = pin;
	db->debounceTime = debounceTime;
	db->debounceWaiting = 0;
	pinMode(pin, INPUT);
	if (edgeDirection == 0) {
		// Trigger on falling edges
		detectAsyncFallingEdge(pin);
	} else {
		// Trigger on rising edges
		detectAsyncRisingEdge(pin);
	}
}

unsigned char debouncedButtonRead(DebouncedButton* db) {
	unsigned char edgeDetected = eventDetected(db->pin);
	unsigned char debouncedValue = 0;
	if (edgeDetected) {
		if (db->debounceWaiting == 0) {
			debouncedValue = 1;
		}
		db->debounceWaiting = db->debounceTime;
		clearEvent(db->pin);
	} else {
		if (db->debounceWaiting > 0) {
			db->debounceWaiting -= 1;
		}
	}

	return debouncedValue;
}
