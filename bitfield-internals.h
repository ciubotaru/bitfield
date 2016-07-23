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
#define BITMASK(b) (1UL << ((b) % LONG_BIT))
#define BITSLOT(b) ((b) / LONG_BIT)
#define BITGET(a, b) (((a)->field[BITSLOT(b)] >> ((b) % LONG_BIT))  & 1UL)
#define BITSET(a, b) ((a)->field[BITSLOT(b)] |= BITMASK(b))
#define BITCLEAR(a, b) ((a)->field[BITSLOT(b)] &= ~BITMASK(b))
#define BITTEST(a, b) ((a)->field[BITSLOT(b)] & BITMASK(b))
#define BITTOGGLE(a, b) ((a)->field[BITSLOT(b)] ^= BITMASK(b))
#define BITNSLOTS(nb) ((nb + LONG_BIT - 1) / LONG_BIT)

#if __BYTE_ORDER == __BIG_ENDIAN
/* big-endian systems */
#define bf_letoh_ip(x) _bf_letoh_ip(x)
#define int_letoh_ip(x, y) _int_letoh_ip(x, y)
#define bf_htole(x) (struct bitfield *) _bf_htole(x)
#define int_htole(x, y) (unsigned int *) _int_htole(x, y)
#else
/* little-endian systems (mixed endians?) */
#define bf_letoh_ip(x)
#define int_letoh_ip(x, y)
#define bf_htole(x) x
#define int_htole(x, y) x
#endif

struct bitfield {		/* defines a bitfield */
	unsigned long *field;
	int size;
};

struct bitfield *__bfcat(const struct bitfield *input1,
		       const struct bitfield *input2);
