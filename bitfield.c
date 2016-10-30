/**
 * File name: bitfield.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Copyright 2015, 2016
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <endian.h>
#include "bitfield.h"
#include "bitfield-internals.h"

inline void bfcleartail(struct bitfield *instance)
{
	int tail = instance->size % LONG_BIT;
	if (tail != 0) {
		/* create a mask for the tail */
		unsigned long mask = (1UL << tail) - 1UL;
		/* clear the extra bits */
		instance->field[BITNSLOTS(instance->size) - 1] &= mask;
	}
}

inline void _bf_letoh_ip(struct bitfield *instance)
/**
 * convert long integers inside a bitfield from little endian to host.
 * needed after memcpy to bf on big endian machines
 **/
{
	int i;
	for (i = 0; i < BITNSLOTS(bfsize(instance)); i++) {
		if (sizeof(unsigned long) == 4)
			/* 32-bit systems */
			instance->field[i] = le32toh(instance->field[i]);
		else
			/* 64-bit systems */
			instance->field[i] = le64toh(instance->field[i]);
	}
}

inline void _uint16_letoh_ip(uint16_t * input, const int size)
/**
 * convert short integers from little endian to host.
 * needed when memcpy from bitfield to short on big endian machines
 **/
{
	int i;
	for (i = 0; i < size; i++)
		input[i] = le16toh(input[i]);
}

inline void _uint32_letoh_ip(uint32_t * input, const int size)
/**
 * convert integers from little endian to host.
 * needed when memcpy from bitfield to int/long on big endian machines
 **/
{
	int i;
	for (i = 0; i < size; i++) {
		input[i] = le32toh(input[i]);
	}
}

inline void _uint64_letoh_ip(uint64_t * input, const int size)
/**
 * convert integers from little endian to host.
 * needed when memcpy from bitfield to long on big endian machines
 **/
{
	int i;
	for (i = 0; i < size; i++) {
		input[i] = le64toh(input[i]);
	}
}

inline struct bitfield *_bf_htole(const struct bitfield *input)
/**
 * convert long integers inside a bitfield from host to little endian.
 * needed after memcpy from bitfield on big endian machines
 **/
{
	struct bitfield *output = bfclone(input);
	int i;
	for (i = 0; i < BITNSLOTS(bfsize(input)); i++) {
		if (sizeof(unsigned long) == 4)
			/* if long is 4 bits */
			output->field[i] = htole32(input->field[i]);
		else
			/* if long is 8 bits */
			output->field[i] = htole64(input->field[i]);
	}
	return output;
}

inline unsigned short *_short_htole(const unsigned short *input, const int size)
/**
 * convert integers from host to little endian.
 * needed when memcpy from short to bitfield on big endian machines
 **/
{
	int i;
	unsigned short *output = malloc(size * sizeof(unsigned short));
	for (i = 0; i < size; i++) {
		/* assumed to always equal 2 bytes / 16 bits */
		output[i] = htole16(input[i]);
	}
	return output;
}

inline unsigned int *_int_htole(const unsigned int *input, const int size)
/**
 * convert integers from host to little endian.
 * needed when memcpy from int to bitfield on big endian machines
 **/
{
	int i;
	unsigned int *output = malloc(size * sizeof(unsigned int));
	for (i = 0; i < size; i++) {
		if (sizeof(unsigned int) == 2)
			/* if int is 2 bits */
			output[i] = htole16(input[i]);
		else
			/* if int is 4 bits */
			output[i] = htole32(input[i]);
	}
	return output;
}

/*
 * Convert integer data types, all unsigned, to bitfield structures, with
 * in-place equivalents:
 * char as a character (each char storing '0' or '1')
 * char as an integer
 * int
 * long
 */

struct bitfield *str2bf(const char *input)
{
	int input_len = strlen(input);
	struct bitfield *output = bfnew_quick(input_len);
	int bitnslots = BITNSLOTS(input_len);
	int i, j;
	for (i = 0; i < bitnslots - 1; i++) {
		for (j = 0; j < LONG_BIT; j++) {
			if (input[i * LONG_BIT + j] == '1')
				output->field[i] |= (1UL << j);
			else
				output->field[i] &= ~(1UL << j);
		}
	}
	for (j = 0; j < (input_len - 1) % LONG_BIT + 1; j++) {
		if (input[i * LONG_BIT + j] == '1')
			output->field[i] |= (1UL << j);
		else
			output->field[i] &= ~(1UL << j);
	}
	bfcleartail(output);
	return output;
}

struct bitfield *short2bf(const unsigned short *input, int size)
{
	struct bitfield *output;
	if (sizeof(unsigned short) == 2)
		output = uint16tobf((uint16_t *) input, size);
	else
		output = uint32tobf((uint32_t *) input, size);
	return output;
}

