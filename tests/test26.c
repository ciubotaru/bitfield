/**
 * File name: tests/test26.c
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

/* Testing bf2char_ip() and char2bf_ip() */

int main()
{
	srand((unsigned)time(NULL));
	unsigned int i, cmp;			//counter
	unsigned int len = 80;
	char *msg = "Testing bf2char_ip() and char2bf_ip()";
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
	unsigned int chars = (len - 1) / CHAR_BIT + 1;
	unsigned char *input_char = malloc(chars * sizeof(unsigned char));
	bf2char_ip(input, input_char);
	/* check first function */
	struct bitfield *check = bfclone(input);
	for (i = 0; i < BITNSLOTS(len); i++) {
		switch (sizeof(unsigned long)) {
			case 4:
				check->field[i] = (unsigned long) htole32((uint32_t) check->field[i]);
				break;
			case 8:
				check->field[i] = (unsigned long) htole64((uint64_t) check->field[i]);
				break;
		}
	}
	unsigned char *check_char = malloc(chars * sizeof(unsigned char));
	memcpy(check_char, check->field, chars);
	bfdel(check);
	cmp = memcmp(input_char, check_char, chars);
	free(check_char);
	if (cmp != 0) {
		printf("%s\n", failed);
		return 1;
	}
	struct bitfield *output = bfnew(len);
	char2bf_ip(input_char, output);
	free(input_char);
	/* check second function */
	cmp = bfcmp(input, output, NULL);
	bfdel(input);
	bfdel(output);
	if (cmp != 0) {
		printf("%s\n", failed);
		return 1;
	}
	printf("%s\n", passed);
	return 0;
}
