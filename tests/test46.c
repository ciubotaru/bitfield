/**
 * File name: tests/test46.c
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

/* Testing bffz() */

int main()
{
	unsigned int i;		//counter
	unsigned int len = 400;
	char *msg = "Testing bffz()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	unsigned int dots = 80 - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input = bfnew_ones(len);
	unsigned int ffz;
	unsigned int ffz_check;
	/* testing bffz for no clear bits */
	ffz_check = bffz(input);
	if (ffz_check != 0) {
		retval = 1;
		goto ret;
	}
	/* testing bffz for a clear set in all possible places */
	for (i = 0; i < len; i++) {
		BITCLEAR(input, i);
		ffz_check = bffz(input);
		if (i + 1 != ffz_check) {
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