struct bitfield *long2bf(const unsigned long *input, int size)
{
	struct bitfield *output;
	if (sizeof(unsigned long) == 4)
		output = uint32tobf((uint32_t *) input, size);
	else
		output = uint64tobf((uint64_t *) input, size);
	return output;
}

struct bitfield *ll2bf(const unsigned long long *input, int size)
{
	struct bitfield *output;
	output = uint64tobf((uint64_t *) input, size);
	return output;
}

struct bitfield *uint8tobf(const uint8_t * input, int size)
{
	struct bitfield *output = bfnew(size);
	int bitnslots = (size - 1) / CHAR_BIT + 1;
	/* order ints in LE, memcpy to bifield, order result in host endian */
	memcpy(output->field, input, bitnslots);
	bf_letoh_ip(output);
	/**
	 * clear the tail, in case bfnew created a bitfield with non-zeroes AND
	 * memcpy did not cover the end of bitfield memory.
	 **/
	bfcleartail(output);
	return output;
}

struct bitfield *uint16tobf(const uint16_t * input, int size)
{
	struct bitfield *output = bfnew(size);
	int bitnslots = (size - 1) / CHAR_BIT + 1;
	/* order ints in LE, memcpy to bifield, order result in host endian */
	memcpy(output->field, uint16_htole(input, bitnslots),
	       bitnslots);
	bf_letoh_ip(output);
	/**
	 * clear the tail, in case bfnew created a bitfield with non-zeroes AND
	 * memcpy did not cover the end of bitfield memory.
	 **/
	bfcleartail(output);
	return output;
}

struct bitfield *uint32tobf(const uint32_t * input, int size)
{
	struct bitfield *output = bfnew(size);
	int bitnslots = (size - 1) / CHAR_BIT + 1;
	/* order ints in LE, memcpy to bifield, order result in host endian */
	memcpy(output->field, uint32_htole(input, bitnslots),
	       bitnslots);
	bf_letoh_ip(output);
	/**
	 * clear the tail, in case bfnew created a bitfield with non-zeroes AND
	 * memcpy did not cover the end of bitfield memory.
	 **/
	bfcleartail(output);
	return output;
}

struct bitfield *uint64tobf(const uint64_t * input, int size)
{
	struct bitfield *output = bfnew(size);
	int bitnslots = (size - 1) / CHAR_BIT + 1;
	/* order ints in LE, memcpy to bifield, order result in host endian */
	memcpy(output->field, uint64_htole(input, bitnslots),
	       bitnslots);
	bf_letoh_ip(output);
	/**
	 * clear the tail, in case bfnew created a bitfield with non-zeroes AND
	 * memcpy did not cover the end of bitfield memory.
	 **/
	bfcleartail(output);
	return output;
}

void str2bf_ip(const char *input, struct bitfield *output)
{
	int input_len =
	    (strlen(input) < output->size) ? strlen(input) : output->size;
	int bitnslots = BITNSLOTS(input_len);
	int i, j;
	for (i = 0; i < bitnslots - 1; i++) {
		for (j = 0; j < LONG_BIT; j++) {
			if (input[i * LONG_BIT + j] == '1')
				output->field[i] |= (1UL << j);
			else
				output->field[i] &= ~(1UL << j);
		}
	}
	for (j = 0; j < (input_len - 1) % LONG_BIT + 1; j++) {
		if (input[i * LONG_BIT + j] == '1')
			output->field[i] |= (1UL << j);
		else
			output->field[i] &= ~(1UL << j);
	}
}

void short2bf_ip(const unsigned short *input, struct bitfield *output)
{
	if (sizeof(unsigned short) == 2)
		uint16tobf_ip((const uint16_t *)input, output);
	else
		uint32tobf_ip((const uint32_t *)input, output);
}

void long2bf_ip(const unsigned long *input, struct bitfield *output)
{
	if (sizeof(unsigned long) == 4)
		uint32tobf_ip((const uint32_t *)input, output);
	else
		uint64tobf_ip((const uint64_t *)input, output);
}

void ll2bf_ip(const unsigned long long *input, struct bitfield *output)
{
	uint64tobf_ip((const uint64_t *)input, output);
}

void uint8tobf_ip(const uint8_t * input, struct bitfield *output)
{
	int size = bfsize(output);
	int bitnslots = (size - 1) / 8 + 1;
	memcpy(output->field, input, bitnslots * sizeof(uint8_t));
	bf_letoh_ip(output);
}

