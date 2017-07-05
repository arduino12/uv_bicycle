arduino_test_serial.ino:
========================

lines 7-12:
#define SLICE_ON_MS			(70)		/* time to turn on leds slice (vertical pixels) */
#define SLICE_OFF_MS		(0)			/* time to turn off leds after each slice is draw */
#define CHAR_OFF_MS			(90)		/* time to turn off leds after each char is draw */
#define RIGHT_TO_LEFT		(true)		/* draw the chars from left or right */
#define SERIAL_BAUD_RATE	(115200)
#define SERIAL_TIMEOUT		(1)

change those numbers to control the draw speed and diraction.


leds.h:
=======

lines 7,8,17:
// #define USE_CAT4016
#define LEDS_BITS			32
// #define USE_LENS_LED

uncomment if using musium lens leds configuration (led gruop of 3)


interactive_menu.h:
===================

lines 10-19:
#define COMMAND_BUFFER_SIZE			(100)
#define COMMAND_TIMEOUT_MS			(150)
#define SCREEN_SAVER_S				(40)

const uint8_t * FIX_TEXTS[] = {
	"שלום עולם",
	"כל הכבוד לברוך!",
	"מוזיאון המדע בירושלים..!",
	"o_o  ^.^  O..O",
};

change those numbers to control the default menu values and fix texts.
