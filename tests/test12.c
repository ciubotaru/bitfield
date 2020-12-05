/**
 * File name: tests/test12.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie@ciubotaru.tokyo>
 * License: General Public License, version 3 or later
 * Copyright 2015, 2016, 2020
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"

/* Testing bfpopcount() */

int main()
{
	srand((unsigned)time(NULL));
	int i;			//counter
	int len = 80;
	char *msg = "Testing bfpopcount()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");

	char *input_char = malloc((len + 1) * sizeof(char));	/* allocate space for 80+1 chars */
	for (i = 0; i < len; i++) {
		if (rand() % 2)
			input_char[i] = '1';
		else
			input_char[i] = '0';
	}
	input_char[len] = '\0';

	/* population count in char string */
	int count_s = 0;
	for (i = 0; i < len; i++)
		if (input_char[i] == '1')
			count_s++;

	struct bitfield *input = str2bf(input_char);
	/* population count in bitfield */
	int count_b = bfpopcount(input);
	if (count_s != count_b)
		retval = 1;

	free(input_char);
	bfdel(input);
	printf("%s\n", status[retval]);
	return retval;
}
