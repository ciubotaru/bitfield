/**
 * File name: tests/test32.c
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

/* Testing uint16tobf() */

int main()
{
	srand((unsigned)time(NULL));
	int i, j, cmp;		//counters
	int len = 80;
	char *msg = "Testing uint16tobf()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	int shorts = (len - 1) / 16 + 1;
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
				input[i] = (unsigned short) htole16((uint16_t) input[i]);
				break;
			case 4:
				input[i] = (unsigned short) htole32((uint32_t) input[i]);
				break;
		}
	}
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
	cmp = memcmp(input, output->field, shorts * sizeof(unsigned short));
	free(input);
	bfdel(output);
	if (cmp != 0) {
		printf("%s\n", failed);
		return 1;
	}
	printf("%s\n", passed);
	return 0;
}
