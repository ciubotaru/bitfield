/**
 * File name: bitfield.h
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Date: September 1, 2015
**/

struct bitfield;

struct bitfield *bfnew(const int size);	/* creates a bitfield structure, sets all its bits to false and returns a pointer to it */

struct bitfield *bfnew_quick(const int size);	/* creates a bitfield structure and returns a pointer to it */

struct bitfield *bfnew_ones(const int size);	/* creates a bitfield structure, sets all its bits to true with and returns a pointer to it */

struct bitfield *bfclone(const struct bitfield *input);	/* creates a copy of an existing bitfield */

void bfdel(struct bitfield *instance);	/* destroys a bitfield structure and frees memory */

int bfcmp(const struct bitfield *input1, const struct bitfield *input2, char **errmsg);	/* compares two bitfields and returns 0 if same or non-zero and error message if different */

void bfprint(const struct bitfield *instance);	/* prints a bitfield as a series of ones and zeroes */

struct bitfield *str2bf(const char *input);	/* converts a character string of ones and zeroes into a bitfield and returns the result in a new bitfield */

void str2bf_ip(const char *input, struct bitfield *output);	/* converts a character string of ones and zeroes into a bitfield */

char *bf2str(const struct bitfield *input);	/* converts a bitfield into a character string of ones and zeroes and returns the result in a new character string */

void bf2str_ip(const struct bitfield *input, char *output);	/* converts a bitfield into a character string of ones and zeroes */

struct bitfield *bfsub(const struct bitfield *input, const unsigned int start, const unsigned int end);	/* extracts a sub-bitfield from a bitfield */

struct bitfield *bfcat(const struct bitfield *input1, const struct bitfield *input2);	/* concatenates two bitfields into one */

struct bitfield *bfshift(const struct bitfield *input, const int offset);	/* circular-shifts the contents of a bitfield and return the result in new bitfield */

void bfshift_ip(struct bitfield *input, const int offset);	/* circular-shifts the contents of a bitfield */

struct bitfield *bfor(const struct bitfield *input1, const struct bitfield *input2);	/* performs bitwise inclusive OR over a pair of bitfields */

struct bitfield *bfand(const struct bitfield *input1, const struct bitfield *input2);	/* performs bitwise AND over a pair of bitfields */

struct bitfield *bfxor(const struct bitfield *input1, const struct bitfield *input2);	/* performs bitwise exclusive OR over a pair of bitfields */

struct bitfield *bfnot(const struct bitfield *input);	/* reverses all bits in a bitfield and return the result in new bitfield */

void bfnot_ip(struct bitfield *instance);	/* reverses all bits in a bitfield */

int bfcpy(const struct bitfield *src, struct bitfield *dest);	/* copies the contents of a bitfield into another pre-existing bitfield */

void bfclearall(struct bitfield *instance);	/* fills a bitfield with zeroes */

void bfsetall(struct bitfield *instance);	/* fills a bitfield with ones */

void bfresize(struct bitfield *instance, int new_size);	/* resizes an existing bitfield */

void bfsetbit(struct bitfield *instance, int bit);	/* sets one bit in a bitfield */

void bfclearbit(struct bitfield *instance, int bit);	/* clears one bit in a bitfield */

struct bitfield *bfrev(const struct bitfield *input);	/* reverses the order of bits in a bitfield and return result in new bitfield */

void bfrev_ip(struct bitfield *instance);	/* reverses the order of bits in a bitfield */

int bfsize(const struct bitfield *instance);	/* obtains the number of bits of a bitfield */

int bfgetbit(const struct bitfield *instance, const int bit);	/* checks whether a bit in a bitfield is set */

void bftogglebit(struct bitfield *instance, const int bit);	/* toggles a bit in a bitfield */

int bfpopcount(const struct bitfield *instance);	/* counts the set bits in a bitfield */

int bfhamming(const struct bitfield *input1, const struct bitfield *input2);	/* counts the Hamming distance between two bitfields */

unsigned long *bf2long(const struct bitfield *input);	/* return the bitfield as an array of unsigned long integers */

int bfisempty(const struct bitfield *instance);	/* checks whether all bits of an array are unset */

int bfpos(const struct bitfield *haystack, const struct bitfield *needle);	/* check whether an array of bits contains a sub-array */

struct bitfield *long2bf(unsigned long *input, int size);	/* write the contents of an array of long integers into a bitfield structure */
