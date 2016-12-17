/**
 * File name: tests/test30.c
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

/* Testing uint8tobf() */

int main()
{
	srand((unsigned)time(NULL));
	int i, j, cmp;		//counters
	int len = 80;
	char *msg = "Testing uint8tobf()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	int chars = (len - 1) / 8 + 1;
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
		switch (sizeof(unsigned long)) {
			case 4:
				output->field[i] = (unsigned long) htole32((uint32_t) output->field[i]);
				break;
			case 8:
				output->field[i] = (unsigned long) htole64((uint64_t) output->field[i]);
				break;
		}
	}
	cmp = memcmp(input, output->field, chars);
	free(input);
	bfdel(output);
	if (cmp != 0) {
		printf("%s\n", failed);
		return 1;
	}
	printf("%s\n", passed);
	return 0;
}
