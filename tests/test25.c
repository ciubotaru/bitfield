/**
 * File name: tests/test25.c
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

/* Testing bf2char() */

int main()
{
	srand((unsigned)time(NULL));
	unsigned int i, cmp;	//counter
	unsigned int len = 80;
	char *msg = "Testing bf2char()";
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
	unsigned int bitnslots = (len - 1) / CHAR_BIT + 1;
	unsigned char *input_char = bf2char(input);
	unsigned int min_memory_length =
	    (bitnslots * sizeof(unsigned char) <
	     BITNSLOTS(len) * sizeof(storage_unit)) ? (bitnslots *
						       sizeof(unsigned char)) :
	    BITNSLOTS(len) * sizeof(storage_unit);
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
	cmp = memcmp(input_char, input->field, min_memory_length);
	bfdel(input);
	free(input_char);
	if (cmp != 0)
		retval = 1;
	printf("%s\n", status[retval]);
	return retval;
}
