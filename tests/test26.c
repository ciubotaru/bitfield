/**
 * File name: tests/test23.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: April 1, 2016
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"

/* Testing bf2char_ip() and char2bf_ip() */

int main()
{
	srand((unsigned)time(NULL));
	int i;			//counter
	int len = 80;
	char *msg = "Testing bf2char_ip() and char2bf_ip()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input = bfnew(len);
	for (i = 0; i < len; i++)
		if (rand() % 2)
			BITSET(input, i);
	int bitnslots = (len - 1) / CHAR_BIT + 1;
	unsigned char *input_char = malloc(bitnslots * sizeof(unsigned char));
	bf2char_ip(input, input_char);
	/* check first function */
	for (i = 0; i < bitnslots; i++) {
		if (input_char[i] != (unsigned char)bfsub(input, i * CHAR_BIT, (i + 1) * CHAR_BIT)->field[0])
		{
			printf("%s\n", failed);
			return 1;
		}
	}
	struct bitfield *output = bfnew(len);
	char2bf_ip(input_char, output);
	/* check second function */
	if (bfcmp(input, output, NULL) != 0)
		{
			printf("%s\n", failed);
			return 1;
		}
	printf("%s\n", passed);
	return 0;
}