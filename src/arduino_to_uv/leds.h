#ifndef LEDS_H
#define LEDS_H

#include "common.h"

/* leds count in multiples of 8 */
#define LEDS_BITS				32
#define leds_bitmask_t			UINT_BITS(LEDS_BITS)

/*
 * ARDUINO PIN IN PCB:         LASER INDEX IN PCB
 *  ____ ____                   ____ ____
 * |_00_|_00_|___________      |_00_|_00_|___________
 * |_10_|_02_|   <GRAY>  \     |_03_|_05_|   <GRAY>  \
 * |_11_|_03_|   <GRAY>  /     |_01_|_02_|   <GRAY>  /
 * |_12_|_04_|   <GRAY>  \     |_--_|_04_|   <GRAY>  \
 * |_13_|_05_|   <GRAY>  /     |_--_|_06_|   <GRAY>  /
 * |_A0_|_06_|   <GRAY>  \     |_07_|_08_|   <GRAY>  \
 * |_A1_|_07_|   <GRAY>  /     |_09_|_10_|   <GRAY>  /
 * |_A2_|_08_|   <GRAY>  \     |_11_|_12_|   <GRAY>  \
 * |_A3_|_09_|___<RED>___/     |_13_|_14_|___<RED>___/
 * |_00_|_00_|                 |_00_|_00_|
*/

const uint8_t LED_PIN_MAP[] = {
	7, 13, 6, 12, 5, 11, 4, 10, 3, 2, 1, 9, 0, 8
};
#define LEDS_COUNT			(ARRAY_SIZE(LED_PIN_MAP))


/* set the leds by the given bitmask (0 = all off, 5 = first and third on) */
void leds_write(leds_bitmask_t leds_bitmask)
{
	leds_bitmask_t ports_bitmask = 0;

	for (uint8_t i = 0; i < LEDS_COUNT; i++, leds_bitmask >>= 1)
		if (leds_bitmask & 1)
			ports_bitmask |= (leds_bitmask_t)1 << LED_PIN_MAP[i];

	PORTD = (PORTD & 0x03) | ((ports_bitmask & 0x003F) << 2);
	PORTB = (PORTB & 0xF0) | ((ports_bitmask & 0x03C0) >> 6);
	PORTC = (PORTC & 0xF0) | ((ports_bitmask & 0x3C00) >> 10);
}

/* initialize gpios, must be called at setup */
void leds_init()
{
	DDRD |= 0xFC;	/* (2 - 7 OUTPUT) */
	DDRB |= 0x0F;	/* (8 - 11 OUTPUT) */
	DDRC |= 0x0F;	/* (A0 - A3 OUTPUT) */
	leds_write(0);
}

#endif /* LEDS_H */
