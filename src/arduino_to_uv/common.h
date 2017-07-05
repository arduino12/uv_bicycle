#ifndef COMMON_H
#define COMMON_H

#ifndef cbi
	#define cbi(sfr, bit)	(_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
	#define sbi(sfr, bit)	(_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define ARRAY_SIZE(x)		(sizeof(x) / sizeof(*x))

#define _PASTE_2(a, b)		a ## b
#define PASTE_2(a, b)		_PASTE_2(a, b)

#define _PASTE_3(a, b, c)	a ## b ## c
#define PASTE_3(a, b, c)	_PASTE_3(a, b, c)

#define UINT_BITS(i)		PASTE_3(uint, i, _t)

#endif /* COMMON_H */
