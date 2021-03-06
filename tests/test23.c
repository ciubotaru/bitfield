/**
 * File name: tests/test23.c
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

/* Testing bf2int_ip() and int2bf_ip() */

int main()
{
	srand((unsigned)time(NULL));
	int i, cmp;		//counter
	int len = 80;
	char *msg = "Testing bf2int_ip() and int2bf_ip()";
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
	int bitnslots = (len - 1) / INT_BIT + 1;
	unsigned int *input_int = calloc(bitnslots, sizeof(unsigned int));
	bf2int_ip(input, input_int);
	/* check first function */
	int min_memory_length =
	    (bitnslots * sizeof(unsigned int) <
	     BITNSLOTS(len) * sizeof(unsigned long)) ? (bitnslots *
							sizeof(unsigned int)) :
	    BITNSLOTS(len) * sizeof(unsigned long);
	if (memcmp(input_int, input->field, min_memory_length) != 0) {
		printf("%s\n", failed);
		return 1;
	}
	struct bitfield *output = bfnew(len);
	int2bf_ip(input_int, output);
	free(input_int);
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