void uint16tobf_ip(const uint16_t * input, struct bitfield *output)
{
	int size = bfsize(output);
	int bitnslots = (size - 1) / 16 + 1;
	/* order ints in LE, memcpy to bifield, order result in host endian */
	memcpy(output->field, uint16_htole(input, bitnslots),
	       bitnslots * sizeof(uint16_t));
	bf_letoh_ip(output);
}

void uint32tobf_ip(const uint32_t * input, struct bitfield *output)
{
	int size = bfsize(output);
	int bitnslots = (size - 1) / 32 + 1;
	/* order ints in LE, memcpy to bifield, order result in host endian */
	memcpy(output->field, uint32_htole(input, bitnslots),
	       bitnslots * sizeof(uint32_t));
	bf_letoh_ip(output);
}

void uint64tobf_ip(const uint64_t * input, struct bitfield *output)
{
	int size = bfsize(output);
	int bitnslots = (size - 1) / 64 + 1;
	/* order ints in LE, memcpy to bifield, order result in host endian */
	memcpy(output->field, uint64_htole(input, bitnslots),
	       bitnslots * sizeof(uint64_t));
	bf_letoh_ip(output);
}

/*
 * Convert bitfield structures to integer data types, all unsigned, with 
 * in-place equivalents:
 * char as a character (each char storing '0' or '1')
 * char as an integer
 * int
 * long
 */

char *bf2str(const struct bitfield *input)
{
	int input_len = input->size;
	char *output = malloc((input_len + 1) * sizeof(char));
	int bitnslots = BITNSLOTS(input_len);
	int i, j;
	for (i = 0; i < bitnslots - 1; i++) {
		for (j = 0; j < LONG_BIT; j++) {
			if ((input->field[i] >> j) & 1LU)
				output[i * LONG_BIT + j] = '1';
			else
				output[i * LONG_BIT + j] = '0';
		}
	}
	for (j = 0; j < (input_len - 1) % LONG_BIT + 1; j++) {
		if ((input->field[bitnslots - 1] >> j) & 1LU)
			output[(bitnslots - 1) * LONG_BIT + j] = '1';
		else
			output[(bitnslots - 1) * LONG_BIT + j] = '0';
	}
	output[input_len] = '\0';
	return output;
}

unsigned short *bf2short(const struct bitfield *input)
{
	unsigned short *output;
	if (sizeof(unsigned short) == 2)
		output = (unsigned short *)bftouint16(input);
	else
		output = (unsigned short *)bftouint32(input);
	return output;
}

unsigned long *bf2long(const struct bitfield *input)
{
	unsigned long *output;
	if (sizeof(unsigned long) == 4)
		output = (unsigned long *)bftouint32(input);
	else
		output = (unsigned long *)bftouint64(input);
	return output;
}

unsigned long long *bf2ll(const struct bitfield *input)
{
	int bitnslots =
	    (input->size - 1) / (sizeof(unsigned long long) * CHAR_BIT) + 1;
	unsigned long long *output =
	    calloc(1, bitnslots * sizeof(unsigned long long));
	memcpy(output, input->field, bitnslots * sizeof(unsigned long long));
	return output;
}

inline uint8_t *bftouint8(const struct bitfield * input)
{
	int bitnslots = (input->size - 1) / CHAR_BIT + 1;
	uint8_t *output = calloc(1, bitnslots * sizeof(uint8_t));
	memcpy(output, bf_htole(input)->field, bitnslots * sizeof(uint8_t));
	return output;
}

inline uint16_t *bftouint16(const struct bitfield * input)
{
	int bitnslots = (input->size - 1) / 16 + 1;
	uint16_t *output = calloc(1, bitnslots * sizeof(uint16_t));
	memcpy(output, bf_htole(input)->field, bitnslots * sizeof(uint16_t));
	return output;
}

inline uint32_t *bftouint32(const struct bitfield * input)
{
	int bitnslots = (input->size - 1) / 32 + 1;
	uint32_t *output = calloc(1, bitnslots * sizeof(uint32_t));
	memcpy(output, bf_htole(input)->field, bitnslots * sizeof(uint32_t));
	return output;
}

inline uint64_t *bftouint64(const struct bitfield * input)
{
	int bitnslots = (input->size - 1) / 64 + 1;
	uint64_t *output = calloc(1, bitnslots * sizeof(uint64_t));
	memcpy(output, bf_htole(input)->field, bitnslots * sizeof(uint64_t));
	return output;
}

