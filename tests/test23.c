/**
 * File name: tests/test23.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie@ciubotaru.tokyo>
 * License: General Public License, version 3 or later
 * Copyright 2016, 2017, 2020
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
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
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
		retval = 1;
		goto ret;
	}
	struct bitfield *output = bfnew(len);
	int2bf_ip(input_int, output);
	free(input_int);
	/* check second function */
	cmp = bfcmp(input, output, NULL);
	bfdel(input);
	bfdel(output);
	if (cmp != 0)
		retval = 1;
 ret:
	printf("%s\n", status[retval]);
	return retval;
}
