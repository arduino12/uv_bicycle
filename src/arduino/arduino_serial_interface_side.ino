#include "leds.h"
#include "heb_5x7.h"
#include "graphics.h"
#include "interactive_menu.h"

/******************************************/
#define SLICE_ON_MS			(70)		/* time to turn on leds slice (vertical pixels) */
#define SLICE_OFF_MS		(0)			/* time to turn off leds after each slice is draw */
#define CHAR_OFF_MS			(90)		/* time to turn off leds after each char is draw */
#define RIGHT_TO_LEFT		(true)		/* draw the chars from left or right */
#define SERIAL_BAUD_RATE	(115200)
#define SERIAL_TIMEOUT		(1)
/******************************************/


void setup() {
	Serial.begin(SERIAL_BAUD_RATE);
	Serial.setTimeout(SERIAL_TIMEOUT);
	leds_init();
	graphics_set_font(heb_5x7);
	graphics_set_draw(SLICE_ON_MS, SLICE_OFF_MS, CHAR_OFF_MS, RIGHT_TO_LEFT);
	print_help();
}

void loop() {
	interactive_menu_update();
}
