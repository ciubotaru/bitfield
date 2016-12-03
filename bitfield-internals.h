#ifndef CHAR_BIT
#include <limits.h>
#endif
#ifndef SHORT_BIT
#define SHORT_BIT (unsigned int) (sizeof(unsigned short) * CHAR_BIT)
#endif
#ifndef INT_BIT
#define INT_BIT (unsigned int) (sizeof(unsigned int) * CHAR_BIT)
#endif
#ifndef LONG_BIT
#define LONG_BIT (unsigned int) (sizeof(unsigned long) * CHAR_BIT)
#endif
#ifndef LONG_LONG_BIT
#define LONG_LONG_BIT (unsigned int) (sizeof(unsigned long long) * CHAR_BIT)
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

