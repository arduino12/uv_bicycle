#ifndef LEDS_H
#define LEDS_H

#include "common.h"

/* uncomment if using musium lens leds configuration (led gruop of 3) */
// #define USE_CAT4016
/* leds count in multiples of 8 */
#define LEDS_BITS				32
#define leds_bitmask_t			UINT_BITS(LEDS_BITS)

#ifdef USE_CAT4016
	#include "cat4016.h"

	/******************************************/
	// #define USE_LENS_LED
	/******************************************/

	#ifdef USE_LENS_LED
		#define LED_GROUP_SIZE	(3)
	#else
		#define LED_GROUP_SIZE	(2)
	#endif
	#define LED_MAPPING(index)	((((leds_bitmask_t)1 << LED_GROUP_SIZE) - 1) << (index * LED_GROUP_SIZE))

	const leds_bitmask_t LEDS_MAPPING[] = {
	#ifdef USE_LENS_LED
		LED_MAPPING(0),
		LED_MAPPING(1),
		LED_MAPPING(2),
		LED_MAPPING(3),
		LED_MAPPING(4),
		LED_MAPPING(5) << 1,
		LED_MAPPING(6) << 1,
		LED_MAPPING(7) << 1,
		#if LEDS_BITS > 8
			LED_MAPPING(8) << 1,
			LED_MAPPING(9) << 1,
		#endif
	#else
		LED_MAPPING(0),
		LED_MAPPING(11),
		LED_MAPPING(2),
		LED_MAPPING(3),
		LED_MAPPING(10),
		LED_MAPPING(5),
		LED_MAPPING(6),
		LED_MAPPING(7),
		#if LEDS_BITS > 8
			LED_MAPPING(8),
			LED_MAPPING(9),
		#endif
	#endif
	};

	#define LEDS_COUNT			(ARRAY_SIZE(LEDS_MAPPING))
#else
	#define LEDS_COUNT			(10)
#endif

/* set the leds by the given bitmask (0 = all off, 5 = first and third on) */
void leds_write(leds_bitmask_t leds_bitmask) {
#ifdef USE_CAT4016
	union {
		leds_bitmask_t bitmask = 0;
		uint8_t buffer[LEDS_BITS / 8];
	} cat4016;

	for (uint8_t i = 0; i < LEDS_COUNT; i++, leds_bitmask >>= 1)
		if (leds_bitmask & 1)
			cat4016.bitmask |= LEDS_MAPPING[i];

	cat4016_write(cat4016.buffer, sizeof(cat4016.buffer));
#else
	PORTD = (PORTD & 0x03) | ((leds_bitmask & 0x003F) << 2);
	PORTB = (PORTB & 0xF0) | ((leds_bitmask & 0x03C0) >> 6);
	PORTC = (PORTC & 0xF0) | ((leds_bitmask & 0x3C00) >> 10);
#endif
}

/* initialize gpios / led driver, must be called at setup */
void leds_init() {
#ifdef USE_CAT4016
	cat4016_setup();
#else
	DDRD |= 0xFC;	/* (2 - 7 OUTPUT) */
	DDRB |= 0x0F;	/* (8 - 11 OUTPUT) */
	DDRC |= 0x0F;	/* (A0 - A3 OUTPUT) */
#endif
	leds_write(0);
}

#endif /* LEDS_H */
