/**
 * File name: tests/test4.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: September 20, 2015
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitfield.h"
#include "bitfield-internals.h"

/* Testing bfnew_ones(), bfsetall() and bfclearall() */

int main()
{
	int i;			//counter
	int len = 80;
	char *msg = "Testing bfnew_ones(), bfsetall() and bfclearall()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	char *input_char = "11111111111111111111111111111111111111111111111111111111111111111111111111111111";	/* 80 units */
	struct bitfield *output1 = str2bf(input_char);

	struct bitfield *output2 = bfnew_ones(len);
	if (bfcmp(output1, output2, NULL) != 0) {
		printf("%s\n", failed);
		return 1;
	}

	struct bitfield *output3 = bfnew_quick(len);
	bfsetall(output3);
	if (bfcmp(output1, output3, NULL) != 0) {
		printf("%s\n", failed);
		return 1;
	}

	struct bitfield *output4 = bfnew(len);
	bfclearall(output3);
	if (bfcmp(output3, output4, NULL) != 0) {
		printf("%s\n", failed);
		return 1;
	}

	printf("%s\n", passed);
	return 0;
}
