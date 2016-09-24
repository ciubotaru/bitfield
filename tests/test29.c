/**
 * File name: tests/test29.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: September 1, 2016
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"

/* Testing bf2short_ip() and short2bf_ip() */

int main()
{
	srand((unsigned)time(NULL));
	int i;			//counter
	int len = 80;
	char *msg = "Testing bf2short_ip() and short2bf_ip()";
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
	int bitnslots = (len - 1) / SHORT_BIT + 1;
	unsigned short *input_short = malloc(bitnslots * sizeof(unsigned short));
	bf2short_ip(input, input_short);
	/* check first function */
	int min_memory_length = (bitnslots * sizeof(unsigned short) < BITNSLOTS(len) * sizeof(unsigned long)) ? (bitnslots * sizeof(unsigned short)) : BITNSLOTS(len) * sizeof(unsigned long);
	if (memcmp(input_short, input->field, min_memory_length) != 0) {
		printf("%s\n", failed);
		return 1;
	}
	struct bitfield *output = bfnew(len);
	short2bf_ip(input_short, output);
	/* check second function */
	if (bfcmp(input, output, NULL) != 0)
		{
			printf("%s\n", failed);
			return 1;
		}
	printf("%s\n", passed);
	return 0;
}
