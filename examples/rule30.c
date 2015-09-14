#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <bitfield.h>

int main() {
    srand(time(NULL));
    int size = 78;
    int i;
    /* create an array of parent generation cells with 2 extra cells */
    struct bitfield * input = bfnew(size + 2);
    /* randomly assign "dead" (0) or "alive" (1) status to parent generation */
    for (i = 0; i < input->size; i++) if (rand() % 2 == 1) BITSET(input, i);
    /* show it */
    bfprint(input);
    /* Because the state of every cell depends on its parent and the parent's neighbours from right and left,
     * we need three arrays: neighbours from left, parents (center) and neighbours from right.
     */
    struct bitfield * left = bfsub(input, 0, input->size - 2);
    struct bitfield * center = bfsub(input, 1, input->size - 1);
    struct bitfield * right = bfsub(input, 2, input->size);
    /* compute the child generation by Rule 30:
     * Child(i) = Parent(i-1) XOR ( Parent(i) OR Parent(i+1) )
     */
    struct bitfield * output = bfxor(left, bfor(center, right));
    /* show it */
    printf(" ");
    bfprint(output);
    return 1;
}
