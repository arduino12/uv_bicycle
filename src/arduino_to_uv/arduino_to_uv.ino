/*
 * Arduino code for rotating led/laser array.
 */
#include "leds.h"
#include "graphics.h"
#include "interactive_menu.h"

/* 0=hebrew, 1=german */
#define LANGUAGE_CODE		(1)
/* 0=7_lasers, 1=14_lasers */
#define HARDWARE_VERSION	(1)
/* uart baud rate in bit per seconds */
#define SERIAL_BAUD_RATE	(115200)
/* set uart read timeout in milliseconds */
#define SERIAL_TIMEOUT		(1)
/* default characters draw diraction bitmask GRAPHICS_RIGHT_TO_LEFT=1, GRAPHICS_TOP_TO_BOTTOM=2*/
#define DRAW_DIRACTION		(LANGUAGE_CODE == 0)
/* turn on leds slice milliseconds (vertical pixels) */
#define SLICE_ON_MS			(70)
/* turn off leds milliseconds after each slice is draw */
#define SLICE_OFF_MS		(0)
/* turn off leds milliseconds after each char is draw */
#define CHAR_OFF_MS			(90)


#if HARDWARE_VERSION == 0
	#define GRAPHICS_FONT	(heb_5x7)
	#include "heb_5x7.h"
#else
	#define GRAPHICS_FONT	(heb_ger_11x13)
	#include "heb_ger_11x13.h"
#endif


void setup()
{
	Serial.begin(SERIAL_BAUD_RATE);
	Serial.setTimeout(SERIAL_TIMEOUT);
	leds_init();
	graphics_set_font(GRAPHICS_FONT);
	graphics_set_draw(SLICE_ON_MS, SLICE_OFF_MS, CHAR_OFF_MS, DRAW_DIRACTION);
	print_help();
}

void loop()
{
	interactive_menu_update();
}
