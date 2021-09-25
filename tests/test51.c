/**
 * File name: tests/test51.c
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

/* Testing bfcto() */

int main()
{
	int i;			//counter
	unsigned int len;
	char *msg = "Testing bfcto()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	unsigned int dots = 80 - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input;
	unsigned int cto;

	/* testing for null input */
	input = NULL;
	cto = bfcto(input);
	if (cto != 0) {
		retval = 1;
		goto ret;
	}

	/* testing for different sizes of bitfield */
	for (len = 1; len < 2 * STORAGE_UNIT_SIZE; len++) {
		input = bfnew(len);
		/* testing bfcto for no set bits */
		cto = bfcto(input);
		if (cto != 0) {
			retval = 1;
			goto ret;
		}
		/* testing bfcto for a set bit in all possible places */
		for (i = 0; i < len; i++) {
			BITSET(input, i);
			cto = bfcto(input);
			if (i + 1 != cto) {
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
