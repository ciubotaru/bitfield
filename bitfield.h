/**
 * File name: bitfield.h
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3
 * Date: September 1, 2015
**/

#ifndef CHAR_BIT
#include <limits.h>
#endif
#ifndef LONG_BIT
#define LONG_BIT (unsigned int) (sizeof(unsigned long) * CHAR_BIT)
#endif
#define BITMASK(b) (1UL << ((b) % LONG_BIT))
#define BITSLOT(b) ((b) / LONG_BIT)
#define BITGET(a, b) (((a)->field[BITSLOT(b)] >> ((b) % LONG_BIT))  & 1UL)
#define BITSET(a, b) ((a)->field[BITSLOT(b)] |= BITMASK(b))
#define BITCLEAR(a, b) ((a)->field[BITSLOT(b)] &= ~BITMASK(b))
#define BITTEST(a, b) ((a)->field[BITSLOT(b)] & BITMASK(b))
#define BITTOGGLE(a, b) ((a)->field[BITSLOT(b)] ^= BITMASK(b))
#define BITNSLOTS(nb) ((nb + LONG_BIT - 1) / LONG_BIT)

struct bitfield {		/* defines a bitfield */
	unsigned long *field;
	int size;
};

struct bitfield *bfnew(const int);	/* creates a bitfield structure, sets all its bits to false and returns a pointer to it */

struct bitfield *bfnew_quick(const int);	/* creates a bitfield structure and returns a pointer to it */

struct bitfield *bfnew_ones(const int);	/* creates a bitfield structure, sets all its bits to true with and returns a pointer to it */

struct bitfield *bfclone(struct bitfield *);	/* creates a copy of an existing bitfield */

void bfdel(struct bitfield *);	/* destroys a bitfield structure and frees memory */

int bfcmp(const struct bitfield *, const struct bitfield *, char **);	/* compares two bitfields and returns 0 if same or non-zero and error message if different */

void bfprint(const struct bitfield *);	/* prints a bitfield as a series of ones and zeroes */

void char2bf(const char *, struct bitfield *);	/* converts a character string of ones and zeroes into a bitfield */

void bf2char(const struct bitfield *, char *);	/* converts a bitfield into a character string of ones and zeroes */

struct bitfield *bfsub(const struct bitfield *, const unsigned int, const unsigned int);	/* extracts a sub-bitfield from a bitfield */

struct bitfield *bfcat(const struct bitfield *, const struct bitfield *);	/* concatenates two bitfields into one */

struct bitfield *bfshift(const struct bitfield *, const int);	/* circular-shifts the contents of a bitfield and return the result in new bitfield */

void bfshift_ip(struct bitfield *, const int);	/* circular-shifts the contents of a bitfield */

struct bitfield *bfor(const struct bitfield *, const struct bitfield *);	/* performs bitwise inclusive OR over a pair of bitfields */

struct bitfield *bfand(const struct bitfield *, const struct bitfield *);	/* performs bitwise AND over a pair of bitfields */

struct bitfield *bfxor(const struct bitfield *, const struct bitfield *);	/* performs bitwise exclusive OR over a pair of bitfields */

struct bitfield *bfnot(const struct bitfield *);	/* reverses all bits in a bitfield */

inline void bfcleartail(struct bitfield *);	/* set unused bits to zero */
