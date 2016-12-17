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

/* Testing bftouint64() */

int main()
{
	srand((unsigned)time(NULL));
	int i, cmp;			//counter
	int len = 80;
	char *msg = "Testing bftouint64()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input = bfnew(len);
	for (i = 0; i < len; i++)
		if (rand() % 2)
			BITSET(input, i);
	int int64s = (len - 1) / 64 + 1;
	uint64_t *input_int = bftouint64(input);
	for (i = 0; i < int64s; i++) input_int[i] = htole64(input_int[i]);
	for (i = 0; i < BITNSLOTS(len); i++) {
		switch (sizeof(unsigned long)) {
			case 8:
				input->field[i] = (unsigned long) htole64((uint64_t) input->field[i]);
				break;
			case 4:
				input->field[i] = (unsigned long) htole32((uint32_t) input->field[i]);
				break;
		}
	}
	cmp = memcmp(input_int, input->field, (len - 1) / CHAR_BIT + 1);
	free(input_int);
	bfdel(input);
	if (cmp != 0) {
		printf("%s\n", failed);
		return 1;
	}
	printf("%s\n", passed);
	return 0;
}
