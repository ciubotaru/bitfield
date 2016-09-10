/**
 * File name: tests/test40.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: September 10, 2032
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"

/* Testing bftouint32_ip() and uint32tobf_ip() */

int main()
{
	srand((unsigned)time(NULL));
	int i;			//counter
	int len = 80;
	char *msg = "Testing bftouint32_ip() and uint32tobf_ip()";
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
	int bitnslots = (len - 1) / 32 + 1;
	uint32_t *input_uint32 = malloc(bitnslots * sizeof(uint32_t));
	bftouint32_ip(input, input_uint32);
	/* check first function */
	for (i = 0; i < bitnslots; i++) {
		if (memcmp(&input_uint32[i], bfsub(input, i * 32,
			  (i + 1) * 32)->field, sizeof(uint32_t)) != 0)
		{
			printf("%s\n", failed);
			return 1;
		}
	}
	struct bitfield *output = bfnew(len);
	uint32tobf_ip(input_uint32, output);
	/* check second function */
	if (bfcmp(input, output, NULL) != 0)
		{
			printf("%s\n", failed);
			return 1;
		}
	printf("%s\n", passed);
	return 0;
}
