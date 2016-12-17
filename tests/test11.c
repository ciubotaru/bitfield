/**
 * File name: tests/test11.c
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

/* Testing bftogglebit() */

int main()
{
	srand((unsigned)time(NULL));
	int i;			//counter
	int len = 80;
	char *msg = "Testing bftogglebit()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");

	struct bitfield *input = bfnew_quick(len);
	for (i = 0; i < len; i++) {
		if (rand() % 2 == 1)
			bfsetbit(input, i);
		else
			bfclearbit(input, i);
	}

	struct bitfield *output = bfclone(input);
	for (i = 0; i < len; i++)
		bftogglebit(output, i);

	struct bitfield *not = bfnot(output);
	bfdel(output);

	int cmp = bfcmp(input, not, NULL);
	bfdel(input);
	bfdel(not);

	if (cmp != 0) {
		printf("%s\n", failed);
		return 1;
	}
	printf("%s\n", passed);
	return 0;
}
