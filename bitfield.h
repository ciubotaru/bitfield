/**
 * File name: bitfield.h
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3
 * Date: September 1, 2015
**/

struct bitfield;

struct bitfield *bfnew(const int);	/* creates a bitfield structure, sets all its bits to false and returns a pointer to it */

struct bitfield *bfnew_quick(const int);	/* creates a bitfield structure and returns a pointer to it */

struct bitfield *bfnew_ones(const int);	/* creates a bitfield structure, sets all its bits to true with and returns a pointer to it */

struct bitfield *bfclone(const struct bitfield *);	/* creates a copy of an existing bitfield */

void bfdel(struct bitfield *);	/* destroys a bitfield structure and frees memory */

int bfcmp(const struct bitfield *, const struct bitfield *, char **);	/* compares two bitfields and returns 0 if same or non-zero and error message if different */

void bfprint(const struct bitfield *);	/* prints a bitfield as a series of ones and zeroes */

void char2bf(const char *, struct bitfield *);	/* converts a character string of ones and zeroes into a bitfield */

void bf2char(const struct bitfield *, char *);	/* converts a bitfield into a character string of ones and zeroes */

struct bitfield *bfsub(const struct bitfield *, const unsigned int, const unsigned int);	/* extracts a sub-bitfield from a bitfield */

struct bitfield *bfcat(const struct bitfield *, const struct bitfield *);	/* concatenates two bitfields into one */

struct bitfield *bfshift(const struct bitfield *, const int);	/* circular-shifts the contents of a bitfield and return the result in new bitfield */

void bfshift_ip(struct bitfield *, const int);	/* circular-shifts the contents of a bitfield */

struct bitfield *bfor(const struct bitfield *, const struct bitfield *);	/* performs bitwise inclusive OR over a pair of bitfields */

struct bitfield *bfand(const struct bitfield *, const struct bitfield *);	/* performs bitwise AND over a pair of bitfields */

struct bitfield *bfxor(const struct bitfield *, const struct bitfield *);	/* performs bitwise exclusive OR over a pair of bitfields */

struct bitfield *bfnot(const struct bitfield *);	/* reverses all bits in a bitfield and return the result in new bitfield */

void bfnot_ip(struct bitfield *);	/* reverses all bits in a bitfield */

int bfcpy(const struct bitfield *, struct bitfield *);	/* copies the contents of a bitfield into another pre-existing bitfield */

void bfclearall(struct bitfield *);	/* fills a bitfield with zeroes */

void bfsetall(struct bitfield *);	/* fills a bitfield with ones */

void bfresize(struct bitfield *, int);	/* resizes an existing bitfield */

void bfsetbit(struct bitfield *, int);	/* sets one bit in a bitfield */

void bfclearbit(struct bitfield *, int);	/* clears one bit in a bitfield */

struct bitfield *bfrev(const struct bitfield *);	/* reverses the order of bits in a bitfield and return result in new bitfield */

void bfrev_ip(struct bitfield *);	/* reverses the order of bits in a bitfield */

int bfsize(const struct bitfield *);	/* obtains the number of bits of a bitfield */

int bfgetbit(const struct bitfield *, const int);	/* checks whether a bit in a bitfield is set */

void bftogglebit(struct bitfield *, const int);		/* toggles a bit in a bitfield */

int bfpopcount(const struct bitfield *);	/* counts the set bits in a bitfield */

int bfhamming(const struct bitfield *, const struct bitfield *);	/* counts the Hamming distance between two bitfields */
