/**
 * File name: tests/test5.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: October 1, 2015
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"

/* Testing bfresize() */

int main()
{
	srand((unsigned)time(NULL));
	unsigned int i;		//counter
	unsigned int len = 80;
	char *msg = "Testing bfresize()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	unsigned int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
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

	/* create a small bitfield */
	struct bitfield *input = bfnew_quick(1);
	if (input_char[0] == '1')
		BITSET(input, 0);
	else
		BITCLEAR(input, 0);

	/* enlarge the bit field one bit at a time */
	for (i = 1; i < len; i++) {
		bfresize(input, i + 1);
		if (input->size != (i + 1)) {
			printf("%s\n", failed);
			free(input_char);
			return 1;
		}
		/* set the newly added bit from input_char */
		if (input_char[i] == '1')
			BITSET(input, i);
		else
			BITCLEAR(input, i);
	}
	free(input_char);

	/* shrink it back one bit at a time */
	for (i = len; i > 1; i--) {
		bfresize(input, i - 1);
		if (input->size != (i - 1)) {
			printf("%s\n", failed);
			return 1;
		}
	}
	printf("%s\n", passed);
	bfdel(input);
	return 0;
}
