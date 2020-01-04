/**
 * File name: tests/test29.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: September 1, 2016
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"
#include <endian.h>

/* Testing bf2short_ip() and short2bf_ip() */

int main()
{
	srand((unsigned)time(NULL));
	unsigned int i, cmp;	//counter
	unsigned int len = 80;
	char *msg = "Testing bf2short_ip() and short2bf_ip()";
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
	unsigned int shorts = (len - 1) / SHORT_BIT + 1;
	unsigned short *input_short = malloc(shorts * sizeof(unsigned short));
	bf2short_ip(input, input_short);
	/* check first function */
	struct bitfield *check = bfclone(input);
	for (i = 0; i < BITNSLOTS(len); i++) {
		switch (sizeof(unsigned long)) {
		case 4:
			check->field[i] =
			    (unsigned long)htole32((uint32_t) check->field[i]);
			break;
		case 8:
			check->field[i] =
			    (unsigned long)htole64((uint64_t) check->field[i]);
			break;
		}
	}
	unsigned short *check_short =
	    calloc(1, shorts * sizeof(unsigned short));
	memcpy(check_short, check->field, shorts * sizeof(unsigned short));
	bfdel(check);
	for (i = 0; i < shorts; i++) {
		switch (sizeof(unsigned short)) {
		case 2:
			check_short[i] =
			    (unsigned short)le16toh((uint16_t) check_short[i]);
			break;
		case 4:
			check_short[i] =
			    (unsigned short)le32toh((uint32_t) check_short[i]);
			break;
		}
	}
	cmp = memcmp(input_short, check_short, shorts * sizeof(unsigned short));
	free(check_short);
	if (cmp != 0) {
		retval = 1;
		goto ret;
	}
	struct bitfield *output = bfnew(len);
	short2bf_ip(input_short, output);
	/* check second function */
	cmp = bfcmp(input, output, NULL);
	bfdel(output);
	if (cmp != 0) {
		retval = 1;
		goto ret;
	}
ret:
	free(input_short);
	bfdel(input);
	printf("%s\n", status[retval]);
	return retval;
}
