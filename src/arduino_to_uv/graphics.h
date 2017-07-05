#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "leds.h"
#include "utf8.h"

#define GRAPHICS_FONT_HEIGHT		(0)
#define GRAPHICS_FONT_FIRST_CHAR	(1)
#define GRAPHICS_FONT_CHAR_COUNT	(2)
#define GRAPHICS_FONT_TABLE			(3)
#define GRAPHICS_FONT_TABLE_ROWS	(3)
#define GRAPHICS_FONT_TABLE_MSB		(1)
#define GRAPHICS_FONT_TABLE_LSB		(2)

#define GRAPHICS_DRAW_EMPTY_TEXT	(0)
#define GRAPHICS_DRAW_ON_IDLE		(1)
#define GRAPHICS_DRAW_OFF_IDLE		(2)
#define GRAPHICS_DRAW_SLICE			(3)
#define GRAPHICS_DRAW_CHAR			(4)
#define GRAPHICS_DRAW_TEXT			(5)

struct {
	uint8_t * font = NULL;
	uint8_t height;
	uint8_t bytes;
	uint8_t first_char;
	uint8_t last_char;
} graphics_font;

struct {
	uint8_t * text = NULL;
	uint16_t text_char_count;
	uint16_t text_char_index;
	uint8_t char_slices;
	uint8_t char_slice;
	uint16_t char_offset;
} graphics_text;

struct {
	uint8_t status;
	uint16_t slice_on_ms;
	uint16_t slice_off_ms;
	uint16_t char_off_ms;
	uint32_t next_millis;
	bool right_to_left;
} graphics_draw;


uint8_t _read_font_8(uint16_t index) {
	return pgm_read_byte(&graphics_font.font[index]);
}

uint8_t _graphics_unicode_to_iso_8859_8(unicode_t ch) {
	if (ch >= 0x20 && ch < 0x80)
		return ch;
	if (ch >= 0x05D0 && ch < 0x05EB)
		return ch - 0x05D0 + 0x80;
	return BAD_CHAR;
}

void _graphics_set_char() {
	unicode_t ch = utf8_char_at(graphics_text.text, graphics_text.text_char_index);
	ch = _graphics_unicode_to_iso_8859_8(ch); /* fix by unicode native font */

	if (ch < graphics_font.first_char || ch > graphics_font.last_char)
		ch = BAD_CHAR;

	graphics_text.char_offset = GRAPHICS_FONT_TABLE + (ch - graphics_font.first_char) * GRAPHICS_FONT_TABLE_ROWS;
	graphics_text.char_slices = _read_font_8(graphics_text.char_offset);
	graphics_text.char_offset = ((uint16_t)_read_font_8(graphics_text.char_offset + GRAPHICS_FONT_TABLE_MSB) << 8) +
		_read_font_8(graphics_text.char_offset + GRAPHICS_FONT_TABLE_LSB);
	graphics_text.char_slice = 0;
}

void _graphics_draw_char_slice() {
	leds_bitmask_t leds_bitmask = 0;
	for(uint8_t y = 0; y < graphics_font.bytes; y++) {
		leds_bitmask <<= 8;
		leds_bitmask |= _read_font_8(
			graphics_text.char_offset + y + graphics_font.bytes *
			(graphics_draw.right_to_left ? graphics_text.char_slices - graphics_text.char_slice - 1 : graphics_text.char_slice));
	}
	leds_write(leds_bitmask);
}

void graphics_clear_text() {
	if (graphics_text.text == NULL)
		return;
	free(graphics_text.text);
	graphics_text.text = NULL;
	leds_write(0);
}

void graphics_set_text(const uint8_t * text) {
	graphics_clear_text();
	if (!*text)
		return;
	graphics_text.text = malloc(strlen(text) + 1);
	if (graphics_text.text == NULL)
		return;
	strcpy(graphics_text.text, text);
	graphics_text.text_char_count = utf8_char_count(graphics_text.text);
	graphics_text.text_char_index = graphics_text.text_char_count;
	graphics_text.char_slices = 0;
	graphics_draw.status = GRAPHICS_DRAW_OFF_IDLE;
}

void graphics_update(uint32_t cur_millis) {
	if (graphics_text.text == NULL)
		graphics_draw.status = GRAPHICS_DRAW_EMPTY_TEXT;
	
	switch (graphics_draw.status) {
		case GRAPHICS_DRAW_ON_IDLE:
			if (cur_millis > graphics_draw.next_millis) {
				if (graphics_text.char_slice >= (graphics_text.char_slices - 1)) {
					leds_write(0);
					if (graphics_text.text_char_index >= (graphics_text.text_char_count - 1))
						graphics_draw.status = GRAPHICS_DRAW_TEXT;
					else {
						graphics_draw.status = GRAPHICS_DRAW_CHAR;
						graphics_draw.next_millis = cur_millis + graphics_draw.char_off_ms;
					}
				}
				else {
					if (graphics_draw.slice_off_ms)
						leds_write(0);
					graphics_draw.status = GRAPHICS_DRAW_SLICE;
					graphics_draw.next_millis = cur_millis + graphics_draw.slice_off_ms;
				}
			}
		break;
		case GRAPHICS_DRAW_SLICE:
		case GRAPHICS_DRAW_CHAR:
		case GRAPHICS_DRAW_TEXT:
			graphics_draw.status = GRAPHICS_DRAW_OFF_IDLE;
		case GRAPHICS_DRAW_OFF_IDLE:
			if (cur_millis > graphics_draw.next_millis) {
				if (++graphics_text.char_slice >= graphics_text.char_slices) {
					if (++graphics_text.text_char_index >= graphics_text.text_char_count)
						graphics_text.text_char_index = 0;
					_graphics_set_char();
				}
				_graphics_draw_char_slice();
				graphics_draw.next_millis = cur_millis + graphics_draw.slice_on_ms;
				graphics_draw.status = GRAPHICS_DRAW_ON_IDLE;
			}
		break;
	}
}

void graphics_set_font(uint8_t * font) {
	graphics_clear_text();
	graphics_font.font = font;
	graphics_font.height = _read_font_8(GRAPHICS_FONT_HEIGHT);
	graphics_font.bytes = (graphics_font.height + 7) / 8;
	graphics_font.first_char = _read_font_8(GRAPHICS_FONT_FIRST_CHAR);
	graphics_font.last_char = graphics_font.first_char + _read_font_8(GRAPHICS_FONT_CHAR_COUNT) - 1;
}

void graphics_set_draw(uint16_t slice_on_ms, uint16_t slice_off_ms, uint16_t char_off_ms, bool right_to_left) {
	graphics_clear_text();
	graphics_draw.slice_on_ms = slice_on_ms;
	graphics_draw.slice_off_ms = slice_off_ms;
	graphics_draw.char_off_ms = char_off_ms;
	graphics_draw.right_to_left = right_to_left;
}

#endif /* GRAPHICS_H */
