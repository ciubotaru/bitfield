/**
 * File name: tests/test44.c
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

/* Testing bf2ll_ip() and ll2bf_ip() */

int main()
{
	srand((unsigned)time(NULL));
	unsigned int i, cmp;			//counter
	unsigned int len = 80;
	char *msg = "Testing bf2ll_ip() and ll2bf_ip()";
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
	unsigned long long *input_ll =
	    malloc(lls * sizeof(unsigned long long));
	bf2ll_ip(input, input_ll);
	/* check first function */
	struct bitfield *check = bfclone(input);
	for (i = 0; i < BITNSLOTS(len); i++) {
		switch (sizeof(unsigned long)) {
			case 8:
				check->field[i] = (unsigned long) htole64((uint64_t) input->field[i]);
				break;
			case 4:
				check->field[i] = (unsigned long) htole32((uint32_t) input->field[i]);
				break;
		}
	}
	unsigned long long *check_ll =
	    malloc(lls * sizeof(unsigned long long));
	for (i = 0; i < lls; i++) check_ll[i] = (unsigned long long) htole64((uint64_t) input_ll[i]);
	cmp = memcmp(check->field, check_ll, (len - 1) / CHAR_BIT + 1);
	bfdel(check);
	free(check_ll);
	if (cmp != 0) {
		bfdel(input);
		free(input_ll);
		printf("%s\n", failed);
		return 1;
	}
	struct bitfield *output = bfnew(len);
	ll2bf_ip(input_ll, output);
	free(input_ll);
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