void bf2str_ip(const struct bitfield *input, char *output)
{
	int bitnslots = BITNSLOTS(input->size);
	int i, j;
	for (i = 0; i < bitnslots - 1; i++) {
		for (j = 0; j < LONG_BIT; j++) {
			if ((input->field[i] >> j) & 1LU)
				output[i * LONG_BIT + j] = '1';
			else
				output[i * LONG_BIT + j] = '0';
		}
	}
	for (j = 0; j < (input->size - 1) % LONG_BIT + 1; j++) {
		if ((input->field[bitnslots - 1] >> j) & 1LU)
			output[(bitnslots - 1) * LONG_BIT + j] = '1';
		else
			output[(bitnslots - 1) * LONG_BIT + j] = '0';
	}
	output[input->size] = '\0';
}

void bf2short_ip(const struct bitfield *input, unsigned short *output)
{
	if (sizeof(unsigned short) == 2)
		bftouint16_ip(input, (uint16_t *) output);
	else
		bftouint32_ip(input, (uint32_t *) output);
}

void bf2long_ip(const struct bitfield *input, unsigned long *output)
{
	if (sizeof(unsigned long) == 4)
		bftouint32_ip(input, (uint32_t *) output);
	else
		bftouint64_ip(input, (uint64_t *) output);
}

void bf2ll_ip(const struct bitfield *input, unsigned long long *output)
{
	int bitnslots =
	    (input->size - 1) / (sizeof(unsigned long long) * CHAR_BIT) + 1;
	memcpy(output, input->field, bitnslots * sizeof(unsigned long long));
}

inline void bftouint8_ip(const struct bitfield *input, uint8_t * output)
{
	int bitnslots = (input->size - 1) / 8 + 1;
	memcpy(output, bf_htole(input)->field, bitnslots * sizeof(uint8_t));
}

inline void bftouint16_ip(const struct bitfield *input, uint16_t * output)
{
	int bitnslots = (input->size - 1) / 16 + 1;
	/* order bitfield in LE, memcpy to int, order result in host endian */
	memcpy(output, bf_htole(input)->field, bitnslots * sizeof(uint16_t));
	uint16_letoh_ip(output, bitnslots);
}

inline void bftouint32_ip(const struct bitfield *input, uint32_t * output)
{
	int bitnslots = (input->size - 1) / 32 + 1;
	/* order bitfield in LE, memcpy to int, order result in host endian */
	memcpy(output, bf_htole(input)->field, bitnslots * sizeof(uint32_t));
	uint32_letoh_ip(output, bitnslots);
}

inline void bftouint64_ip(const struct bitfield *input, uint64_t * output)
{
	int bitnslots = (input->size - 1) / 64 + 1;
	/* order bitfield in LE, memcpy to int, order result in host endian */
	memcpy(output, bf_htole(input)->field, bitnslots * sizeof(uint64_t));
	uint64_letoh_ip(output, bitnslots);
}

/*
 * Create and delete bitfields
 */

struct bitfield *bfnew(const int size)
{
	struct bitfield *instance = malloc(sizeof(struct bitfield));
	instance->size = size;
	instance->field = calloc(1, BITNSLOTS(size) * sizeof(unsigned long));
	return instance;
}

struct bitfield *bfnew_ones(const int size)
{
	struct bitfield *instance = malloc(sizeof(struct bitfield));
	instance->size = size;
	instance->field = malloc(BITNSLOTS(size) * sizeof(unsigned long));
	bfsetall(instance);
	return instance;
}

struct bitfield *bfnew_quick(const int size)
{
	struct bitfield *instance = malloc(sizeof(struct bitfield));
	instance->size = size;
	instance->field = malloc(BITNSLOTS(size) * sizeof(unsigned long));
	instance->field[BITNSLOTS(size) - 1] = 0UL;	//because the tail should be zeroed anyway
	return instance;
}

void bfdel(struct bitfield *instance)
{
	free(instance->field);
	free(instance);
}

/*
 * Operations with single bits
 */

int bfgetbit(const struct bitfield *instance, const int bit)
{
	/* might be good to check whether bit is within range */

	return BITGET(instance, bit);
}

void bfsetbit(struct bitfield *instance, int bit)
{
	BITSET(instance, bit);
}

void bfclearbit(struct bitfield *instance, int bit)
{
	BITCLEAR(instance, bit);
}

void bftogglebit(struct bitfield *instance, const int bit)
{
	BITTOGGLE(instance, bit);
}

/*
 * Logical operations with bitfields
 */

struct bitfield *bfand(const struct bitfield *input1,
		       const struct bitfield *input2)
{
	/* If the inputs are different size, take the shorter, and ignore the difference.
	 * This way we'll surely have no error.
	 */
	int size = input1->size < input2->size ? input1->size : input2->size;
	int bitnslots = BITNSLOTS(size);
	int i;
	struct bitfield *output = bfnew(size);
	for (i = 0; i < bitnslots; i++)
		output->field[i] = input1->field[i] & input2->field[i];
	/* make sure to clear the trailing bits, if there are any */
	bfcleartail(output);
	return output;
}

