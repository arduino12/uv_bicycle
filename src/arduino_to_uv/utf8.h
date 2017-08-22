#ifndef UTF8_H
#define UTF8_H

#include "common.h"

/******************************************/
#define UNICODE_BITS		16
/******************************************/

#define BAD_CHAR					'#'

#define unicode_t			UINT_BITS(UNICODE_BITS)

static const unicode_t UTF8_OFFSETS[] = {
	0x00000000,
	0x00003080,
#if UNICODE_BITS > 16
	0x000E2080,
	0x03C82080,
#endif
#if UNICODE_BITS > 32
	0xFA082080,
	0x82082080,
#endif
};

uint8_t utf8_trailing(uint8_t c) {
	if (c < 192) return 0;
#if UNICODE_BITS > 16
	if (c < 224) return 1;
	if (c < 240) return 2;
#endif
#if UNICODE_BITS > 32
	if (c < 248) return 3;
	if (c < 252) return 4;
#endif
#if UNICODE_BITS == 16
	return 1;
#elif UNICODE_BITS == 32
	return 3;
#else
	return 5;
#endif
}

uint16_t utf8_to_unicode(const char * src, unicode_t * dst, uint16_t dst_size) {
	unicode_t ch;
	uint16_t i = 0;
	uint8_t trailing;

	while (i < (dst_size - 1) && *src) {
		trailing = utf8_trailing(*src);
		ch = 0;
		switch (trailing) {
#if UNICODE_BITS > 32
			case 5: ch += (uint8_t)*src++; ch <<= 6;
			case 4: ch += (uint8_t)*src++; ch <<= 6;
#endif
#if UNICODE_BITS > 16
			case 3: ch += (uint8_t)*src++; ch <<= 6;
			case 2: ch += (uint8_t)*src++; ch <<= 6;
#endif
			case 1: ch += (uint8_t)*src++; ch <<= 6;
			case 0: ch += (uint8_t)*src++;
		}
		dst[i++] = ch - UTF8_OFFSETS[trailing];
	}

	dst[i] = 0;
	return i;
}

unicode_t utf8_char_at(const char * text, uint16_t index) {
    unicode_t ch = 0;

	while(*text && index--)
		text += utf8_trailing(*text) + 1;

	uint8_t trailing = utf8_trailing(*text);
	index = trailing;

	do {
		ch <<= 6;
		ch += (uint8_t)*text++;
	} while (*text && index--);

	return ch - UTF8_OFFSETS[trailing];
}

uint16_t utf8_char_count(const char * text) {
	uint8_t char_count = 0;
	while(*text && ++char_count)
		text += utf8_trailing(*text) + 1;
	return char_count;
}

void utf8_from_iso_8859_8(char * text) {
	unicode_t ch;
	char * scr = text;
	char * tmp = (char*)malloc(strlen(text) * 2 + 1);
	char * dst = tmp;
	if (tmp == NULL)
		return;

	while (ch = (uint8_t)*scr++) {
		if (ch < 0x80) {
			*dst++ = ch;
		}
		else if (ch >= 0xE0 && ch < 0xFA) {
			ch += 0x05D0 - 0xE0;
			*dst++ = (ch >> 6) | 0xC0;
			*dst++ = (ch & 0x3F) | 0x80;
		}
		else {
			free(tmp);
			return;
		}
	}
	*dst = '\0';
	strcpy(text, tmp);
	free(tmp);
}

#endif /* UTF8_H */
