/**
 * File name: tests/test20.c
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

/* Testing bf2int() */

int main()
{
	srand((unsigned)time(NULL));
	int i, cmp;		//counter
	int len = 80;
	char *msg = "Testing bf2int()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input = bfnew(len);
	for (i = 0; i < len; i++)
		if (rand() % 2)
			BITSET(input, i);
	int bitnslots = BITNSLOTS(len);
	int ints = (len - 1) / INT_BIT + 1;
	unsigned int *output = bf2int(input);
	unsigned int *output2 = malloc(bitnslots * sizeof(unsigned long));
#if __BYTE_ORDER == __BIG_ENDIAN
	for (i = 0; i < bitnslots; i++) {
		switch (sizeof(unsigned long)) {
		case 4:
			input->field[i] = htole32(input->field[i]);
			break;
		case 8:
			input->field[i] = htole64(input->field[i]);
			break;
		}
	}
#endif
	memcpy(output2, input->field, bitnslots * sizeof(unsigned long));
#if __BYTE_ORDER == __BIG_ENDIAN
	for (i = 0; i < ints; i++) {
		switch (sizeof(unsigned int)) {
		case 2:
			output2[i] = le16toh(output2[i]);
			break;
		case 4:
			output2[i] = le32toh(output2[i]);
			break;
		}
	}
#endif
	if (ints * sizeof(unsigned int) != bitnslots * sizeof(unsigned long)) {
		output2 =
		    (unsigned int *)realloc(output2,
					    ints * sizeof(unsigned int));
		if (output2 == NULL)
			free(output2);
	}
	cmp = memcmp(output, output2, ints * sizeof(unsigned int));
	if (cmp != 0) retval = 1;
ret:
	bfdel(input);
	free(output);
	free(output2);
	printf("%s\n", status[retval]);
	return retval;
}
