/**
 * File name: tests/test52.c
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

/* Testing bfctz() */

int main()
{
	int i;			//counter
	unsigned int len = 400;
	char *msg = "Testing bfctz()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	unsigned int dots = 80 - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input = bfnew_ones(len);
	unsigned int ctz;
	unsigned int ctz_check;
	/* testing bfctz for no clear bits */
	ctz_check = bfctz(input);
	if (ctz_check != 0) {
		retval = 1;
		goto ret;
	}
	/* testing bfctz for a clear bit in all possible places */
	for (i = 0; i < len; i++) {
		BITCLEAR(input, i);
		ctz_check = bfctz(input);
		if (i + 1 != ctz_check) {
			retval = 1;
			goto ret;
		}
	}
 ret:
	bfdel(input);
	printf("%s\n", status[retval]);
	return retval;
}
