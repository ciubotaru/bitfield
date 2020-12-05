/**
 * File name: tests/test11.c
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

/* Testing bftogglebit() */

int main()
{
	srand((unsigned)time(NULL));
	int i;			//counter
	int len = 80;
	char *msg = "Testing bftogglebit()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");

	struct bitfield *input = bfnew_quick(len);
	for (i = 0; i < len; i++) {
		if (rand() % 2 == 1)
			bfsetbit(input, i);
		else
			bfclearbit(input, i);
	}

	struct bitfield *output = bfclone(input);
	for (i = 0; i < len; i++)
		bftogglebit(output, i);
	struct bitfield *not = bfnot(output);
	int cmp = bfcmp(input, not, NULL);
	if (cmp != 0)
		retval = 1;

	bfdel(input);
	bfdel(output);
	bfdel(not);
	printf("%s\n", status[retval]);
	return retval;
}
