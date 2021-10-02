/**
 * File name: bitfield-internals.h
 * Project name: bitfield, a bit array manipulation library written in C                                               * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie@ciubotaru.tokyo>
 * License: General Public License, version 3 or later
 * Copyright 2015-2017, 2019-2021
**/

#include "config.h"
#ifndef CHAR_BIT
#include <limits.h>
#endif

#if (defined(__x86_64__) || defined(__aarch64__) || defined(__PPC64__))
#define STORAGE_UNIT_SIZE 64
#elif (defined(__i386) || defined(__PPC__) || defined(__arm__))
#define STORAGE_UNIT_SIZE 32
#else
#error Unknown architecture
#endif

#if STORAGE_UNIT_SIZE == 64
typedef uint64_t storage_unit;
#define STORAGE_UNIT_PREFIX(x) INT64_C(x)
#elif STORAGE_UNIT_SIZE == 32
typedef uint32_t storage_unit;
#define STORAGE_UNIT_PREFIX(x) INT32_C(x)
#elif STORAGE_UNIT_SIZE == 16
typedef uint16_t storage_unit;
#define STORAGE_UNIT_PREFIX(x) INT16_C(x)
#elif STORAGE_UNIT_SIZE == 8
typedef uint8_t storage_unit;
#define STORAGE_UNIT_PREFIX(x) INT8_C(x)
#endif

#define BITMASK(b) (STORAGE_UNIT_PREFIX(1) << ((b) % STORAGE_UNIT_SIZE))
#define BITSLOT(b) ((b) / STORAGE_UNIT_SIZE)
#define BITGET(a, b) (((a)->field[BITSLOT(b)] >> ((b) % STORAGE_UNIT_SIZE))  & STORAGE_UNIT_PREFIX(1))
#define BITSET(a, b) ((a)->field[BITSLOT(b)] |= BITMASK(b))
#define BITCLEAR(a, b) ((a)->field[BITSLOT(b)] &= ~BITMASK(b))
#define BITTEST(a, b) ((a)->field[BITSLOT(b)] & BITMASK(b))
#define BITTOGGLE(a, b) ((a)->field[BITSLOT(b)] ^= BITMASK(b))
#define BITNSLOTS(nb) (((nb) + STORAGE_UNIT_SIZE - 1) / STORAGE_UNIT_SIZE)
#define BFSIZE(a) ((a)->size)

struct bitfield {		/* defines a bitfield */
	storage_unit *field;
	unsigned int size;
};
