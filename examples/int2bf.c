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
//#include <assert.h>
#include <bitfield.h>
#include <bitfield-internals.h>

int main()
{
	srand(time(NULL));
	int bitfield_size = INT_BIT;	/* equal to the number of bits in an unsigned int */
	int i;
	unsigned int input = 0;
	for (i = 0; i < bitfield_size; i++) {	/* randomly set bits in input */
		if (rand() % 2)
			input |= (1U << i);
	}
	/**
	 * create an array long enough to hold all bits from an unsigned int
	 * because we have one int, not an array of ints, we pass it by pointer
	**/
	struct bitfield *output = int2bf(&input, bitfield_size);
	printf("%u -> %s\n", input, bf2str(output));
}
