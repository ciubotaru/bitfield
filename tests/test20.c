/**
 * File name: tests/test15.c
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

/* Testing bf2int() */

int main()
{
	srand((unsigned)time(NULL));
	int i;			//counter
	int len = 80;
	char *msg = "Testing bf2int()";
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
	int bitnslots = (len - 1) / INT_BIT + 1;
	unsigned int *input_int = bf2int(input);
	for (i = 0; i < bitnslots; i++) {
		if (input_int[i] != (unsigned int) bfsub(input, i * INT_BIT, (i + 1) * INT_BIT)->field[0]) {
			printf("%s\n", failed);
			return 1;
		}
	}
	printf("%s\n", passed);
	return 0;
}
