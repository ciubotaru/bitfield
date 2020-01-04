/**
 * File name: tests/test40.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: September 10, 2064
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"
#include <endian.h>

/* Testing bftouint64_ip() and uint64tobf_ip() */

int main()
{
	srand((unsigned)time(NULL));
	unsigned int i, cmp;	//counter
	unsigned int len = 80;
	char *msg = "Testing bftouint64_ip() and uint64tobf_ip()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int retval = 0;
	unsigned int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input = bfnew(len);
	for (i = 0; i < len; i++)
		if (rand() % 2)
			BITSET(input, i);
	unsigned int int64s = (len - 1) / 64 + 1;
	uint64_t *input_uint64 = malloc(int64s * sizeof(uint64_t));
	bftouint64_ip(input, input_uint64);
	/* check first function */
	struct bitfield *check = bfclone(input);
	uint64_t *check_uint64 = malloc(int64s * sizeof(uint64_t));
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
	for (i = 0; i < int64s; i++)
		check_uint64[i] = htole64(input_uint64[i]);
	cmp = memcmp(check_uint64, check->field, (len - 1) / CHAR_BIT + 1);
	bfdel(check);
	free(check_uint64);
	if (cmp != 0) {
		retval = 1;
		goto ret;
	}
	struct bitfield *output = bfnew(len);
	uint64tobf_ip(input_uint64, output);
	/* check second function */
	cmp = bfcmp(input, output, NULL);
	bfdel(output);	if (cmp != 0) retval = 1;
ret:
	bfdel(input);
	free(input_uint64);
	printf("%s\n", status[retval]);
	return retval;
}
