/**
 * File name: bitfield.h
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Copyright 2015, 2016
**/

#include <stdint.h>

#ifndef HAVE_BITFIELD_H
#define HAVE_BITFIELD_H

struct bitfield;

/*
 * Convert integer data types, all unsigned, to bitfield structures, with
 * in-place equivalents:
 * char as a character (each char storing '0' or '1')
 * char as an integer
 * int
 * long
 */

struct bitfield *str2bf(const char *input);	/* converts a character string of ones and zeroes into a bitfield and returns the result in a new bitfield */

#define char2bf(X, Y) (struct bitfield *) uint8tobf((const uint8_t *) X, Y)	/* write the contents of an array of chars into a bitfield structure */

struct bitfield *short2bf(const unsigned short *input, int size); /* write the contents of an array of short integers into a bitfield structure */

#define int2bf(X, Y) (struct bitfield *) uint32tobf((const uint32_t *) X, Y) /* write the contents of an array of integers into a bitfield structure */

struct bitfield *long2bf(const unsigned long *input, int size); /* write the contents of an array of long integers into a bitfield structure */

struct bitfield *ll2bf(const unsigned long long *input, int size);	/* write the contents of an array of longlong integers into a bitfield structure */

struct bitfield *uint8tobf(const uint8_t *input, int size);

struct bitfield *uint16tobf(const uint16_t *input, int size);

struct bitfield *uint32tobf(const uint32_t *input, int size);

struct bitfield *uint64tobf(const uint64_t *input, int size);

void str2bf_ip(const char *input, struct bitfield *output);	/* converts a character string of ones and zeroes into a bitfield */

#define char2bf_ip(X, Y) uint8tobf_ip((const uint8_t *) X, Y) /* convert an array of chars into a bitfield structure */

void short2bf_ip(const unsigned short *intput, struct bitfield *output); /* convert an array of short integers into a bitfield structure */

#define int2bf_ip(X, Y) uint32tobf_ip((const uint32_t *) X, Y) /* convert an array of integers into a bitfield structure */

void long2bf_ip(const unsigned long *input, struct bitfield *output); /* convert an array of long integers into a bitfield structure */

void ll2bf_ip(const unsigned long long *input, struct bitfield *output); /* convert an array of longlong integers into a bitfield structure */

void uint8tobf_ip(const uint8_t *input, struct bitfield *output);

void uint16tobf_ip(const uint16_t *input, struct bitfield *output);

void uint32tobf_ip(const uint32_t *input, struct bitfield *output);

void uint64tobf_ip(const uint64_t *input, struct bitfield *output);

/*
 * Convert bitfield structures to integer data types, all unsigned, with 
 * in-place equivalents:
 * char as a character (each char storing '0' or '1')
 * char as an integer
 * int
 * long
 */

char *bf2str(const struct bitfield *input);	/* converts a bitfield into a character string of ones and zeroes and returns the result in a new character string */

#define bf2char(X) (unsigned char *) bftouint8(X)	/* return the bitfield as an array of unsigned chars */

unsigned short *bf2short(const struct bitfield *input); /* return the bitfield as an array of unsigned short integers */

#define bf2int(X) (unsigned int *) bftouint32(X)	/* return the bitfield as an array of unsigned integers */

unsigned long *bf2long(const struct bitfield *input); /* return the bitfield as an array of unsigned long integers */

unsigned long long *bf2ll(const struct bitfield *input);	/* return the bitfield as an array of unsigned longlong integers */

uint8_t *bftouint8(const struct bitfield *input);

uint16_t *bftouint16(const struct bitfield *input);

uint32_t *bftouint32(const struct bitfield *input);

uint64_t *bftouint64(const struct bitfield *input);

void bf2str_ip(const struct bitfield *input, char *output);	/* converts a bitfield into a character string of ones and zeroes */

#define bf2char_ip(X, Y) bftouint8_ip(X, (uint8_t *) Y)	/* converts a bitfield into an array of unsigned chars */

void bf2short_ip(const struct bitfield *input, unsigned short *output); /* converts a bitfield into an array of unsigned short integers */

#define bf2int_ip(X, Y) bftouint32_ip(X, (uint32_t *) Y)	/* converts a bitfield into an array of unsigned integers */

void bf2long_ip(const struct bitfield *input, unsigned long *output);	/* converts a bitfield into an array of unsigned long integers */

void bf2ll_ip(const struct bitfield *input, unsigned long long *output);	/* converts a bitfield into an array of unsigned longlong integers */

