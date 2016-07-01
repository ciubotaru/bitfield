/**
 * File name: tests/test21.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: July 1, 2016
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "bitfield.h"
#include "bitfield-internals.h"

/* Testing bfnormalize() */

int main()
{
	srand((unsigned)time(NULL));
	int i;			//counter
	int len = 80;
	char *msg = "Testing bfnormalize()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");

	/**
	 * test contents:
	 * take a string of ones, insert a substring of zeroes of random length
	 * and see if bfnormalize will align zeroes at the end
	 **/

	/* number of zeroes between 1 and len - 1 */
	int nr_zeroes, nr_ones_head, nr_ones_tail;
	struct bitfield *zeroes, *ones_head, *ones_tail, *input, *check, *output;
	
	for (i = 0; i < 50; i++) {
		nr_zeroes = rand() % (len - 2) + 1;
		zeroes = bfnew(nr_zeroes);
		nr_ones_head = rand() % (len - nr_zeroes - 1) + 1;
		ones_head = bfnew_ones(nr_ones_head);
		nr_ones_tail = len - nr_zeroes - nr_ones_head;
		ones_tail = bfnew_ones(nr_ones_tail);
	
		input = bfcat(ones_head, zeroes);
		input = bfcat(input, ones_tail);
//bfprint(input);
//printf("\n");
		check = bfcat(ones_head, ones_tail);
		check = bfcat(check, zeroes);
//bfprint(check);
//printf("\n");
		output = bfnormalize(input);
//bfprint(output);
//printf("\n");
		if (bfcmp(output, check, NULL)) {
			printf("%s\n", failed);
			return 1;
		}
	}
	printf("%s\n", passed);
	return 0;
}
