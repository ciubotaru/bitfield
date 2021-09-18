/**
 * File name: tests/test32.c
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

/* Testing uint16tobf() */

int main()
{
	srand((unsigned)time(NULL));
	unsigned int i, j, cmp;	//counters
	unsigned int len = 80;
	char *msg = "Testing uint16tobf()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	unsigned int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	unsigned int shorts = (len - 1) / 16 + 1;
	uint16_t *input = calloc(1, shorts * sizeof(uint16_t));
	for (i = 0; i < shorts - 1; i++) {
		for (j = 0; j < 16; j++) {
			if (rand() % 2)
				input[i] |= (1U << j);
		}
	}
	for (i = 0; i < len % 16; i++)
		if (rand() % 2)
			input[shorts - 1] |= (1U << i);
	struct bitfield *output = uint16tobf(input, len);
	for (i = 0; i < shorts; i++) {
		switch (sizeof(unsigned short)) {
		case 2:
			input[i] = (unsigned short)htole16((uint16_t) input[i]);
			break;
		case 4:
			input[i] = (unsigned short)htole32((uint32_t) input[i]);
			break;
		}
	}
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
	cmp = memcmp(input, output->field, shorts * sizeof(unsigned short));
	if (cmp != 0)
		retval = 1;
	free(input);
	bfdel(output);
	printf("%s\n", status[retval]);
	return retval;
}
