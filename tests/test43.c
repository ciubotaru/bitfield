/**
 * File name: tests/test43.c
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

/* Testing bf2ll() */

int main()
{
	srand((unsigned)time(NULL));
	unsigned int i, cmp;			//counter
	unsigned int len = 80;
	char *msg = "Testing bf2ll()";
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
	unsigned int lls = (len - 1) / LONG_LONG_BIT + 1;
	unsigned long long *output = bf2ll(input);
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
	for (i = 0; i < lls; i++) output[i] = (unsigned long long) htole64((uint64_t) output[i]);
	cmp = memcmp(input->field, output, (len - 1) / CHAR_BIT + 1);
	free(output);
	bfdel(input);
	if (cmp != 0) {
		printf("%s\n", failed);
		return 1;
	}
	printf("%s\n", passed);
	return 0;
}
