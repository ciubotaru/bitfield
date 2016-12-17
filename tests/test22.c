/**
 * File name: tests/test22.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: April 1, 2016
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"

/* Testing bf2long_ip() and long2bf_ip() */

int main()
{
	srand((unsigned)time(NULL));
	int i, cmp;			//counter
	int len = 80;
	char *msg = "Testing bf2long_ip() and long2bf_ip()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input = bfnew(len);
	for (i = 0; i < len; i++)
		if (rand() % 2)
			BITSET(input, i);
	int bitnslots = BITNSLOTS(len);
	unsigned long *input_long = calloc(bitnslots, sizeof(unsigned long));
	bf2long_ip(input, input_long);
	/* check first function */
	for (i = 0; i < bitnslots; i++) {
		if (input_long[i] != input->field[i]) {
			printf("%s\n", failed);
			return 1;
		}
	}
	struct bitfield *output = bfnew(len);
	long2bf_ip(input_long, output);
	free(input_long);
	/* check second function */
	cmp = bfcmp(input, output, NULL);
	bfdel(input);
	bfdel(output);
	if (cmp != 0) {
		printf("%s\n", failed);
		return 1;
	}
	printf("%s\n", passed);
	return 0;
}
