/**
 * File name: tests/test34.c
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

/* Testing uint32tobf() */

int main()
{
	uint32_t unit = 1;
	srand((unsigned)time(NULL));
	int i, j;		//counters
	int len = 80;
	char *msg = "Testing uint32tobf()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	int bitnslots = (len - 1) / 32 + 1;
	uint32_t *input = calloc(1, bitnslots * sizeof(uint32_t));
	for (i = 0; i < bitnslots - 1; i++) {
		for (j = 0; j < 32; j++) {
			if (rand() % 2)
				input[i] |= (unit << j);
		}
	}
	for (i = 0; i < len % 32; i++)
		if (rand() % 2)
			input[bitnslots - 1] |= (unit << i);
	struct bitfield *output = uint32tobf(input, len);
	for (i = 0; i < bitnslots; i++) {
		if (memcmp(&input[i], bfsub(output, i * 32,
			  (i + 1) * 32)->field, sizeof(uint32_t)) != 0) {
			printf("%s\n", failed);
			return 1;
		}
	}
	printf("%s\n", passed);
	return 0;
}
