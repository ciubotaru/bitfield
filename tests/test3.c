/**
 * File name: tests/test3.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: September 1, 2015
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
	int i;			//counter
	int len = 80;
	char *msg = "Testing bfand(), bfnot(), bfor() and bfxor()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input1 = bfnew(len);
	struct bitfield *input2 = bfnew(len);
	struct bitfield *output1 = bfnew(len);
	struct bitfield *output2 = bfnew(len);
	for (i = 0; i < len; i++) {
		if (rand() % 2)
			BITSET(input1, i);
		if (rand() % 2)
			BITSET(input2, i);
	}

	output1 = bfxor(input1, input2);
	output2 = bfand(bfor(input1, input2), bfnot(bfand(input1, input2)));
	if (bfcmp(output1, output2, NULL) != 0) {
		printf("%s\n", failed);
		return 1;
	}
	printf("%s\n", passed);
	return 0;
}
