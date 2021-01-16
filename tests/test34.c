/**
 * File name: tests/test34.c
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

/* Testing uint32tobf() */

int main()
{
	uint32_t unit = 1;
	srand((unsigned)time(NULL));
	unsigned int i, j, cmp;	//counters
	unsigned int len = 80;
	char *msg = "Testing uint32tobf()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	unsigned int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	unsigned int int32s = (len - 1) / 32 + 1;
	uint32_t *input = calloc(1, int32s * sizeof(uint32_t));
	for (i = 0; i < int32s - 1; i++) {
		for (j = 0; j < 32; j++) {
			if (rand() % 2)
				input[i] |= (unit << j);
		}
	}
	for (i = 0; i < len % 32; i++)
		if (rand() % 2)
			input[int32s - 1] |= (unit << i);
	struct bitfield *output = uint32tobf(input, len);
	for (i = 0; i < int32s; i++)
		input[i] = htole32(input[i]);
	for (i = 0; i < BITNSLOTS(len); i++) {
#if SIZEOF_UNSIGNED_LONG == 1
#elif SIZEOF_UNSIGNED_LONG == 2
		output->field[i] = htole16(output->field[i]);
#elif SIZEOF_UNSIGNED_LONG == 4
		output->field[i] = htole32(output->field[i]);
#elif SIZEOF_UNSIGNED_LONG == 8
		output->field[i] = htole64(output->field[i]);
#else
#error Not implemented
#endif
	}
	cmp = memcmp(input, output->field, int32s * sizeof(uint32_t));
	if (cmp != 0)
		retval = 1;
	free(input);
	bfdel(output);
	printf("%s\n", status[retval]);
	return retval;
}
