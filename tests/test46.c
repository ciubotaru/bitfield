/**
 * File name: tests/test46.c
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

/* Testing bffz() */

int main()
{
	unsigned int i;		//counter
	unsigned int len;
	char *msg = "Testing bffz()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	unsigned int dots = 80 - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input;
	unsigned int ffz;

	/* testing for null input */
	input = NULL;
	ffz = bffz(input);
	if (ffz != 0) {
		retval = 1;
		goto ret;
	}

	/* testing for different lengths of bitfield */
	for (len = 1; len < 2 * STORAGE_UNIT_SIZE; len++) {
		input = bfnew_ones(len);
		/* testing bffz for no clear bits */
		ffz = bffz(input);
		if (ffz != 0) {
			retval = 1;
			goto ret;
		}
		/* testing bffz for a clear set in all possible places */
		for (i = 0; i < len; i++) {
			BITCLEAR(input, i);
			ffz = bffz(input);
			if (i + 1 != ffz) {
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
