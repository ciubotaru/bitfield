#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "bitfield.h"

int main()
{
	struct timeval time;
	gettimeofday(&time, NULL);
	srand((((time.tv_sec ^ getpid()) * 1000001) + time.tv_usec));
	int size = 80;
	int i;
	struct bitfield *input = bfnew_quick(size);
	for (i = 0; i < size; i++) {
		if (rand() % 2)
			BITSET(input, i);
		else
			BITCLEAR(input, i);
	}
	bfprint(input);
	return 0;
}
