/**
 * File name: tests/test36.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: September 10, 2016
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"
#include <endian.h>

/* Testing uint64tobf() */

int main()
{
	uint64_t unit = 1;
	srand((unsigned)time(NULL));
	int i, j, cmp;		//counters
	int len = 80;
	char *msg = "Testing uint64tobf()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	int int64s = (len - 1) / 64 + 1;
	uint64_t *input = calloc(1, int64s * sizeof(uint64_t));
	for (i = 0; i < int64s - 1; i++) {
		for (j = 0; j < 64; j++) {
			if (rand() % 2)
				input[i] |= (unit << j);
		}
	}
	for (i = 0; i < len % 64; i++)
		if (rand() % 2)
			input[int64s - 1] |= (unit << i);
	struct bitfield *output = uint64tobf(input, len);
	for (i = 0; i < int64s; i++) input[i] = htole64(input[i]);
	for (i = 0; i < BITNSLOTS(len); i++) {
		switch (sizeof(unsigned long)) {
			case 8:
				output->field[i] = (unsigned long) htole64((uint64_t) output->field[i]);
				break;
			case 4:
				output->field[i] = (unsigned long) htole32((uint32_t) output->field[i]);
				break;
		}
	}
	cmp = memcmp(input, output->field, (len - 1) / CHAR_BIT + 1);
	bfdel(output);
	free(input);
	if (cmp != 0) {
		printf("%s\n", failed);
		return 1;
	}
	printf("%s\n", passed);
	return 0;
}
