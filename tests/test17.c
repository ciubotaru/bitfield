/**
 * File name: tests/test17.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: February 15, 2016
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"

/* Testing bfpos() */

int main()
{
	srand((unsigned)time(NULL));
	int i, j, k;		//counters
	int r;
	int len = 80;
	int result;
	char *msg = "Testing bfpos()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");

	/**
	 * set haystack size to 80
	 * test different sizes of needle, from 1 to 80
	 * test different positions of needle, from 0 to where it fits
	 * all haystacks here contain exactly one instanse of needle,
	 * so search should return non-negative-values
	**/
	for (i = 1; i < len; i++) {	// needle size
		for (j = 0; j < len - i; j++) {	// needle position in the haystack
			/* create the needle */
			struct bitfield *needle = bfnew(i);

			/* fill the needle with random values */
			for (k = 0; k < i; k++) {
				if (rand() % 2) {
					BITSET(needle, k);
				}
			}

			/* make sure the needle contains at least one set bit, selected randomly */
			r = rand() % i;
			BITSET(needle, r);

			/* create the haystack, make sure it contains the needle */
			struct bitfield *haystack = bfclone(needle);

			/* if there is space between the start of the haystack and the sought needle, fill it with zeroes */
			if (j > 0) {
				struct bitfield *start = bfnew(j);
				struct bitfield *tmp = bfcat(start, haystack);
				bfresize(haystack, bfsize(tmp));
				bfcpy(tmp, haystack);
				bfdel(start);
				bfdel(tmp);
			}

			/* if there is space between the sought needle and the end of haystack, fill it with zeroes */
			if (j + i < len) {
				struct bitfield *end = bfnew(len - j - i);
				struct bitfield *tmp2 = bfcat(haystack, end);
				bfresize(haystack, bfsize(tmp2));
				bfcpy(tmp2, haystack);
				bfdel(end);
				bfdel(tmp2);
			}

			/* now let's test */
			result = bfpos(haystack, needle);
			bfdel(haystack);
			bfdel(needle);
			/* the result should be equal to j */
			if (result != j) {
				retval = 1;
				goto ret;
			}
		}
	}
ret:
	printf("%s\n", status[retval]);
	return retval;
}
