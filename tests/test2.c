/**
 * File name: tests/test2.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie@ciubotaru.tokyo>
 * License: General Public License, version 3 or later
 * Copyright 2015, 2016, 2017, 2020
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"

/* Testing bf2str_ip() and str2bf_ip() */

int main()
{
	srand((unsigned)time(NULL));
	int i, cmp;		//counter
	int len = 80;
	char *msg = "Testing bf2str_ip() and str2bf_ip()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input = bfnew(len);
	struct bitfield *output = bfnew(len);
	char *input_char = malloc((len + 1) * sizeof(char));
	for (i = 0; i < len; i++)
		if (rand() % 2)
			BITSET(input, i);
	bf2str_ip(input, input_char);
	str2bf_ip(input_char, output);
	free(input_char);
	cmp = bfcmp(input, output, NULL);
	bfdel(input);
	bfdel(output);
	if (cmp != 0) {
		retval = 1;
	}
	printf("%s\n", status[retval]);
	return retval;
}
