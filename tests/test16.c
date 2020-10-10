/**
 * File name: tests/test16.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: November 15, 2015
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"

/* Testing bfisempty() */

int main()
{
	srand((unsigned)time(NULL));
	int i, j;		//counters
	int len = 80;
	int content;
	char *msg = "Testing bfisempty()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");

	/* 50 trials */
	for (i = 0; i < 50; i++) {
		/* create a bitfield */
		struct bitfield *input = bfnew(len);

		/* choose whether to leave it empty or fill with random ones */
		if (rand() % 2) {
			/* fill with ones */
			content = 1;
			for (j = 0; j < len; j++) {
				if (rand() % 2) {
					BITSET(input, j);
				}
			}
			/* make sure there's at least one unit */
			BITSET(input, rand() % len);
		}
		/* leave empty */
		else
			content = 0;
		/* check if result of bfisempty corresponds with the value of content */
		int result = bfisempty(input);
		if (result != content) {
			bfdel(input);
			retval = 1;
			goto ret;
		}
		bfdel(input);
	}
 ret:
	printf("%s\n", status[retval]);
	return retval;
}
