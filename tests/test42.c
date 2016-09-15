/**
 * File name: tests/test42.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: September 10, 2016
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"

/* Testing ll2bf() */

int main()
{
	srand((unsigned)time(NULL));
	int i, j;		//counters
	int len = 80;
	char *msg = "Testing ll2bf()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	int bitnslots = (len - 1) / LONG_LONG_BIT + 1;
	unsigned long long *input = calloc(1, bitnslots * sizeof(unsigned long long));
	for (i = 0; i < bitnslots - 1; i++) {
		for (j = 0; j < LONG_LONG_BIT; j++) {
			if (rand() % 2)
				input[i] |= (1ULL << j);
		}
	}
	for (i = 0; i < len % LONG_LONG_BIT; i++)
		if (rand() % 2)
			input[bitnslots - 1] |= (1ULL << i);
	struct bitfield *output = ll2bf(input, len);
	int min_memory_length = (bitnslots * sizeof(unsigned long long) - BITNSLOTS(len) * sizeof(unsigned long) < 0) ? (bitnslots * sizeof(unsigned long long)) : BITNSLOTS(len) * sizeof(unsigned long);
	if (memcmp(input, output->field, min_memory_length) != 0) {
		printf("%s\n", failed);
		return 1;
	}
	printf("%s\n", passed);
	return 0;
}
