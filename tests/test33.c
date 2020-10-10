/**
 * File name: tests/test33.c
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

/* Testing bftouint16() */

int main()
{
	srand((unsigned)time(NULL));
	unsigned int i, cmp;	//counter
	unsigned int len = 80;
	char *msg = "Testing bftouint16()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	unsigned int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input = bfnew(len);
	for (i = 0; i < len; i++)
		if (rand() % 2)
			BITSET(input, i);
	unsigned int uint16s = (len - 1) / 16 + 1;
	uint16_t *output = bftouint16(input);
	for (i = 0; i < BITNSLOTS(len); i++) {
		switch (sizeof(unsigned long)) {
		case 4:
			input->field[i] =
			    (unsigned long)htole32((uint32_t) input->field[i]);
			break;
		case 8:
			input->field[i] =
			    (unsigned long)htole64((uint64_t) input->field[i]);
			break;
		}
	}
	for (i = 0; i < uint16s; i++)
		output[i] = htole16(output[i]);
	cmp = memcmp(output, input->field, uint16s * sizeof(uint16_t));
	if (cmp != 0)
		retval = 1;
	bfdel(input);
	free(output);
	printf("%s\n", status[retval]);
	return retval;
}