struct bitfield *bfnot(const struct bitfield *input)
{
	int bitnslots = BITNSLOTS(input->size);
	int i;
	struct bitfield *output = bfnew(input->size);
	for (i = 0; i < bitnslots; i++)
		output->field[i] = ~input->field[i];
	/* make sure to clear the trailing bits, if there are any */
	bfcleartail(output);
	return output;
}

void bfnot_ip(struct bitfield *instance)
{
	int bitnslots = BITNSLOTS(instance->size);
	int i;
	for (i = 0; i < bitnslots; i++)
		instance->field[i] = ~instance->field[i];
	/* make sure to clear the trailing bits, if there are any */
	bfcleartail(instance);
}

struct bitfield *bfor(const struct bitfield *input1,
		      const struct bitfield *input2)
{
	/* If the inputs are different size, take the shorter, and ignore the difference.
	 * This way we'll surely have no error.
	 */
	int size = input1->size < input2->size ? input1->size : input2->size;
	int bitnslots = BITNSLOTS(size);
	int i;
	struct bitfield *output = bfnew(size);
	for (i = 0; i < bitnslots; i++)
		output->field[i] = input1->field[i] | input2->field[i];
	/* make sure to clear the trailing bits, if there are any */
	bfcleartail(output);
	return output;
}

struct bitfield *bfxor(const struct bitfield *input1,
		       const struct bitfield *input2)
{
	/* If the inputs are different size, take the shorter, and ignore the difference.
	 * This way we'll surely have no error.
	 */
	int size = input1->size < input2->size ? input1->size : input2->size;
	int bitnslots = BITNSLOTS(size);
	int i;
	struct bitfield *output = bfnew(size);
	for (i = 0; i < bitnslots; i++)
		output->field[i] = input1->field[i] ^ input2->field[i];
	/* make sure to clear the trailing bits, if there are any */
	bfcleartail(output);
	return output;
}

/*
 * Manipulate bitfields
 */

inline struct bitfield *__bfcat(const struct bitfield *input1,
				const struct bitfield *input2)
{
	int i;
	int output_size = input1->size + input2->size;
	struct bitfield *output = bfnew(output_size);
	/* copy the first input to output as is */
	for (i = 0; i < BITNSLOTS(input1->size); i++) {
		output->field[i] |= input1->field[i];
	}

	/* find offset bit and offset slot */
	int offset_bit = input1->size % LONG_BIT;
	int offset_slot = input1->size / LONG_BIT;

	for (i = 0; i < BITNSLOTS(input2->size); i++) {
		output->field[i + offset_slot] |=
		    (input2->field[i] << offset_bit);
	}

	/*
	 * If offset_bit is not zero, additional operations are needed.
	 * Number of iterations depends on the nr of slots in output. Two
	 * options:
	 * (a) nr of slots in output is the sum of inputs' slots. In this
	 * case, the nr of bits in the last slot of output is less than the
	 * nr of bits in second input (i.e. ), OR
	 * (b) nr of slots of output is the sum of inputs' slots less one
	 * (i.e. less iterations needed). In this case, the nr of bits in
	 * the last slot of output is greater than the nr of bits in second
	 * input.
	 * If offset_bit is zero, no additional copies needed.
	 */
	if ((output_size - 1) % LONG_BIT < (input2->size - 1) % LONG_BIT) {
		for (i = 0; i < BITNSLOTS(input2->size); i++) {
			output->field[i + offset_slot + 1] |= (input2->field[i] >> (LONG_BIT - offset_bit));
		}
	}
	else if ((output_size - 1) % LONG_BIT > (input2->size - 1) % LONG_BIT) {
		for (i = 0; i < (BITNSLOTS(input2->size) - 1); i++) {
			output->field[i + offset_slot + 1] |= (input2->field[i] >> (LONG_BIT - offset_bit));
		}
	}
	return output;
}

struct bitfield *_bfcat(int count, ...)
{
	int i;
	va_list args;
	va_start(args, count);
	struct bitfield *output = bfclone(va_arg(args, struct bitfield *));
	for (i = 1; i < count; i++) {

		struct bitfield *tmp =
		    __bfcat(output, va_arg(args, struct bitfield *));
		/* reassign *output to point to new struct without leaking memory */
		free(output->field);
		*output = *tmp;
		free(tmp);
	}
	va_end(args);
	return output;
}

inline int count_arguments(char *s)
{
	unsigned i, argc = 1;
	for (i = 0; s[i]; i++)
		if (s[i] == ',')
			argc++;
	return argc;
}

void bfclearall(struct bitfield *instance)
{
	int i;
	for (i = 0; i < BITNSLOTS(instance->size); i++)
		instance->field[i] = 0UL;
}

