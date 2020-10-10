/**
 * File name: tests/test15.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: November 15, 2015
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"

/* Testing bf2long() */

int main()
{
	srand((unsigned)time(NULL));
	int i, cmp;		//counter
	int len = 80;
	char *msg = "Testing bf2long()";
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
	unsigned long *input_long = bf2long(input);
	int bitnslots = BITNSLOTS(len);
	unsigned long *input_long2 = malloc(bitnslots * sizeof(unsigned long));
	for (i = 0; i < bitnslots; i++)
		input_long2[i] = input->field[i];
	cmp =
	    memcmp(input_long, input_long2, bitnslots * sizeof(unsigned long));
	if (cmp)
		retval = 1;

	bfdel(input);
	free(input_long);
	free(input_long2);
	printf("%s\n", status[retval]);
	return retval;
}
