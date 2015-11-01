/**
 * File name: tests/test13.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: October 20, 2015
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"

/* Testing bfhamming() */

int main()
{
	srand((unsigned)time(NULL));
	int i;			//counter
	int len = 80;
	char *msg = "Testing bfhamming()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");

	char *input_char1 = malloc((len + 1) * sizeof(char));	/* allocate space for 80+1 chars */
	for (i = 0; i < len; i++) {
		if (rand() % 2) {
			input_char1[i] = '1';
		} else
			input_char1[i] = '0';
	}
	input_char1[len] = '\0';

	char *input_char2 = malloc((len + 1) * sizeof(char));	/* allocate space for 80+1 chars */
	for (i = 0; i < len; i++) {
		if (rand() % 2) {
			input_char2[i] = '1';
		} else
			input_char2[i] = '0';
	}
	input_char2[len] = '\0';

	/* Hamming distance counter for char string */
	int hamming_s = 0;
	for (i = 0; i < len; i++) {
		if (input_char1[i] != input_char2[i])
			hamming_s++;
	}
//      printf("%s\n", input_char1);
//      printf("%s\n", input_char2);
//      printf("%i\n", humming_s);

	struct bitfield *input1 = bfnew_quick(len);
	struct bitfield *input2 = bfnew_quick(len);
	str2bf(input_char1, input1);
	str2bf(input_char2, input2);
//      bfprint(input1);
//      bfprint(input2);

	/* Hamming distance for bitfield */
	int hamming_b = bfhamming(input1, input2);
//      printf("%i\n", humming_b);

	if (hamming_s != hamming_b) {
		printf("%s\n", failed);
		return 1;
	}
	printf("%s\n", passed);
	return 0;
}