struct bitfield *bfclone(const struct bitfield *input)
{
	int bitnslots = BITNSLOTS(input->size);
	/* not using bfnew, because calloc is slow and 0-ed memory not needed anyway */
	struct bitfield *output = malloc(sizeof(struct bitfield));
	output->size = input->size;
	output->field = malloc(bitnslots * sizeof(unsigned long));
	memcpy(output->field, input->field, bitnslots * sizeof(unsigned long));
	return output;
}

int bfcmp(const struct bitfield *input1, const struct bitfield *input2,
	  char **errmsg)
{
	int retcode = 0;
	char *msg;
	if (input1->size != input2->size) {
		msg = "Can not compare bitfields of different length";
		retcode = 2;
		goto error;
	}

	int i;
	int bitnslots = BITNSLOTS(input1->size);
	for (i = 0; i < (bitnslots - 1); i++) {	//leaving the last slot for later
		if (input1->field[i] != input2->field[i]) {
			msg = "Bitfields differ";
			retcode = 1;
			goto error;
		}
	}
	/* comparing the last slot using mask, because the tail bits may differ */
	unsigned long mask =
	    ((input1->size % LONG_BIT) ==
	     0) ? -1UL : (1UL << input1->size % LONG_BIT) - 1UL;
	if ((input1->field[(input1->size - 1) / LONG_BIT] & mask) !=
	    ((input2->field)[(input2->size - 1) / LONG_BIT] & mask)) {
		msg = "Bitfields differ";
		retcode = 1;
		goto error;
	}
	return 0;
 error:
	if (errmsg) {
		*errmsg = malloc(strlen(msg) + 1);
		memcpy(*errmsg, msg, strlen(msg) + 1);
	}
	return retcode;
}

int bfcpy(const struct bitfield *src, struct bitfield *dest)
{
	if (src->size > dest->size)
		return 1;
	int i;
	for (i = 0; i < BITNSLOTS(src->size); i++)
		dest->field[i] = src->field[i];
	return 0;
}

int bfhamming(const struct bitfield *input1, const struct bitfield *input2)
{
	struct bitfield *input_xored = bfxor(input1, input2);
	int hamming = bfpopcount(input_xored);
	bfdel(input_xored);
	return hamming;
}

int bfisempty(const struct bitfield *instance)
{
	int i;
	int bitnslots = BITNSLOTS(instance->size);
	for (i = 0; i < bitnslots; i++) {
		if (instance->field[i] != 0UL)
			return 1;
	}
	return 0;
}

int bfpopcount(const struct bitfield *instance)
{
	int bits = 0;
	int i;
	for (i = 0; i < BITNSLOTS(instance->size); i++)
		/* this is GCC and Clang only */
		bits += __builtin_popcountl(instance->field[i]);
	return bits;
}

int bfpos(const struct bitfield *haystack, const struct bitfield *needle)
{
	int heystack_size = bfsize(haystack);
	int needle_size = bfsize(needle);
	int i;
	int diff = heystack_size - needle_size;
	int result;

	/* needle longer than haystack; return error code -2 */
	if (diff < 0) {
		return -2;
	}
	for (i = 0; i <= diff; i++) {
		struct bitfield *needle_candidate =
		    bfsub(haystack, i, needle_size + i);
		result = bfcmp(needle_candidate, needle, NULL);
		bfdel(needle_candidate);
		/* needle matches a sub-array of haystack; return starting position of the sub-array */
		if (result == 0) {
			return i;
		}
	}
	/* nothing matched; return -1 */
	return -1;
}

void bfprint_lsb(const struct bitfield *instance)
{
	int i;
	for (i = 0; i < instance->size; i++)
		printf("%lu",
		       (instance->field[i / LONG_BIT] >> (i % LONG_BIT)) & 1UL);
	/* maybe it would be quicker to convert bitfield to string (bf2str) and print it all at once */
}

void bfprint_msb(const struct bitfield *instance)
{
	int i;
	for (i = instance->size - 1; i >= 0; i--)
		printf("%lu",
		       (instance->field[i / LONG_BIT] >> (i % LONG_BIT)) & 1UL);
}

void bfresize(struct bitfield *instance, int new_size)
{
	int old_bitnslots = BITNSLOTS(instance->size);
	int new_bitnslots = BITNSLOTS(new_size);
	instance->size = new_size;
	void *tmp;
	tmp = realloc(instance->field, new_bitnslots * sizeof(unsigned long));
	if (tmp != NULL)
		instance->field = tmp;
	if ((new_bitnslots < old_bitnslots) & (new_size % LONG_BIT != 0))
		bfcleartail(instance);
}

