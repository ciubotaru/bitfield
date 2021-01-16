/**
 * File name: tests/test38.c
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
#include <endian.h>

/* Testing bftouint8_ip() and uint8tobf_ip() */

int main()
{
	srand((unsigned)time(NULL));
	unsigned int i, cmp;	//counter
	unsigned int len = 80;
	char *msg = "Testing bftouint8_ip() and uint8tobf_ip()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	unsigned int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input = bfnew(len);
	for (i = 0; i < len; i++)
		if (rand() % 2)
			BITSET(input, i);
	unsigned int int8s = (len - 1) / 8 + 1;
	uint8_t *input_uint8 = malloc(int8s);
	bftouint8_ip(input, input_uint8);
	/* check first function */
	struct bitfield *output = bfnew(len);
	for (i = 0; i < BITNSLOTS(len); i++) {
#if SIZEOF_UNSIGNED_LONG == 1
		output->field[i] = input->field[i];
#elif SIZEOF_UNSIGNED_LONG == 2
		output->field[i] = htole16(input->field[i]);
#elif SIZEOF_UNSIGNED_LONG == 4
		output->field[i] = htole32(input->field[i]);
#elif SIZEOF_UNSIGNED_LONG == 8
		output->field[i] = htole64(input->field[i]);
#else
#error Not implemented
#endif
	}
	cmp = memcmp(input_uint8, output->field, int8s);
	if (cmp != 0) {
		retval = 1;
		goto ret;
	}
	uint8tobf_ip(input_uint8, output);
	/* check second function */
	cmp = bfcmp(input, output, NULL);
	bfdel(output);
	if (cmp != 0)
		retval = 1;
 ret:
	bfdel(input);
	free(input_uint8);
	printf("%s\n", status[retval]);
	return retval;
}
