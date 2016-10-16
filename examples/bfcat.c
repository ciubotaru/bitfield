#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include <stdio.h>
#include <stdarg.h>

int main()
{
	printf("Joining three bit arrays into one...\n");
	printf("Inputs:\n");
	struct bitfield *input1 = str2bf("1010101");
	bfprint_lsb(input1);
	printf("\n");
	struct bitfield *input2 = str2bf("101010100");
	bfprint_lsb(input2);
	printf("\n");
	struct bitfield *input3 = str2bf("1010101111111");
	bfprint_lsb(input3);
	printf("\n");
	struct bitfield *output =
	    bfcat(input1, input2, input3, input1, input2, input3);
	printf("Output:\n");
	bfprint_lsb(output);
	printf("\n");
	return 0;
}