void bfrev_ip(struct bitfield *instance)
{
	int size = instance->size;
	struct bitfield *tmp = bfnew_quick(size);
	int i, j;
	int bitnslots = BITNSLOTS(size);
	for (i = 0; i < (bitnslots - 1); i++) {
		for (j = 0; j < LONG_BIT; j++) {
			if ((instance->field[i] >> j) & 1UL)
				BITSET(tmp, size - i * LONG_BIT - j - 1);
			else
				BITCLEAR(tmp, size - i * LONG_BIT - j - 1);
		}
	}
	int bits_in_last_input_slot = (size - 1) % LONG_BIT + 1;
	for (j = 0; j < bits_in_last_input_slot; j++) {
		if ((instance->field[bitnslots - 1] >> j) & 1UL)
			BITSET(tmp, bits_in_last_input_slot - j - 1);
		else
			BITCLEAR(tmp, bits_in_last_input_slot - j - 1);
	}
	memcpy(instance->field, tmp->field, bitnslots * sizeof(unsigned long));
	bfdel(tmp);
}

struct bitfield *bfrev(const struct bitfield *input)
{
	int size = input->size;
	int bitnslots = BITNSLOTS(size);
	int i;
	struct bitfield *output = bfnew_quick(size);
	for (i = 0; i < bitnslots; i++) {
		/* taken from http://graphics.stanford.edu/~seander/bithacks.html#BitReverseObvious */
		unsigned long v = input->field[bitnslots - i - 1];	// the source slot to be reversed
		output->field[i] = v;	// the destination slot; will be reversed bits of v; first get LSB of v
		int s = LONG_BIT - 1;	// extra shift needed at end
		for (v >>= 1; v; v >>= 1) {
			output->field[i] <<= 1UL;
			output->field[i] |= v & 1UL;
			s--;
		}
		output->field[i] <<= s;	// shift when v's highest bits are zero
	}
	int tail = bitnslots * LONG_BIT - size;
	output->size = bitnslots * LONG_BIT;
	bfshift_ip(output, -tail);
	output->size = size;
	return output;
}

void bfsetall(struct bitfield *instance)
{
	int i;
	for (i = 0; i < BITNSLOTS(instance->size); i++)
		instance->field[i] = -1UL;
	bfcleartail(instance);
}

void bfshift_ip(struct bitfield *input, const int offset)
{
	if (input->size <= 1) {
		return;		/* input too short to shift */
	}
	/* positive offset moves the last offset characters to the beginning */
	/* removing extra rotations */
	int offset_internal = offset % input->size;
	if (offset_internal == 0) {
		return;		/* no need to shift */
	}
	/* changing a negative offset to a positive equivalent */
	if (offset < 0)
		offset_internal += input->size;
	struct bitfield *first_chunk =
	    bfsub(input, 0, input->size - offset_internal);
	struct bitfield *second_chunk =
	    bfsub(input, input->size - offset_internal, input->size);

	struct bitfield *tmp = bfcat(second_chunk, first_chunk);
	free(input->field);
	*input = *tmp;
	free(tmp);
	bfdel(first_chunk);
	bfdel(second_chunk);
}

struct bitfield *bfshift(const struct bitfield *input, const int offset)
{
	/* positive offset moves the last offset characters to the beginning */
	int bitnslots = BITNSLOTS(input->size);
	/* removing extra rotations */
	int offset_internal = offset % input->size;
	if (offset_internal == 0) {
		struct bitfield *output = bfnew(input->size);
		memcpy(output->field, input->field,
		       bitnslots * sizeof(unsigned long));
		return output;	/* nothing to shift */
	}
	/* changing a negative offset to a positive equivalent */
	if (offset < 0)
		offset_internal += input->size;
	struct bitfield *first_chunk =
	    bfsub(input, 0, input->size - offset_internal);
	struct bitfield *second_chunk =
	    bfsub(input, input->size - offset_internal, input->size);
	struct bitfield *output = bfcat(second_chunk, first_chunk);
	bfdel(first_chunk);
	bfdel(second_chunk);
	return output;
}

int bfsize(const struct bitfield *instance)
{
	return instance->size;
}

