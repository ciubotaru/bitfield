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
	unsigned int len = 400;
	char *msg = "Testing bfcto()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	unsigned int dots = 80 - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input = bfnew(len);
	unsigned int cto;
	unsigned int cto_check;
	/* testing bfcto for no set bits */
	cto_check = bfcto(input);
	if (cto_check != 0) {
		retval = 1;
		goto ret;
	}
	/* testing bfcto for a set bit in all possible places */
	for (i = 0; i < len; i++) {
		BITSET(input, i);
		cto_check = bfcto(input);
		if (i + 1 != cto_check) {
			retval = 1;
			goto ret;
		}
	}
 ret:
	bfdel(input);
	printf("%s\n", status[retval]);
	return retval;
}
