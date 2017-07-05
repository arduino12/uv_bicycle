#ifndef CAT4016_H
#define CAT4016_H

#include "common.h"

#define CAT4016_PORT			(PORTD)
#define CAT4016_LATCH_PIN		(4)
#define CAT4016_CLK_PIN			(5)
#define CAT4016_SIN_PIN			(6)
#define CAT4016_SET(pin)		sbi(CAT4016_PORT, pin)
#define CAT4016_CLEAR(pin)		cbi(CAT4016_PORT, pin)
#define CAT4016_PULSE(pin)		do {CAT4016_SET(pin); CAT4016_CLEAR(pin);} while(0)


void cat4016_write(uint8_t * buffer, uint8_t buffer_size) {
	while (buffer_size--) {
		for (uint8_t mask = 1; mask; mask <<= 1) {
			if (*buffer & mask)
				CAT4016_SET(CAT4016_SIN_PIN);
			else
				CAT4016_CLEAR(CAT4016_SIN_PIN);
			CAT4016_PULSE(CAT4016_CLK_PIN);
		}
		buffer++;
	}
	CAT4016_PULSE(CAT4016_LATCH_PIN);
}

void cat4016_setup() {
	pinMode(CAT4016_LATCH_PIN, OUTPUT);
	pinMode(CAT4016_CLK_PIN, OUTPUT);
	pinMode(CAT4016_SIN_PIN, OUTPUT);

	CAT4016_CLEAR(CAT4016_LATCH_PIN);
	CAT4016_CLEAR(CAT4016_CLK_PIN);
	CAT4016_CLEAR(CAT4016_SIN_PIN);
}

#endif /* CAT4016_H */
