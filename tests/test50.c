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
	unsigned int len = 400;
	char *msg = "Testing bfclz()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	unsigned int dots = 80 - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input = bfnew_ones(len);
	unsigned int clz;
	unsigned int clz_check;
	/* testing bfclz for no clear bits */
	clz_check = bfclz(input);
	if (clz_check != 0) {
		retval = 1;
		goto ret;
	}
	/* testing bfclz for a clear set in all possible places */
	for (i = len - 1; i >= 0; i--) {
		BITCLEAR(input, i);
		clz_check = bfclz(input);
		if (len - i != clz_check) {
			retval = 1;
			goto ret;
		}
	}
 ret:
	bfdel(input);
	printf("%s\n", status[retval]);
	return retval;
}
