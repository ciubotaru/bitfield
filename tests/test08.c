/**
 * File name: tests/test08.c
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

/* Testing bfrev() and bfrev_ip */

int main()
{
	srand((unsigned)time(NULL));
	int i;			//counter
	int len = 80;
	char *msg = "Testing bfrev() and bfrev_ip()";
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
	char *input_char_rev = malloc((len + 1) * sizeof(char));
	for (i = 0; i < len; i++) {
		if (input_char[i] == '1')
			input_char_rev[len - i - 1] = '1';
		else
			input_char_rev[len - i - 1] = '0';
	}
	input_char_rev[len] = '\0';

	struct bitfield *input1 = str2bf(input_char);

	struct bitfield *input2 = bfrev(input1);
	bfdel(input1);

	for (i = 0; i < len; i++) {
		if (BITGET(input2, i) != input_char_rev[i] - '0') {
			retval = 1;
			goto ret;
		}
	}

	bfrev_ip(input2);
	for (i = 0; i < len; i++) {
		if (BITGET(input2, i) != input_char[i] - '0') {
			retval = 1;
			goto ret;
		}
	}

 ret:
	free(input_char);
	free(input_char_rev);
	bfdel(input2);
	printf("%s\n", status[retval]);
	return retval;
}