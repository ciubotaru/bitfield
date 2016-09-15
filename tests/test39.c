/**
 * File name: tests/test39.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: September 10, 2016
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"

/* Testing bftouint16_ip() and uint16tobf_ip() */

int main()
{
	srand((unsigned)time(NULL));
	int i;			//counter
	int len = 80;
	char *msg = "Testing bftouint16_ip() and uint16tobf_ip()";
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
	int bitnslots = (len - 1) / 16 + 1;
	uint16_t *input_uint16 = malloc(bitnslots * sizeof(uint16_t));
	bftouint16_ip(input, input_uint16);
	/* check first function */
	int min_memory_length = (bitnslots * sizeof(uint16_t) - BITNSLOTS(len) * sizeof(unsigned long) < 0) ? (bitnslots * sizeof(uint16_t)) : BITNSLOTS(len) * sizeof(unsigned long);
	if (memcmp(input_uint16, input->field, min_memory_length) != 0) {
		printf("%s\n", failed);
		return 1;
	}

	struct bitfield *output = bfnew(len);
	uint16tobf_ip(input_uint16, output);
	/* check second function */
	if (bfcmp(input, output, NULL) != 0)
		{
			printf("%s\n", failed);
			return 1;
		}
	printf("%s\n", passed);
	return 0;
}
