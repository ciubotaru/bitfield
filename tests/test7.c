/**
 * File name: tests/test7.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: October 1, 2015
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"

/* Testing bfclone() and bfcpy() */

int main()
{
	srand((unsigned)time(NULL));
	int i;			//counter
	int len = 80;
	char *msg = "Testing bfclone() and bfcpy()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	char *input_char = malloc((len + 1) * sizeof(char));	/* allocate space for 80+1 chars */
	for (i = 0; i < len; i++) {
		if (rand() % 2)
			input_char[i] = '1';
		else
			input_char[i] = '0';
	}
	input_char[len] = '\0';
	struct bitfield *input = bfnew_quick(len);
	str2bf(input_char, input);

	struct bitfield *input2 = bfclone(input);
//      bfprint(input);
//      bfprint(input2);

	char *errmsg;
	if (bfcmp(input, input2, &errmsg) != 0) {
		printf("zyu1");
		printf("%s\n", failed);
		return 1;
	}

	struct bitfield *input3 = bfnew_quick(len);
	int result = bfcpy(input, input3);
	if (result != 0) {
		printf("zyu2");
		printf("%s\n", failed);
		return 1;
	}

	if (bfcmp(input, input3, &errmsg) != 0) {
		printf("zyu3");
		printf("%s\n", failed);
		return 1;
	}

	printf("%s\n", passed);
	return 0;
}
