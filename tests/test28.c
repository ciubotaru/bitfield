/**
 * File name: tests/test28.c
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

/* Testing bf2short() */

int main()
{
	srand((unsigned)time(NULL));
	unsigned int i, cmp;	//counter
	unsigned int len = 80;
	char *msg = "Testing bf2short()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	unsigned int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	unsigned int shorts = (len - 1) / SHORT_BIT + 1;
	struct bitfield *input = bfnew(len);
	for (i = 0; i < len; i++)
		if (rand() % 2)
			BITSET(input, i);
	unsigned short *input_short = bf2short(input);
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
//      long_htole_ip(input, BITNSLOTS(len));
	unsigned short *check = malloc(shorts * sizeof(unsigned short));
	memcpy(check, input->field, shorts * sizeof(unsigned short));
	bfdel(input);
	for (i = 0; i < shorts; i++) {
		switch (sizeof(unsigned short)) {
		case 2:
			check[i] = (unsigned short)le16toh((uint16_t) check[i]);
			break;
		case 4:
			check[i] = (unsigned short)le32toh((uint32_t) check[i]);
			break;
		}
	}
	cmp = memcmp(input_short, check, shorts * sizeof(unsigned short));
	free(input_short);
	free(check);
	if (cmp != 0) {
		printf("%s\n", failed);
		return 1;
	}
	printf("%s\n", passed);
	return 0;
}
