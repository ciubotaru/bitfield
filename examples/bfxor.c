/**
 * File name: examples/bfxor.c
 * Project name: bitfield, a bit array manipulation library written in C                                               * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie@ciubotaru.tokyo>
 * License: General Public License, version 3 or later
 * Copyright 2017
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <bitfield.h>
#include <stdio.h>
#include <stdarg.h>

int main()
{
	printf("Running logical XOR over three bit arrays...\n");
	printf("Inputs:\n");
	struct bitfield *input1 = str2bf("10111111");
	bfprint_lsb(input1);
	printf("\n");
	struct bitfield *input2 = str2bf("10100000");
	bfprint_lsb(input2);
	printf("\n");
	struct bitfield *input3 = str2bf("0010101111111");
	bfprint_lsb(input3);
	printf("\n");
	struct bitfield *output = bfxor(input1, input2, input3);
	printf("Output:\n");
	bfprint_lsb(output);
	printf("\n");
	printf("---\n");
	return 0;
}
