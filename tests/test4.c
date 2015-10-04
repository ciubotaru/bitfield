/**
 * File name: tests/test4.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3
 * Date: September 20, 2015
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"

/* Testing bfnew_ones() and bfone() */

int main()
{
	srand((unsigned)time(NULL));
	int i;			//counter
	int len = 80;
	char *errmsg;
	char *msg = "Testing bfnew_ones() and bfone()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	char *input_char = "11111111111111111111111111111111111111111111111111111111111111111111111111111111";	/* 80 units */
	struct bitfield *output1 = bfnew_quick(len);
	char2bf(input_char, output1);
//      bfprint(output1);
	struct bitfield *output2 = bfnew_ones(len);
//      bfprint(output2);
	struct bitfield *output3 = bfnew_quick(len);
	bfone(output3);
//      bfprint(output3);
	if (bfcmp(output1, output2, &errmsg) != 0) {
		printf("%s\n", failed);
		return 1;
	} else if (bfcmp(output1, output3, &errmsg) != 0) {
		printf("%s\n", failed);
		return 1;
	}
	printf("%s\n", passed);
	return 0;
}