/**
 * File name: tests/test28.c
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

/* Testing bf2short() */

int main()
{
	srand((unsigned)time(NULL));
	unsigned int i, cmp;	//counter
	unsigned int len = 80;
	char *msg = "Testing bf2short()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	unsigned int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	unsigned int shorts = (len - 1) / SHORT_BIT + 1;
	struct bitfield *input = bfnew(len);
	for (i = 0; i < len; i++)
		if (rand() % 2)
			BITSET(input, i);
	unsigned short *input_short = bf2short(input);
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
	unsigned short *check = malloc(shorts * sizeof(unsigned short));
	memcpy(check, input->field, shorts * sizeof(unsigned short));
	bfdel(input);
	for (i = 0; i < shorts; i++) {
		switch (sizeof(unsigned short)) {
		case 2:
			check[i] = (unsigned short)le16toh((uint16_t) check[i]);
			break;
		case 4:
			check[i] = (unsigned short)le32toh((uint32_t) check[i]);
			break;
		}
	}
	cmp = memcmp(input_short, check, shorts * sizeof(unsigned short));
	free(input_short);
	free(check);
	if (cmp != 0)
		retval = 1;
	printf("%s\n", status[retval]);
	return retval;
}
