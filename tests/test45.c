/**
 * File name: tests/test45.c
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

/* Testing bffs() */

int main()
{
	unsigned int i;		//counter
	unsigned int len;
	char *msg = "Testing bffs()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	unsigned int dots = 80 - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input;
	unsigned int ffs;

	/* testing for null input */
	input = NULL;
	ffs = bffs(input);
	if (ffs != 0) {
		retval = 1;
		goto ret;
	}

	/* testing for different lengths of bitfield */
	for (len = 1; len <= 2 * STORAGE_UNIT_SIZE; len++) {
		input = bfnew(len);
		/* testing bffs for no set bits */
		ffs = bffs(input);
		if (ffs != 0) {
			retval = 1;
			goto ret;
		}
		/* testing bffs for a bit set in all possible places */
		for (i = 0; i < len; i++) {
			BITSET(input, i);
			ffs = bffs(input);
			if (i + 1 != ffs) {
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
