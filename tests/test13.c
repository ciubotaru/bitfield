/**
 * File name: tests/test13.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie@ciubotaru.tokyo>
 * License: General Public License, version 3 or later
 * Copyright 2015, 2016, 2020
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"

/* Testing bfhamming() */

int main()
{
	srand((unsigned)time(NULL));
	int i;			//counter
	int len = 80;
	char *msg = "Testing bfhamming()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");

	int count = 0;
	struct bitfield *input1 = bfnew(len);
	struct bitfield *input2 = bfnew(len);
	for (i = 0; i < len; i++) {
		if (rand() % 2)
			BITSET(input1, i);
		if (rand() % 2)
			BITSET(input2, i);
		if (BITGET(input1, i) != BITGET(input2, i))
			count++;
	}
	if (count != bfhamming(input1, input2))
		retval = 1;

	bfdel(input1);
	bfdel(input2);
	printf("%s\n", status[retval]);
	return retval;
}
