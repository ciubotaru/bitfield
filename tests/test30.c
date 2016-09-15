/**
 * File name: tests/test30.c
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

/* Testing uint8tobf() */

int main()
{
	srand((unsigned)time(NULL));
	int i, j;		//counters
	int len = 80;
	char *msg = "Testing uint8tobf()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	int bitnslots = (len - 1) / 8 + 1;
	uint8_t *input = calloc(1, bitnslots * sizeof(uint8_t));
	for (i = 0; i < bitnslots - 1; i++) {
		for (j = 0; j < 8; j++) {
			if (rand() % 2)
				input[i] |= (1U << j);
		}
	}
	for (i = 0; i < len % 8; i++)
		if (rand() % 2)
			input[bitnslots - 1] |= (1U << i);
	struct bitfield *output = uint8tobf(input, len);
	int min_memory_length = (bitnslots * sizeof(uint8_t) - BITNSLOTS(len) * sizeof(unsigned long) < 0) ? (bitnslots * sizeof(uint8_t)) : BITNSLOTS(len) * sizeof(unsigned long);
	if (memcmp(input, output->field, min_memory_length) != 0) {
		printf("%s\n", failed);
		return 1;
	}
	printf("%s\n", passed);
	return 0;
}
