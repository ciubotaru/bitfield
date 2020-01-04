/**
 * File name: tests/test24.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: July 10, 2016
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"
#include <endian.h>

/* Testing char2bf() */

int main()
{
	srand((unsigned)time(NULL));
	unsigned int i, j, cmp;	//counters
	unsigned int len = 80;
	char *msg = "Testing char2bf()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	unsigned int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	unsigned int chars = (len - 1) / CHAR_BIT + 1;
	unsigned char *input = calloc(1, chars);
	for (i = 0; i < chars - 1; i++) {
		for (j = 0; j < CHAR_BIT; j++) {
			if (rand() % 2)
				input[i] |= (1U << j);
		}
	}
	for (i = 0; i < (len - 1) % CHAR_BIT + 1; i++)
		if (rand() % 2)
			input[chars - 1] |= (1U << i);
	struct bitfield *output = char2bf(input, len);
	struct bitfield *output2 = bfnew(len);
	memcpy(output2->field, input, chars);
	for (i = 0; i < BITNSLOTS(len); i++) {
		switch (sizeof(unsigned long)) {
		case 4:
			output2->field[i] =
			    le32toh((uint32_t) output2->field[i]);
			break;
		case 8:
			output2->field[i] =
			    le64toh((uint64_t) output2->field[i]);
			break;
		}
	}
	cmp =
	    memcmp(output->field, output2->field,
		   BITNSLOTS(len) * sizeof(unsigned long));
	free(input);
	bfdel(output);
	bfdel(output2);
	if (cmp != 0) retval = 1;
	printf("%s\n", status[retval]);
	return retval;
}
