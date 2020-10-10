/**
 * File name: tests/test49.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: January 10, 2020
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitfield.h"
#include "bitfield-internals.h"
#include <endian.h>

/* Testing bfclo() */

int main()
{
	int i;			//counter
	unsigned int len = 400;
	char *msg = "Testing bfclo()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	unsigned int dots = 80 - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input = bfnew(len);
	unsigned int clo;
	unsigned int clo_check;
	/* testing bfclo for no clear bits */
	clo_check = bfclo(input);
	if (clo_check != 0) {
		retval = 1;
		goto ret;
	}
	/* testing bfclo for a clear set in all possible places */
	for (i = len - 1; i >= 0; i--) {
		BITSET(input, i);
		clo_check = bfclo(input);
		if (len - i != clo_check) {
			retval = 1;
			goto ret;
		}
	}
 ret:
	bfdel(input);
	printf("%s\n", status[retval]);
	return retval;
}
