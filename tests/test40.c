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
#include <endian.h>

/* Testing bftouint32_ip() and uint32tobf_ip() */

int main()
{
	srand((unsigned)time(NULL));
	unsigned int i, cmp;	//counter
	unsigned int len = 80;
	char *msg = "Testing bftouint32_ip() and uint32tobf_ip()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	unsigned int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input = bfnew(len);
	for (i = 0; i < len; i++)
		if (rand() % 2)
			BITSET(input, i);
	unsigned int int32s = (len - 1) / 32 + 1;
	uint32_t *input_uint32 = malloc(int32s * sizeof(uint32_t));
	bftouint32_ip(input, input_uint32);
	/* check first function */
	struct bitfield *check = bfclone(input);
	uint32_t *check_uint32 = malloc(int32s * sizeof(uint32_t));
	for (i = 0; i < BITNSLOTS(len); i++) {
		switch (sizeof(unsigned long)) {
		case 8:
			check->field[i] =
			    (unsigned long)htole64((uint64_t) input->field[i]);
			break;
		case 4:
			check->field[i] =
			    (unsigned long)htole32((uint32_t) input->field[i]);
			break;
		}
	}
	for (i = 0; i < int32s; i++)
		check_uint32[i] = htole32(input_uint32[i]);
	cmp = memcmp(check_uint32, check->field, (len - 1) / CHAR_BIT + 1);
	bfdel(check);
	free(check_uint32);
	if (cmp != 0) {
		free(input_uint32);
		bfdel(input);
		printf("%s\n", failed);
		return 1;
	}

	struct bitfield *output = bfnew(len);
	uint32tobf_ip(input_uint32, output);
	free(input_uint32);
	/* check second function */
	cmp = bfcmp(input, output, NULL);
	bfdel(input);
	bfdel(output);
	if (cmp != 0) {
		printf("%s\n", failed);
		return 1;
	}
	printf("%s\n", passed);
	return 0;
}
