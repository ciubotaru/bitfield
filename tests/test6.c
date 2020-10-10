/**
 * File name: tests/test6.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: October 1, 2015
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitfield.h"
#include "bitfield-internals.h"

/* Testing bfresize() and bfclearbit() */

int main()
{
	int i;			//counter
	int len = 80;
	char *msg = "Testing bfsetbit() and bfclearbit()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input = bfnew_quick(len);
	for (i = 0; i < len; i++) {
		bfsetbit(input, i);
		if (BITGET(input, i) != 1) {
			retval = 1;
			goto ret;
		}
	}

	for (i = 0; i < len; i++) {
		bfclearbit(input, i);
		if (BITGET(input, i) != 0) {
			retval = 1;
			goto ret;
		}
	}
 ret:
	bfdel(input);
	printf("%s\n", status[retval]);
	return retval;
}
