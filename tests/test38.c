/**
 * File name: tests/test38.c
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

/* Testing bftouint8_ip() and uint8tobf_ip() */

int main()
{
	srand((unsigned)time(NULL));
	unsigned int i, cmp;	//counter
	unsigned int len = 80;
	char *msg = "Testing bftouint8_ip() and uint8tobf_ip()";
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
	unsigned int int8s = (len - 1) / 8 + 1;
	uint8_t *input_uint8 = malloc(int8s);
	bftouint8_ip(input, input_uint8);
	/* check first function */
	struct bitfield *output = bfnew(len);
	for (i = 0; i < BITNSLOTS(len); i++) {
		switch (sizeof(unsigned long)) {
		case 8:
			output->field[i] =
			    (unsigned long)htole64((uint64_t) input->field[i]);
			break;
		case 4:
			output->field[i] =
			    (unsigned long)htole32((uint32_t) input->field[i]);
			break;
		}
	}
	cmp = memcmp(input_uint8, output->field, int8s);
	if (cmp != 0) {
		retval = 1;
		goto ret;
	}
	uint8tobf_ip(input_uint8, output);
	/* check second function */
	cmp = bfcmp(input, output, NULL);
	bfdel(output);
	if (cmp != 0) retval = 1;
ret:
	bfdel(input);
	free(input_uint8);
	printf("%s\n", status[retval]);
	return retval;
}
