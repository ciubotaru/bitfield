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
#include <time.h>
#ifdef RAND_MAX
#undef RAND_MAX
#define RAND_MAX UINT_MAX
#endif

int main()
{
	printf("Representing a random array of bits as the least binary number...\n");
	srand(time(NULL));
	int len = 80;		/* terminal width */
	int len_chars = (len - 2) / 2;	/* fit 2 strings and a '->' in one line */
	int int_size = sizeof(unsigned int) * CHAR_BIT;	/* equal to the number of bits in an unsigned int */
	int len_ints = (len_chars - 1) / int_size + 1;	/* nr of ints to hold one len_str */
	int i, j;
	unsigned int *input_ints = malloc(sizeof(unsigned int) * len_ints);
	struct bitfield *input, *output;
	for (i = 0; i < 50; i++) {
		for (j = 0; j < len_ints; j++)
			input_ints[j] = rand();
		input = int2bf(input_ints, len_chars);
		output = bfnormalize(input);
		bfprint(input);
		printf("->");
		bfprint(output);
		printf("\n");
	}
	bfdel(input);
	bfdel(output);
	printf("---\n");
	return 0;
}
