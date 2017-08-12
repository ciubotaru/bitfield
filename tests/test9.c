/**
 * File name: tests/test9.c
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

/* Testing bfgetbit() */

int main()
{
	srand((unsigned)time(NULL));
	unsigned int i;		//counter
	unsigned int len = 80;
	char *msg = "Testing bfgetbit()";
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

	struct bitfield *input = str2bf(input_char);

	for (i = 0; i < len; i++) {
		if (bfgetbit(input, i) != (unsigned int)input_char[i] - '0') {
			printf("%s\n", failed);
			free(input_char);
			bfdel(input);
			return 1;
		}
	}

	printf("%s\n", passed);
	free(input_char);
	bfdel(input);
	return 0;
}
