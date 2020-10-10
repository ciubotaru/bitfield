/**
 * File name: tests/test14.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: December 1, 2015
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"

/* Testing bf2str() and str2bf() */

int main()
{
	srand((unsigned)time(NULL));
	int i, cmp;		//counter
	int len = 80;
	char *msg = "Testing bf2str() and str2bf()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input = bfnew(len);
	for (i = 0; i < len; i++)
		if (rand() % 2)
			BITSET(input, i);
	char *input_char = bf2str(input);
	struct bitfield *output = str2bf(input_char);
	cmp = bfcmp(input, output, NULL);
	if (cmp != 0)
		retval = 1;

	free(input_char);
	bfdel(input);
	bfdel(output);
	printf("%s\n", status[retval]);
	return retval;
}