struct bitfield *bfsub(const struct bitfield *input, const unsigned int start,
		       const unsigned int end)
{
    /**
     * this function will extract a sub-bitfield, starting from 'start' bit (included in subfield) and to 'end' bit (not included)
     * E.g. rule30_subfield(input, 5, 30) will return a bitfield 25 bits long, starting from the 5-th bit.
     * All bit counts start from 0.
     **/
    /** all kinds of checks (start < end, end <= input->field etc.) should be here **/
	struct bitfield *output = bfnew(end - start);

	int start_offset = start % LONG_BIT;	//position of start bit inside slot
	int end_offset = (end - 1) % LONG_BIT;
	int start_slot = start / LONG_BIT;
	int end_slot = (end - 1) / LONG_BIT;	// the slot storing the last bit included into subfield
	int output_slots = BITNSLOTS(end - start);
//    unsigned_long mask_1 = (start_offset = 0) ? -1UL : (1UL << (LONG_BIT - offset)) - 1UL;

	int i;
    /** filling output slots except the last one, because it may be underfull **/
	if (output_slots > 1) {
		if (start_offset != 0) {	// need to move them one by one
			for (i = 0; i < output_slots - 1; i++) {
				output->field[i] =
				    input->field[start_slot +
						 i] >> start_offset;
				output->field[i] |=
				    input->field[start_slot + i +
						 1] << (LONG_BIT -
							start_offset);
			}
		} else
			memcpy(output->field, (input->field) + start_slot, (output_slots - 1) * sizeof(unsigned long));	// the easiest case
	}

    /** filling the last output slot
     *  Three cases here:
     *  1. the data for the last slot in output are stretched over 2 slots in input
     *  This can be tested by start_offset > end_offset. In this case we need to
     *  shift+copy the ending part of the last-but-one slotş then to separate the
     *  first end_offset bits from the last slot and move them to the end of last
     *  slot in output.
     *  2. the data for the output are aligned to bit 0. Tested by start_offset = 0.
     *  The easiest case, treated above.
     *  3. the data for the last slot of output are in the middle of the last
     *  slot in input. Make a mask with end_offset-start_offset bits. Move them
     *  start_offset bits to the right. Compare with the last slot in input. Shift
     *  to the beginning of the last slot in input (i.e. start_offset bits to left).
 **/
	if (start_offset > end_offset) {
		output->field[(end - start - 1) / LONG_BIT] =
		    (input->field)[end_slot - 1] >> start_offset;
		unsigned long mask = (1UL << (end_offset + 1)) - 1UL;	// mask cannot be full, so no need to check
		output->field[(end - start - 1) / LONG_BIT] |=
		    ((input->field)[end_slot] & mask) << (LONG_BIT -
							  start_offset);
	} else if (start_offset == 0) {
		unsigned long mask =
		    (end_offset ==
		     LONG_BIT - 1) ? -1UL : (1UL << (end_offset + 1)) - 1UL;
		output->field[(end - start - 1) / LONG_BIT] =
		    (input->field[end_slot] & mask);
	} else {
		unsigned long mask = (1UL << (end_offset - start_offset + 1)) - 1UL;	/* +1, because end_offset is the bit _included_ in selection */
		output->field[(end - start - 1) / LONG_BIT] |=
		    ((input->field)[end_slot] & (mask << start_offset)) >>
		    start_offset;
	}
	return output;
}

struct bitfield *bfnormalize(const struct bitfield *input)
{
	int size = input->size;
	int bitnslots = BITNSLOTS(size);
	int length_last_chunk = size % LONG_BIT;
	/* pick first potential candidate for output */
	struct bitfield *output = bfclone(input);
	/* will compare bitfields in chunks of length 1 unsigned long */
	unsigned long chunk_a, chunk_b;
	struct bitfield *tmp1, *tmp2;
	/* counters for bit offsets and slots/chunks/longs */
	int i, j;
	/* shift input string 1 position at a time and compare with best candidate (size - 1 comparisons) */
	for (i = 1; i <= size - 1; i++) {
		/* compare 1 slot/long at a time */
		for (j = bitnslots - 1; j >= 0; j--) {
			/* special check for tail chunks (may be underfull) */
			if (j == bitnslots - 1 && length_last_chunk != 0) {
				/* this can probably be optimized */
				chunk_a = output->field[j];
				tmp1 = bfshift(input, length_last_chunk + i);
				tmp2 = bfsub(tmp1, 0, length_last_chunk);
				chunk_b = tmp2->field[0];
				bfdel(tmp1);
				bfdel(tmp2);
			} else {
				/* this can probably be optimized */
				chunk_a = output->field[j];
				tmp1 = bfshift(input, i);
				chunk_b = tmp1->field[j];
				bfdel(tmp1);
			}
			/* compare. if a is greater, offset i becomes new best candidate. move to next i */
			if (chunk_a > chunk_b) {
				tmp1 = bfshift(input, i);
				free(output->field);
				*output = *tmp1;
				free(tmp1);
				break;
			}
			/* if a is smaller, move to next offset */
			else if (chunk_a < chunk_b) {
				break;
			}
			/* if equal, compare next chunk (i.e. do not break out of j loop) */
		}
	}
	return output;
}
