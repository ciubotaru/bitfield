#ifndef CHAR_BIT
#include <limits.h>
#endif
#ifndef LONG_BIT
#define LONG_BIT (unsigned int) (sizeof(unsigned long) * CHAR_BIT)
#endif
#define BITMASK(b) (1 << ((b) % LONG_BIT))
#define BITSLOT(b) ((b) / LONG_BIT)
#define BITGET(a, b) (((a)->field[BITSLOT(b)] >> ((b) % LONG_BIT))  & 1)
#define BITSET(a, b) ((a)->field[BITSLOT(b)] |= BITMASK(b))
#define BITCLEAR(a, b) ((a)->field[BITSLOT(b)] &= ~BITMASK(b))
#define BITTEST(a, b) ((a)->field[BITSLOT(b)] & BITMASK(b))
#define BITNSLOTS(nb) ((nb + LONG_BIT - 1) / LONG_BIT)

struct bitfield {			/* defines a bitfield */
	unsigned long *field;
	int size;
};

struct bitfield *bfnew(const int);	/* returns a pointer to struct bitfield */

void bfdel(struct bitfield *);	/* destroys a bitfield structure and frees memory */

int bfcmp(const struct bitfield *, const struct bitfield *, char **);	/* compares two bitfields and returns 0 if same or non-zero and error message if different */

void bfprint(const struct bitfield *);	/* prints a bitfield as a series of ones and zeroes */

void char2bf(const char *, struct bitfield *);	/* converts a character string of ones and zeroes into a bitfield */

void bf2char(const struct bitfield *, char *);	/* converts a bitfield into a character string of ones and zeroes */

struct bitfield *bfsub(const struct bitfield *, const unsigned int, const unsigned int);	/* extracts a sub-bitfield from a bitfield */

struct bitfield *bfcat(const struct bitfield *, const struct bitfield *);	/* concatenates two bitfields into one */

struct bitfield *bfshift(const struct bitfield *, const int); /* circular-shift the contents of a bitfield */

struct bitfield *bfor(const struct bitfield *, const struct bitfield *); /* performs bitwise inclusive OR over a pair of bitfields */

struct bitfield *bfand(const struct bitfield *, const struct bitfield *); /* performs bitwise AND over a pair of bitfields */

struct bitfield *bfxor(const struct bitfield *, const struct bitfield *); /* performs bitwise exclusive OR over a pair of bitfields */

struct bitfield *bfnot(const struct bitfield *); /* reverses all bits in a bitfield */
