/**
 * File name: tests/test48.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie@ciubotaru.tokyo>
 * License: General Public License, version 3 or later
 * Copyright 2020, 2021
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitfield.h"
#include "bitfield-internals.h"
#include <endian.h>

/* Testing bflz() */

int main()
{
	unsigned int i;		//counter
	unsigned int len;
	char *msg = "Testing bflz()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	unsigned int dots = 80 - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input;
	unsigned int flz;

	/* testing for null input */
	input = NULL;
	flz = bflz(input);
	if (flz != 0) {
		retval = 1;
		goto ret;
	}

	/* testing for different size of bitfield */
	for (len = 1; len < 2 * STORAGE_UNIT_SIZE; len++) {
		input = bfnew_ones(len);
		/* testing bflz for no clear bits */
		flz = bflz(input);
		if (flz != 0) {
			retval = 1;
			goto ret;
		}
		/* testing bfls for a clear in all possible places */
		for (i = 0; i < len; i++) {
			BITCLEAR(input, i);
			flz = bflz(input);
			if (i + 1 != flz) {
				retval = 1;
				goto ret;
			}
			BITSET(input, i);
		}
		bfdel(input);
		input = NULL;
	}
 ret:
	bfdel(input);
	printf("%s\n", status[retval]);
	return retval;
}
