/**
 * File name: tests/test18.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: March 20, 2016
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"

/* Testing long2bf() */

int main()
{
	srand((unsigned)time(NULL));
	unsigned int i, j;		//counters
	unsigned int len = 80;
	char *msg = "Testing long2bf()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	unsigned int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	unsigned int bitnslots = BITNSLOTS(len);
	unsigned long *input = calloc(1, bitnslots * sizeof(unsigned long));
	for (i = 0; i < bitnslots - 1; i++) {
		for (j = 0; j < LONG_BIT; j++) {
			if (rand() % 2)
				input[i] |= (1UL << j);
		}
	}
	for (i = 0; i < len % LONG_BIT; i++)
		if (rand() % 2)
			input[bitnslots - 1] |= (1UL << i);
	struct bitfield *output = long2bf(input, len);
	for (i = 0; i < bitnslots; i++) {
		if (output->field[i] != input[i]) {
			printf("%s\n", failed);
			free(input);
			bfdel(output);
			return 1;
		}
	}
	printf("%s\n", passed);
	free(input);
	bfdel(output);
	return 0;
}
