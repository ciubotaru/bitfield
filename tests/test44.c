/**
 * File name: tests/test44.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie@ciubotaru.tokyo>
 * License: General Public License, version 3 or later
 * Copyright 2016, 2017, 2020
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
	unsigned int i, cmp;	//counter
	unsigned int len = 80;
	char *msg = "Testing bf2ll_ip() and ll2bf_ip()";
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
	unsigned int lls = (len - 1) / LONG_LONG_BIT + 1;
	unsigned long long *input_ll = malloc(lls * sizeof(unsigned long long));
	bf2ll_ip(input, input_ll);
	/* check first function */
	struct bitfield *check = bfclone(input);
	for (i = 0; i < BITNSLOTS(len); i++) {
#if SIZEOF_UNSIGNED_LONG == 1
#elif SIZEOF_UNSIGNED_LONG == 2
		check->field[i] = htole16(check->field[i]);
#elif SIZEOF_UNSIGNED_LONG == 4
		check->field[i] = htole32(check->field[i]);
#elif SIZEOF_UNSIGNED_LONG == 8
		check->field[i] = htole64(check->field[i]);
#else
#error Not implemented
#endif
	}
	unsigned long long *check_ll = malloc(lls * sizeof(unsigned long long));
	for (i = 0; i < lls; i++)
		check_ll[i] =
		    (unsigned long long)htole64((uint64_t) input_ll[i]);
	cmp = memcmp(check->field, check_ll, (len - 1) / CHAR_BIT + 1);
	bfdel(check);
	free(check_ll);
	if (cmp != 0) {
		retval = 1;
		goto ret;
	}
	struct bitfield *output = bfnew(len);
	ll2bf_ip(input_ll, output);
	/* check second function */
	cmp = bfcmp(input, output, NULL);
	bfdel(output);
	if (cmp != 0)
		retval = 1;
 ret:
	bfdel(input);
	free(input_ll);
	printf("%s\n", status[retval]);
	return retval;
}
