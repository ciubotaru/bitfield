/**
 * File name: tests/test34.c
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

/* Testing uint32tobf() */

int main()
{
	uint32_t unit = 1;
	srand((unsigned)time(NULL));
	int i, j, cmp;		//counters
	int len = 80;
	char *msg = "Testing uint32tobf()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	int int32s = (len - 1) / 32 + 1;
	uint32_t *input = calloc(1, int32s * sizeof(uint32_t));
	for (i = 0; i < int32s - 1; i++) {
		for (j = 0; j < 32; j++) {
			if (rand() % 2)
				input[i] |= (unit << j);
		}
	}
	for (i = 0; i < len % 32; i++)
		if (rand() % 2)
			input[int32s - 1] |= (unit << i);
	struct bitfield *output = uint32tobf(input, len);
/*
	int min_memory_length =
	    (bitnslots * sizeof(uint32_t) <
	     BITNSLOTS(len) * sizeof(unsigned long)) ? (bitnslots *
							sizeof(uint32_t)) :
	    BITNSLOTS(len) * sizeof(unsigned long);
*/
//	uint32_t *input2 = uint32_htole(input, bitnslots);
	for (i = 0; i < int32s; i++) input[i] = htole32(input[i]);
//	free(input);
	for (i = 0; i < BITNSLOTS(len); i++) {
		switch(sizeof(unsigned long)) {
			case 4:
				output->field[i] = (unsigned long) htole32((uint32_t) output->field[i]);
				break;
			case 8:
				output->field[i] = (unsigned long) htole64((uint64_t) output->field[i]);
				break;
		}
	}
	cmp = memcmp(input, output->field, int32s * sizeof(uint32_t));
	free(input);
	bfdel(output);
	if (cmp != 0) {
		printf("%s\n", failed);
		return 1;
	}
	printf("%s\n", passed);
	return 0;
}
