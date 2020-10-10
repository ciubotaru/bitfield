/**
 * File name: tests/test47.c
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

/* Testing bfls() */

int main()
{
	unsigned int i;		//counter
	unsigned int len = 400;
	char *msg = "Testing bfls()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	unsigned int dots = 80 - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input = bfnew(len);
	unsigned int fls;
	unsigned int fls_check;
	/* testing bflz for no set bits */
	fls_check = bfls(input);
	if (fls_check != 0) {
		retval = 1;
		goto ret;
	}
	/* testing bfls for a set in all possible places */
	for (i = 0; i < len; i++) {
		BITSET(input, i);
		fls_check = bfls(input);
		if (i + 1 != fls_check) {
			retval = 1;
			goto ret;
		}
		BITCLEAR(input, i);
	}
 ret:
	bfdel(input);
	printf("%s\n", status[retval]);
	return retval;
}
