/**
 * File name: tests/test19.c
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

/* Testing int2bf() */

int main()
{
	srand((unsigned)time(NULL));
	unsigned int i, j;	//counters
	unsigned int len = 80;
	char *msg = "Testing int2bf()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	unsigned int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	unsigned int bitnslots = (len - 1) / INT_BIT + 1;
	unsigned int *input = calloc(1, bitnslots * sizeof(unsigned int));
	for (i = 0; i < bitnslots - 1; i++) {
		for (j = 0; j < INT_BIT; j++) {
			if (rand() % 2)
				input[i] |= (1U << j);
		}
	}
	for (i = 0; i < len % INT_BIT; i++)
		if (rand() % 2)
			input[bitnslots - 1] |= (1U << i);
	struct bitfield *output = int2bf(input, len);
	int min_memory_length =
	    (bitnslots * sizeof(unsigned int) <
	     BITNSLOTS(len) * sizeof(unsigned long)) ? (bitnslots *
							sizeof(unsigned int)) :
	    BITNSLOTS(len) * sizeof(unsigned long);
	if (memcmp(input, output->field, min_memory_length) != 0) {
		retval = 1;
		goto ret;
	}
ret:
	free(input);
	bfdel(output);
	printf("%s\n", status[retval]);
	return retval;
}
