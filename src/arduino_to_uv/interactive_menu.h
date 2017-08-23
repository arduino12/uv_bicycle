#ifndef INTERACTIVE_MENU_H
#define INTERACTIVE_MENU_H

#include "utf8.h"
#include "leds.h"
#include "common.h"
#include "graphics.h"
#include "fix_texts.h"

/* max amount of bytes for a command */
#define COMMAND_BUFFER_SIZE			(254)
/* max amount of milliseconds for command buffer to grow */
#define COMMAND_TIMEOUT_MS			(150)
/* next screen saver will be draw if command was not recived after that amount of seconds */
#define SCREEN_SAVER_S				(90)

struct {
	char buffer[COMMAND_BUFFER_SIZE];
	uint8_t buffer_index = 0;
	uint32_t timeout = 0;
} command;

uint32_t screen_saver_ms = (uint32_t)SCREEN_SAVER_S * 1000;

/* prints human readable commands help */
void print_help() {
	Serial.println(F(
		"                  /'\\\n"
		"                 /\n"
		"                /                 ,\n"
		"             c-'                 /\n"
		"            /'-._         ,____,' .-'''-.\n"
		"       .-'.// \\  '-;-========,\"-,'       '\n"
		"     ,`   /,   \\_//\\       ,/  (  '- *)   )\n"
		"    (   ./  )   {,}========'===='- '     ,\n"
		"     ,     ,   \\/               ',      .\n"
		" _____'-.-`_______________________'-..-'____\n"
		"\n"
		"  U V   B i c y c l e   I n t e r f a c e:\n"
		"____________________________________________\n"
		"\n"
		"A <slice_on_ms[0-32K]>\n"
		"    LEDs on time\n"
		"B <slice_off_ms[0-32K]>\n"
		"    LEDs off time\n"
		"C <char_off_ms[0-32K]>\n"
		"    LEDs off time between characters\n"
		"D <right_to_left[0-1]>\n"
		"    Draw to the left if zero\n"
		"E <text[printable ascii and hebrew]>\n"
		"    Draw the given text\n"
		"F <text_index[0-4]>\n"
		"    Draw fix text\n"
		"G <screen_saver_s[0-32K]>\n"
		"    Draw next fix text after inactivity\n"
		"H <>\n"
		"    Print help\n"
		"I <leds_bitmask[0-32K]>\n"
		"    Set LEDs to the given bitmask\n"
	));
}

/* reads a number from the command buffer */
uint16_t _interactive_menu_read_16(uint16_t number) {
	uint16_t i = number;
	sscanf(command.buffer + 2, "%d", &i);
	return i;
}

/* must be called repeatedly from loop, this is our busy loop */
void interactive_menu_update() {
	uint32_t cur_millis = millis();
	uint16_t i;

	/* is it the time for a screen saver? */
	if (cur_millis > command.timeout) {
		if (command.buffer_index)
			command.buffer_index = 0;
		else if ((cur_millis - command.timeout) > screen_saver_ms) {
			command.timeout = cur_millis;
			graphics_set_text(FIX_TEXTS[cur_millis % ARRAY_SIZE(FIX_TEXTS)]);
		}
	}

	/* got a command? */
	if (Serial.available()) {
		if (!command.buffer_index)
			command.timeout = cur_millis + COMMAND_TIMEOUT_MS;

		command.buffer_index += Serial.readBytes(command.buffer + command.buffer_index,
			sizeof(command.buffer) - command.buffer_index);

		i = command.buffer[command.buffer_index - 1];
		if (command.buffer_index >= 2 && i == '\n' || i == '\r') {
			i = command.buffer[command.buffer_index - 2];
			i = ((i == '\n' || i == '\r') ? 3 : 2);
			while (i--)
				command.buffer[command.buffer_index - i] = '\0';

			switch (command.buffer[0]) {
				case 'H':
					print_help();
				break;
				case 'A':
					graphics_clear_text();
					graphics_draw.slice_on_ms = _interactive_menu_read_16(graphics_draw.slice_on_ms);
					Serial.print(F("slice_on_ms = ")); Serial.println(graphics_draw.slice_on_ms);
				break;
				case 'B':
					graphics_clear_text();
					graphics_draw.slice_off_ms = _interactive_menu_read_16(graphics_draw.slice_off_ms);
					Serial.print(F("slice_off_ms = ")); Serial.println(graphics_draw.slice_off_ms);
				break;
				case 'C':
					graphics_clear_text();
					graphics_draw.char_off_ms = _interactive_menu_read_16(graphics_draw.char_off_ms);
					Serial.print(F("char_off_ms = ")); Serial.println(graphics_draw.char_off_ms);
				break;
				case 'D':
					graphics_clear_text();
					graphics_draw.right_to_left = _interactive_menu_read_16(graphics_draw.right_to_left);
					Serial.print(F("right_to_left = ")); Serial.println(graphics_draw.right_to_left);
				break;
				case 'E':
					utf8_from_iso_8859_8(command.buffer + 2);
					graphics_set_text(command.buffer + 2);
					Serial.print(F("text_char_count = ")); Serial.println(graphics_text.text_char_count);
				break;
				case 'F':
					i = _interactive_menu_read_16(0) % ARRAY_SIZE(FIX_TEXTS);
					graphics_set_text(FIX_TEXTS[i]);
					Serial.print(F("fix_text_index = ")); Serial.println(i);
				break;
				case 'G':
					screen_saver_ms = _interactive_menu_read_16(screen_saver_ms / 1000);
					Serial.print(F("screen_saver_s = ")); Serial.println(screen_saver_ms);
					screen_saver_ms *= 1000;
				break;
				case 'I':
					i = _interactive_menu_read_16(0);
					Serial.print(F("leds_mask = ")); Serial.println(i, BIN);
					leds_write(i);
				break;
			}
			command.buffer_index = 0;
		}
	}

	/* keep drawing, and notify when finish a slice, a char, and the whole text */
	graphics_update(cur_millis);
	switch (graphics_draw.status) {
		case GRAPHICS_DRAW_SLICE:
			Serial.print('.');
		break;
		case GRAPHICS_DRAW_CHAR:
			Serial.print('#');
		break;
		case GRAPHICS_DRAW_TEXT:
			Serial.println('@');
			graphics_clear_text();
		break;
	}
}

#endif /* INTERACTIVE_MENU_H */
