/**
 * File name: tests/test35.c
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

/* Testing bftouint32() */

int main()
{
	srand((unsigned)time(NULL));
	unsigned int i, cmp;			//counter
	unsigned int len = 80;
	char *msg = "Testing bftouint32()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
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
/*
	int min_memory_length =
	    (bitnslots * sizeof(uint32_t) <
	     BITNSLOTS(len) * sizeof(unsigned long)) ? (bitnslots *
							sizeof(uint32_t)) :
	    BITNSLOTS(len) * sizeof(unsigned long);
*/
	for (i = 0; i < BITNSLOTS(len); i++) {
		switch (sizeof(unsigned long)) {
			case 4:
				input->field[i] = (unsigned long) htole32((uint32_t) input->field[i]);
				break;
			case 8:
				input->field[i] = (unsigned long) htole64((uint64_t) input->field[i]);
				break;
		}
	}
	for (i = 0; i < int32s; i++) input_int[i] = htole32(input_int[i]);
	cmp = memcmp(input_int, input->field, (len - 1) / CHAR_BIT + 1);
	bfdel(input);
	free(input_int);
	if (cmp != 0) {
		printf("%s\n", failed);
		return 1;
	}
	printf("%s\n", passed);
	return 0;
}
