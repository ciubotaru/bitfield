/**
 * File name: examples/rule30.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: September 1, 2015
**/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <bitfield.h>

int main()
{
	printf("Wolfram's Rule30 cellular automaton...\n");
	srand(time(NULL));
	int size = 78;
	int i;
	/* create an array of parent generation cells with 2 extra cells */
	struct bitfield *input = bfnew(size + 2);
	/* randomly assign "dead" (0) or "alive" (1) status to parent generation */
	for (i = 0; i < bfsize(input); i++)
		if (rand() % 2 == 1)
			bfsetbit(input, i);
	/* show it */
	bfprint_lsb(input);
	printf("\n");
	/* Because the state of every cell depends on its parent and the parent's neighbours from right and left,
	 * we need three arrays: neighbours from left, parents (center) and neighbours from right.
	 */
	struct bitfield *left = bfsub(input, 0, bfsize(input) - 2);
	struct bitfield *center = bfsub(input, 1, bfsize(input) - 1);
	struct bitfield *right = bfsub(input, 2, bfsize(input));
	/* compute the child generation by Rule 30:
	 * Child(i) = Parent(i-1) XOR ( Parent(i) OR Parent(i+1) )
	 */
	struct bitfield *output = bfxor(left, bfor(center, right));
	/* show it */
	printf(" ");
	bfprint_lsb(output);
	printf("\n");
	printf("---\n");
	return 0;
}
