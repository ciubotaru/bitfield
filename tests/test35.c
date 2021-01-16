/**
 * File name: tests/test35.c
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

/* Testing bftouint32() */

int main()
{
	srand((unsigned)time(NULL));
	unsigned int i, cmp;	//counter
	unsigned int len = 80;
	char *msg = "Testing bftouint32()";
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
	unsigned int int32s = (len - 1) / 32 + 1;
	uint32_t *input_int = bftouint32(input);
	for (i = 0; i < BITNSLOTS(len); i++) {
#if SIZEOF_UNSIGNED_LONG == 1
#elif SIZEOF_UNSIGNED_LONG == 2
		input->field[i] = htole16(input->field[i]);
#elif SIZEOF_UNSIGNED_LONG == 4
		input->field[i] = htole32(input->field[i]);
#elif SIZEOF_UNSIGNED_LONG == 8
		input->field[i] = htole64(input->field[i]);
#else
#error Not implemented
#endif
	}
	for (i = 0; i < int32s; i++)
		input_int[i] = htole32(input_int[i]);
	cmp = memcmp(input_int, input->field, (len - 1) / CHAR_BIT + 1);
	if (cmp != 0)
		retval = 1;
	bfdel(input);
	free(input_int);
	printf("%s\n", status[retval]);
	return retval;
}
