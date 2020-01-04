/**
 * File name: tests/test25.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: April 1, 2016
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"
#include <endian.h>

/* Testing bf2char() */

int main()
{
	srand((unsigned)time(NULL));
	unsigned int i, cmp;	//counter
	unsigned int len = 80;
	char *msg = "Testing bf2char()";
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
	unsigned int bitnslots = (len - 1) / CHAR_BIT + 1;
	unsigned char *input_char = bf2char(input);
	unsigned int min_memory_length =
	    (bitnslots * sizeof(unsigned char) <
	     BITNSLOTS(len) * sizeof(unsigned long)) ? (bitnslots *
							sizeof(unsigned char)) :
	    BITNSLOTS(len) * sizeof(unsigned long);
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
	cmp = memcmp(input_char, input->field, min_memory_length);
	bfdel(input);
	free(input_char);
	if (cmp != 0) retval = 1;
	printf("%s\n", status[retval]);
	return retval;
}
