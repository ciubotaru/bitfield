/**
 * File name: tests/test3.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3
 * Date: September 1, 2015
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"

int main() {
    srand( (unsigned)time( NULL ) );
    int i; //counter
    int len = 80;
    struct bitfield * input1 = bfnew(len);
    struct bitfield * input2 = bfnew(len);
    struct bitfield * output1 = bfnew(len);
    struct bitfield * output2 = bfnew(len);
    for (i = 0; i < len; i++) {
        if (rand() % 2) BITSET(input1, i);
        if (rand() % 2) BITSET(input2, i);
    }
    bfprint(input1);
    bfprint(input2);
    char * errmsg;
    output1 = bfxor(input1, input2);
    bfprint(output1);
    output2 = bfand(bfor(input1, input2), bfnot(bfand(input1, input2)));
    bfprint(output2);
    if (bfcmp(output1, output2, &errmsg) != 0) {
        printf("FAILED\n");
        return 1;
    }
    else printf("PASSED\n");
    return 0;
}

