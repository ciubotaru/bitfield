/**
 * File name: tests/test10.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3
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
	int i;			//counter
	int len = 80;
	char *msg = "Testing bfsize()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");

	for (i = 1; i <= len; i++) {
		struct bitfield *input = bfnew_quick(i);
		if (bfsize(input) != i) {
			printf("%s\n", failed);
			bfdel(input);
			return 1;
		} else
			bfdel(input);
	}

	printf("%s\n", passed);
	return 0;
}