void bftouint8_ip(const struct bitfield *input, uint8_t *output);

void bftouint16_ip(const struct bitfield *input, uint16_t *output);

void bftouint32_ip(const struct bitfield *input, uint32_t *output);

void bftouint64_ip(const struct bitfield *input, uint64_t *output);

/*
 * Create and delete bitfields
 */

struct bitfield *bfnew_ones(const int size);	/* creates a bitfield structure, sets all its bits to true with and returns a pointer to it */

struct bitfield *bfnew_quick(const int size);	/* creates a bitfield structure and returns a pointer to it */

struct bitfield *bfnew(const int size);	/* creates a bitfield structure, sets all its bits to false and returns a pointer to it */

void bfdel(struct bitfield *instance);	/* destroys a bitfield structure and frees memory */

/*
 * Operations with single bits
 */

int bfgetbit(const struct bitfield *instance, const int bit);	/* checks whether a bit in a bitfield is set */

void bfsetbit(struct bitfield *instance, int bit);	/* sets one bit in a bitfield */

void bfclearbit(struct bitfield *instance, int bit);	/* clears one bit in a bitfield */

void bftogglebit(struct bitfield *instance, const int bit);	/* toggles a bit in a bitfield */

/*
 * Logical operations with bitfields
 */

struct bitfield *bfand(const struct bitfield *input1, const struct bitfield *input2);	/* performs bitwise AND over a pair of bitfields */

struct bitfield *bfnot(const struct bitfield *input);	/* reverses all bits in a bitfield and return the result in new bitfield */

void bfnot_ip(struct bitfield *instance);	/* reverses all bits in a bitfield */

struct bitfield *bfor(const struct bitfield *input1, const struct bitfield *input2);	/* performs bitwise inclusive OR over a pair of bitfields */

struct bitfield *bfxor(const struct bitfield *input1, const struct bitfield *input2);	/* performs bitwise exclusive OR over a pair of bitfields */

/*
 * Manipulate bitfields
 */
struct bitfield * _bfcat(int count, ...);

int count_arguments(char *s);

#define bfcat(...) (struct bitfield *) _bfcat( count_arguments(#__VA_ARGS__), __VA_ARGS__)	/* concatenates two bitfields into one */

void bfclearall(struct bitfield *instance);	/* fills a bitfield with zeroes */

struct bitfield *bfclone(const struct bitfield *input);	/* creates a copy of an existing bitfield */

int bfcmp(const struct bitfield *input1, const struct bitfield *input2, char **errmsg);	/* compares two bitfields and returns 0 if same or non-zero and error message if different */

int bfcpy(const struct bitfield *src, struct bitfield *dest);	/* copies the contents of a bitfield into another pre-existing bitfield */

int bfhamming(const struct bitfield *input1, const struct bitfield *input2);	/* counts the Hamming distance between two bitfields */

int bfisempty(const struct bitfield *instance);	/* checks whether all bits of an array are unset */

struct bitfield *bfnormalize(const struct bitfield *input);	/* treats the bitfield as a closed ring and represents it as a smallest value */

int bfpopcount(const struct bitfield *instance);	/* counts the set bits in a bitfield */

int bfpos(const struct bitfield *haystack, const struct bitfield *needle);	/* check whether an array of bits contains a sub-array */

void bfprint_lsb(const struct bitfield *instance);	/* prints a bitfield as a series of ones and zeroes, least significant bit first */

void bfprint_msb(const struct bitfield *instance);	/* prints a bitfield as a series of ones and zeroes, most significant bit first */

void bfresize(struct bitfield *instance, int new_size);	/* resizes an existing bitfield */

void bfrev_ip(struct bitfield *instance);	/* reverses the order of bits in a bitfield */

struct bitfield *bfrev(const struct bitfield *input);	/* reverses the order of bits in a bitfield and return result in new bitfield */

void bfsetall(struct bitfield *instance);	/* fills a bitfield with ones */

void bfshift_ip(struct bitfield *input, const int offset);	/* circular-shifts the contents of a bitfield */

struct bitfield *bfshift(const struct bitfield *input, const int offset);	/* circular-shifts the contents of a bitfield and return the result in new bitfield */

int bfsize(const struct bitfield *instance);	/* obtains the number of bits of a bitfield */

struct bitfield *bfsub(const struct bitfield *input, const unsigned int start, const unsigned int end);	/* extracts a sub-bitfield from a bitfield */

#endif
