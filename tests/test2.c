/**
 * File name: tests/test2.c
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

/* Testing bf2str() and str2bf() */

int main()
{
	srand((unsigned)time(NULL));
	int i;			//counter
	int len = 80;
	char *errmsg;
	char *msg = "Testing bf2str() and str2bf()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input = bfnew(len);
	struct bitfield *output = bfnew(len);
	char *input_char = malloc((len + 1) * sizeof(char));
	for (i = 0; i < len; i++)
		if (rand() % 2)
			BITSET(input, i);
//    bfprint(input);
	bf2str(input, input_char);
//    printf("%s\n", input_char);
	str2bf(input_char, output);
//    bfprint(output);
	if (bfcmp(input, output, &errmsg) != 0) {
		printf("%s\n", failed);
		return 1;
	}
	printf("%s\n", passed);
	return 0;
}
