/**
 * File name: examples/int2bf.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Copyright 2016
**/

/* Convert a random non-negative integer number to a bit array */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bitfield.h>
#include <limits.h>
#ifdef RAND_MAX
#undef RAND_MAX
#define RAND_MAX UINT_MAX
#endif

int main()
{
	srand(time(NULL));
	int bitfield_size = sizeof(unsigned int) * CHAR_BIT; /* equal to the number of bits in an unsigned int */
	int i;
	unsigned int input;
	struct bitfield *output = bfnew(bitfield_size);
	for (i = 0; i < 50; i++) {
		input = rand();
		/**
		 * create an array long enough to hold all bits from an unsigned int
		 * because we have one int, not an array of ints, we pass it by pointer
		**/
		output = int2bf(&input, bitfield_size);
		printf("%u -> %s\n", input, bf2str(output));
	}
}
