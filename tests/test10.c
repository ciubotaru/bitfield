/**
 * File name: tests/test10.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: October 20, 2015
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitfield.h"
#include "bitfield-internals.h"

/* Testing bfsize() */

int main()
{
	unsigned int i;		//counter
	unsigned int len = 80;
	char *msg = "Testing bfsize()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	unsigned int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");

	for (i = 1; i <= len; i++) {
		struct bitfield *input = bfnew_quick(i);
		if (bfsize(input) != i) {
			bfdel(input);
			retval = 1;
			goto ret;
		} else
			bfdel(input);
	}
ret:
	printf("%s\n", status[retval]);
	return retval;
}
