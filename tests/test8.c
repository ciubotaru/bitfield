/**
 * File name: tests/test8.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3
 * Date: October 1, 2015
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
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
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

//      printf("%s\n", input_char);
//      printf("%s\n", input_char_rev);

	struct bitfield *input1 = bfnew_quick(len);
	str2bf(input_char, input1);
//      bfprint(input1);

	struct bitfield *input2 = bfrev(input1);
//      bfprint(input2);
	for (i = 0; i < len; i++) {
		if (BITGET(input2, i) != input_char_rev[i] - '0') {
			printf("%s\n", failed);
			free(input_char);
			free(input_char_rev);
			return 1;
		}
	}

	bfrev_ip(input2);
	for (i = 0; i < len; i++) {
		if (BITGET(input2, i) != input_char[i] - '0') {
			printf("%s\n", failed);
			free(input_char);
			free(input_char_rev);
			return 1;
		}
	}

	printf("%s\n", passed);
	free(input_char);
	free(input_char_rev);
	return 0;
}
