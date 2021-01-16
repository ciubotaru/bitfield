/**
 * File name: bitfield-internals.h
 * Project name: bitfield, a bit array manipulation library written in C                                               * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie@ciubotaru.tokyo>
 * License: General Public License, version 3 or later
 * Copyright 2015, 2016, 2017, 2019
**/

#include "config.h"
#ifndef CHAR_BIT
#include <limits.h>
#endif
#ifndef SHORT_BIT
#define SHORT_BIT (unsigned int) (SIZEOF_UNSIGNED_SHORT * CHAR_BIT)
#endif
#ifndef INT_BIT
#define INT_BIT (unsigned int) (SIZEOF_UNSIGNED_INT * CHAR_BIT)
#endif
#ifndef LONG_BIT
#define LONG_BIT (unsigned int) (SIZEOF_UNSIGNED_LONG * CHAR_BIT)
#endif
#ifndef LONG_LONG_BIT
#define LONG_LONG_BIT (unsigned int) (SIZEOF_UNSIGNED_LONG_LONG * CHAR_BIT)
#endif
#define BITMASK(b) (1UL << ((b) % LONG_BIT))
#define BITSLOT(b) ((b) / LONG_BIT)
#define BITGET(a, b) (((a)->field[BITSLOT(b)] >> ((b) % LONG_BIT))  & 1UL)
#define BITSET(a, b) ((a)->field[BITSLOT(b)] |= BITMASK(b))
#define BITCLEAR(a, b) ((a)->field[BITSLOT(b)] &= ~BITMASK(b))
#define BITTEST(a, b) ((a)->field[BITSLOT(b)] & BITMASK(b))
#define BITTOGGLE(a, b) ((a)->field[BITSLOT(b)] ^= BITMASK(b))
#define BITNSLOTS(nb) (((nb) + LONG_BIT - 1) / LONG_BIT)

struct bitfield {		/* defines a bitfield */
	unsigned long *field;
	unsigned int size;
};
