/**
 * File name: tests/test3.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie@ciubotaru.tokyo>
 * License: General Public License, version 3 or later
 * Copyright 2015, 2016, 2017, 2020
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"

/* Testing bfand(), bfnot(), bfor() and bfxor() */

int main()
{
	srand((unsigned)time(NULL));
	int i, cmp;		//counter
	int len = 80;
	char *msg = "Testing bfand(), bfnot(), bfor() and bfxor()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input1 = bfnew(len);
	struct bitfield *input2 = bfnew(len);
	for (i = 0; i < len; i++) {
		if (rand() % 2)
			BITSET(input1, i);
		if (rand() % 2)
			BITSET(input2, i);
	}

	struct bitfield *output1 = bfxor(input1, input2);
	struct bitfield *and = bfand(input1, input2);
	struct bitfield *or = bfor(input1, input2);
	bfdel(input1);
	bfdel(input2);
	struct bitfield *not = bfnot(and);
	bfdel(and);
	struct bitfield *output2 = bfand(or, not);
	bfdel(or);
	bfdel(not);
	cmp = bfcmp(output1, output2, NULL);
	bfdel(output1);
	bfdel(output2);
	if (cmp != 0) {
		retval = 1;
	}
	printf("%s\n", status[retval]);
	return retval;
}
