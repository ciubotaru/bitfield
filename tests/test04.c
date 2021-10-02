/**
 * File name: tests/test04.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie@ciubotaru.tokyo>
 * License: General Public License, version 3 or later
 * Copyright 2015, 2016, 2017, 2020
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitfield.h"
#include "bitfield-internals.h"

/* Testing bfnew_ones(), bfsetall() and bfclearall() */

int main()
{
	int i, cmp;		//counter
	int len = 80;
	char *msg = "Testing bfnew_ones(), bfsetall() and bfclearall()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	char *input_char = "11111111111111111111111111111111111111111111111111111111111111111111111111111111";	/* 80 units */
	struct bitfield *output1 = str2bf(input_char);

	struct bitfield *output2 = bfnew_ones(len);
	cmp = bfcmp(output1, output2, NULL);
	if (cmp != 0) {
		retval = 1;
		goto ret;
	}

	struct bitfield *output3 = bfnew_quick(len);
	bfsetall(output3);
	cmp = bfcmp(output1, output3, NULL);
	if (cmp != 0) {
		retval = 1;
		goto ret;
	}

	struct bitfield *output4 = bfnew(len);
	bfclearall(output3);
	cmp = bfcmp(output3, output4, NULL);
	if (cmp != 0) {
		retval = 1;
		goto ret;
	}
 ret:
	bfdel(output1);
	bfdel(output2);
	bfdel(output3);
	bfdel(output4);
	printf("%s\n", status[retval]);
	return retval;
}