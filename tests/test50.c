/**
 * File name: tests/test50.c
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

/* Testing bfclz() */

int main()
{
	int i;			//counter
	unsigned int len;
	char *msg = "Testing bfclz()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	unsigned int dots = 80 - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input;
	unsigned int clz;

	/* testing for null input */
	input = NULL;
	clz = bfclz(input);
	if (clz != 0) {
		retval = 1;
		goto ret;
	}
	/* tedting for different sizes of bitfield */
	for (len = 1; len < 2 * STORAGE_UNIT_SIZE; len++) {
		input = bfnew_ones(len);
		/* testing bfclz for no clear bits */
		clz = bfclz(input);
		if (clz != 0) {
			retval = 1;
			goto ret;
		}
		/* testing bfclz for a clear set in all possible places */
		for (i = len - 1; i >= 0; i--) {
			BITCLEAR(input, i);
			clz = bfclz(input);
			if (len - i != clz) {
				retval = 1;
				goto ret;
			}
		}
		bfdel(input);
		input = NULL;
	}
 ret:
	bfdel(input);
	printf("%s\n", status[retval]);
	return retval;
}
