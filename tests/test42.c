/**
 * File name: tests/test42.c
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

/* Testing ll2bf() */

int main()
{
	srand((unsigned)time(NULL));
	unsigned int i, j, cmp;	//counters
	unsigned int len = 80;
	char *msg = "Testing ll2bf()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	unsigned int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	unsigned int lls = (len - 1) / LONG_LONG_BIT + 1;
	unsigned long long *input = calloc(1, lls * sizeof(unsigned long long));
	for (i = 0; i < lls - 1; i++) {
		for (j = 0; j < LONG_LONG_BIT; j++) {
			if (rand() % 2)
				input[i] |= (1ULL << j);
		}
	}
	for (i = 0; i < len % LONG_LONG_BIT; i++)
		if (rand() % 2)
			input[lls - 1] |= (1ULL << i);
	struct bitfield *output = ll2bf(input, len);
	/* assume long long is always 64 bit */
	for (i = 0; i < lls; i++)
		input[i] = (unsigned long long)htole64((uint64_t) input[i]);
	for (i = 0; i < BITNSLOTS(len); i++) {
		switch (sizeof(unsigned long)) {
		case 8:
			output->field[i] =
			    (unsigned long)htole64((uint64_t) output->field[i]);
			break;
		case 4:
			output->field[i] =
			    (unsigned long)htole32((uint32_t) output->field[i]);
			break;
		}
	}
	cmp = memcmp(input, output->field, (len - 1) / CHAR_BIT + 1);
	if (cmp != 0)
		retval = 1;
	free(input);
	bfdel(output);
	printf("%s\n", status[retval]);
	return retval;
}
