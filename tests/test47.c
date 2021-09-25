/**
 * File name: tests/test47.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie@ciubotaru.tokyo>
 * License: General Public License, version 3 or later
 * Copyright 2020
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitfield.h"
#include "bitfield-internals.h"
#include <endian.h>

/* Testing bfls() */

int main()
{
	unsigned int i;		//counter
	unsigned int len;
	char *msg = "Testing bfls()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	unsigned int dots = 80 - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input;
	unsigned int fls;

	/* testing for null input */
	input = NULL;
	fls = bfls(input);
	if (fls != 0) {
		retval = 0;
		goto ret;
	}

	/* testing for different sizes of bitfield */
	for (len = 1; len < 2 * STORAGE_UNIT_SIZE; len++) {
		input = bfnew(len);
		/* testing bflz for no set bits */
		fls = bfls(input);
		if (fls != 0) {
			retval = 1;
			goto ret;
		}
		/* testing bfls for a set in all possible places */
		for (i = 0; i < len; i++) {
			BITSET(input, i);
			fls = bfls(input);
			if (i + 1 != fls) {
				retval = 1;
				goto ret;
			}
			BITCLEAR(input, i);
		}
		bfdel(input);
		input = NULL;
	}
 ret:
	bfdel(input);
	printf("%s\n", status[retval]);
	return retval;
}
