#include "leds.h"
#include "heb_5x7.h"
#include "graphics.h"
#include "interactive_menu.h"

/* turn on leds slice milliseconds (vertical pixels) */
#define SLICE_ON_MS			(70)
/* turn off leds milliseconds after each slice is draw */
#define SLICE_OFF_MS		(0)
/* turn off leds milliseconds after each char is draw */
#define CHAR_OFF_MS			(90)
/* default characters draw diraction */
#define RIGHT_TO_LEFT		(true)
/* uart baud rate in bit per seconds */
#define SERIAL_BAUD_RATE	(115200)
/* set uart read timeout in milliseconds */
#define SERIAL_TIMEOUT		(1)

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
