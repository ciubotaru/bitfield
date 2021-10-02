/**
 * File name: tests/test30.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie@ciubotaru.tokyo>
 * License: General Public License, version 3 or later
 * Copyright 2016, 2017, 2020, 2021
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"
#include <endian.h>

/* Testing uint8tobf() */

int main()
{
	srand((unsigned)time(NULL));
	unsigned int i, j, cmp;	//counters
	unsigned int len = 80;
	char *msg = "Testing uint8tobf()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	unsigned int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	unsigned int chars = (len - 1) / 8 + 1;
	uint8_t *input = calloc(1, chars);
	for (i = 0; i < chars - 1; i++) {
		for (j = 0; j < 8; j++) {
			if (rand() % 2)
				input[i] |= (1U << j);
		}
	}
	for (i = 0; i < len % 8; i++)
		if (rand() % 2)
			input[chars - 1] |= (1U << i);
	struct bitfield *output = uint8tobf(input, len);
	for (i = 0; i < BITNSLOTS(len); i++) {
#if STORAGE_UNIT_SIZE == 8
#elif STORAGE_UNIT_SIZE == 16
		output->field[i] = htole16(output->field[i]);
#elif STORAGE_UNIT_SIZE == 32
		output->field[i] = htole32(output->field[i]);
#elif STORAGE_UNIT_SIZE == 64
		output->field[i] = htole64(output->field[i]);
#else
#error Not implemented
#endif
	}
	cmp = memcmp(input, output->field, chars);
	if (cmp != 0)
		retval = 1;
	free(input);
	bfdel(output);
	printf("%s\n", status[retval]);
	return retval;
}
