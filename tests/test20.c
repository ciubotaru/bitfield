/**
 * File name: tests/test20.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie@ciubotaru.tokyo>
 * License: General Public License, version 3 or later
 * Copyright 2016, 2017, 2020, 2021
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
	int ints = (len - 1) / (SIZEOF_UNSIGNED_INT * CHAR_BIT) + 1;
	unsigned int *output = bf2int(input);
	for (i = 0; i < len; i++) {
		if ((output[i / (SIZEOF_UNSIGNED_INT * CHAR_BIT)] >>
		     (i % (SIZEOF_UNSIGNED_INT * CHAR_BIT)) & 1) !=
		    BITGET(input, i)) {
			retval = 1;
			goto ret;
		}
	}

 ret:
	bfdel(input);
	free(output);
	printf("%s\n", status[retval]);
	return retval;
}
