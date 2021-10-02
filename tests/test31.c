/**
 * File name: tests/test31.c
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

/* Testing bftouint8() */

int main()
{
	srand((unsigned)time(NULL));
	unsigned int i, cmp;	//counter
	unsigned int len = 80;
	char *msg = "Testing bftouint8()";
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
	unsigned int chars = (len - 1) / 8 + 1;
	uint8_t *input_int = bftouint8(input);
	for (i = 0; i < BITNSLOTS(len); i++) {
#if STORAGE_UNIT_SIZE == 8
#elif STORAGE_UNIT_SIZE == 16
		input->field[i] = htole16(input->field[i]);
#elif STORAGE_UNIT_SIZE == 32
		input->field[i] = htole32(input->field[i]);
#elif STORAGE_UNIT_SIZE == 64
		input->field[i] = htole64(input->field[i]);
#else
#error Not implemented
#endif
	}
	cmp = memcmp(input_int, input->field, chars);
	if (cmp != 0)
		retval = 1;
	free(input_int);
	bfdel(input);
	printf("%s\n", status[retval]);
	return retval;
}
