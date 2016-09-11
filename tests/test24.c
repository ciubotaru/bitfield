/**
 * File name: tests/test24.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: July 10, 2016
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"

/* Testing char2bf() */

int main()
{
	srand((unsigned)time(NULL));
	int i, j;		//counters
	int len = 80;
	char *msg = "Testing char2bf()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	int bitnslots = (len - 1) / CHAR_BIT + 1;
	unsigned char *input = calloc(1, bitnslots * sizeof(unsigned char));
	for (i = 0; i < bitnslots - 1; i++) {
		for (j = 0; j < CHAR_BIT; j++) {
			if (rand() % 2)
				input[i] |= (1U << j);
		}
	}
	for (i = 0; i < len % CHAR_BIT; i++)
		if (rand() % 2)
			input[bitnslots - 1] |= (1U << i);
	struct bitfield *output = char2bf(input, len);
	int min_memory_length = (bitnslots * sizeof(unsigned char) - BITNSLOTS(len) * sizeof(unsigned long) < 0) ? (bitnslots * sizeof(unsigned char)) : BITNSLOTS(len) * sizeof(unsigned long);
	if (memcmp(input, output->field, min_memory_length) != 0) {
		printf("%s\n", failed);
		return 1;
	}
	printf("%s\n", passed);
	return 0;
}
