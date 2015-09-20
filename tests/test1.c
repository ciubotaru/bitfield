/**
 * File name: tests/test1.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3
 * Date: September 1, 2015
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"

/* Testing bfsub() function */

int main()
{
	srand((unsigned)time(NULL));
	int i;			//counter
	int len = 80;
	char *errmsg;
	char *msg = "Testing substring extraction and concatenation";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input = bfnew(len);
	for (i = 0; i < len; i++)
		if (rand() % 2)
			BITSET(input, i);
//	bfprint(input);
	int point;
	for (point = 1; point < (len - 1); point++) {
		struct bitfield *chunk1 = bfsub(input, 0, point);
		struct bitfield *chunk2 = bfsub(input, point, input->size);
//            bfprint(chunk1);
		//          bfprint(chunk2);
		struct bitfield *output = bfcat(chunk1, chunk2);
//		bfprint(output);
		if (bfcmp(input, output, &errmsg) != 0) {
			printf("%s\n", failed);
			return 1;
		}
		bfdel(output);
		bfdel(chunk1);
		bfdel(chunk2);
	}
	printf("%s\n", passed);
	return 0;
}
