/**
 * File name: examples/random.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: September 9, 2015
**/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
//#include <sys/types.h>
//#include <unistd.h>
#include <bitfield.h>

int main()
{
	printf("Generating a long random sequence and storing it in a bit array...\n");
	srand(time(NULL));
	int size = 80;
	int i;
	struct bitfield *input = bfnew(size);
	for (i = 0; i < size; i++) {
		if (rand() % 2)
			bfsetbit(input, i);
	}
	bfprint(input);
	printf("\n");
	printf("---\n");
	return 0;
}
