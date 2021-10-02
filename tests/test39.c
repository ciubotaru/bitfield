/**
 * File name: tests/test39.c
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

/* Testing bftouint16_ip() and uint16tobf_ip() */

int main()
{
	srand((unsigned)time(NULL));
	unsigned int i, cmp;	//counter
	unsigned int len = 80;
	char *msg = "Testing bftouint16_ip() and uint16tobf_ip()";
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
	unsigned int int16s = (len - 1) / 16 + 1;
	uint16_t *input_uint16 = malloc(int16s * sizeof(uint16_t));
	bftouint16_ip(input, input_uint16);
	/* check first function */
	uint16_t *check_uint16 = malloc(int16s * sizeof(uint16_t));
	for (i = 0; i < int16s; i++)
		check_uint16[i] = htole16(input_uint16[i]);
	struct bitfield *check = bfclone(input);
	for (i = 0; i < BITNSLOTS(len); i++) {
#if STORAGE_UNIT_SIZE == 8
#elif STORAGE_UNIT_SIZE == 16
		check->field[i] = htole16(check->field[i]);
#elif STORAGE_UNIT_SIZE == 32
		check->field[i] = htole32(check->field[i]);
#elif STORAGE_UNIT_SIZE == 64
		check->field[i] = htole64(check->field[i]);
#else
#error Not implemented
#endif
	}
	cmp = memcmp(check_uint16, check->field, (len - 1) / CHAR_BIT + 1);
	free(check_uint16);
	bfdel(check);
	if (cmp != 0) {
		retval = 1;
		goto ret;
	}

	struct bitfield *output = bfnew(len);
	uint16tobf_ip(input_uint16, output);
	/* check second function */
	cmp = bfcmp(input, output, NULL);
	bfdel(output);
	if (cmp != 0)
		retval = 1;
 ret:
	bfdel(input);
	free(input_uint16);
	printf("%s\n", status[retval]);
	return retval;
}
