/**
 * File name: tests/test49.c
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

/* Testing bfclo() */

int main()
{
	int i;			//counter
	unsigned int len;
	char *msg = "Testing bfclo()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	unsigned int dots = 80 - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input;
	unsigned int clo;

	/* testing for null input */
	input = NULL;
	clo = bfclo(input);
	if (clo != 0) {
		retval = 1;
		goto ret;
	}
	/* testing for different sizes of bitfield */
	for (len = 1; len < 2 * STORAGE_UNIT_SIZE; len++) {
		input = bfnew(len);
		/* testing bfclo for no set bits */
		clo = bfclo(input);
		if (clo != 0) {
			retval = 1;
			goto ret;
		}
		/* testing bfclo for a set bit in all possible places */
		for (i = len - 1; i >= 0; i--) {
			BITSET(input, i);
			clo = bfclo(input);
			if (len - i != clo) {
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
