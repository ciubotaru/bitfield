/**
 * File name: tests/test48.c
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

/* Testing bflz() */

int main()
{
	unsigned int i;		//counter
	unsigned int len = 400;
	char *msg = "Testing bflz()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	unsigned int dots = 80 - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input = bfnew_ones(len);
	unsigned int flz;
	unsigned int flz_check;
	/* testing bflz for no clear bits */
	flz_check = bflz(input);
	if (flz_check != 0) {
		retval = 1;
		goto ret;
	}
	/* testing bfls for a clear in all possible places */
	for (i = 0; i < len; i++) {
		BITCLEAR(input, i);
		flz_check = bflz(input);
		if (i + 1 != flz_check) {
			retval = 1;
			goto ret;
		}
		BITSET(input, i);
	}
 ret:
	bfdel(input);
	printf("%s\n", status[retval]);
	return retval;
}
