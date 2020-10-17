/**
 * File name: bitfield.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3 or later
 * Copyright 2015, 2016, 2017
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>		//for ffs()
#include <stdarg.h>
#include <assert.h>
#include <endian.h>
#include "config.h"
#include "bitfield.h"
#include "bitfield-internals.h"

#if __BYTE_ORDER == __BIG_ENDIAN
/* big-endian systems */

/* big-endian-specific macros */

#define bf_letoh_ip(x) bf_letoh_ip_(x)
#define uint16_letoh_ip(x, y) uint16_letoh_ip_(x, y)
#define uint32_letoh_ip(x, y) uint32_letoh_ip_(x, y)
#define uint64_letoh_ip(x, y) uint64_letoh_ip_(x, y)
#define bf_htole(x) (struct bitfield *) bf_htole_(x)
#define uint16_htole_ip(x, y) uint16_htole_ip_(x, y)
#define uint32_htole_ip(x, y) uint32_htole_ip_(x, y)
#define uint64_htole_ip(x, y) uint64_htole_ip_(x, y)
#define uint64tobf_(x, y, z) uint64tobf_be(x, y, z)

/* big-endian-specific function declarations */

static inline void bf_letoh_ip_(struct bitfield *instance);
static inline void uint16_letoh_ip_(uint16_t * input, const unsigned int size);
static inline void uint32_letoh_ip_(uint32_t * input, const unsigned int size);
static inline void uint64_letoh_ip_(uint64_t * input, const unsigned int size);
static inline struct bitfield *bf_htole_(const struct bitfield *input);
static inline void uint16_htole_ip_(uint16_t * input, const unsigned int size);
static inline void uint32_htole_ip_(uint32_t * input, const unsigned int size);
static inline void uint64_htole_ip_(uint64_t * input, const unsigned int size);
static inline void uint64tobf_be(const uint64_t * input,
				 struct bitfield *output, unsigned int size);

/* big-endian-specific function definitions */

static inline void bf_letoh_ip_(struct bitfield *instance)
/**
 * convert long integers inside a bitfield from little endian to host.
 * needed after memcpy to bf on big endian machines
 **/
{
	if (sizeof(unsigned long) == 4)
		/* 32-bit systems */
		uint32_letoh_ip((uint32_t *) instance->field,
				BITNSLOTS(bfsize(instance)));
	else
		/* 64-bit systems */
		uint64_letoh_ip((uint64_t *) instance->field,
				BITNSLOTS(bfsize(instance)));
}

static inline void _uint16_letoh_ip(uint16_t * input, const unsigned int size)
/**
 * convert short integers from little endian to host.
 * needed when memcpy from bitfield to short on big endian machines
 * Used in bftouint16() and bftouint16_ip().
 **/
{
	unsigned int i;
	for (i = 0; i < size; i++)
		input[i] = le16toh(input[i]);
}

static inline void uint32_letoh_ip_(uint32_t * input, const unsigned int size)
/**
 * convert integers from little endian to host.
 * needed when memcpy from bitfield to int/long on big endian machines
 * Used in bftouint32() and bftouint32_ip()
 **/
{
	unsigned int i;
	for (i = 0; i < size; i++) {
		input[i] = le32toh(input[i]);
	}
}

static inline void uint64_letoh_ip_(uint64_t * input, const unsigned int size)
/**
 * convert integers from little endian to host.
 * needed when memcpy from bitfield to long on big endian machines
 * Used in bftouint64() and bftouint64_ip()
 **/
{
	unsigned int i;
	for (i = 0; i < size; i++) {
		input[i] = le64toh(input[i]);
	}
}

static inline struct bitfield *bf_htole_(const struct bitfield *input)
/**
 * convert long integers inside a bitfield from host to little endian.
 * needed after memcpy from bitfield on big endian machines
 * Used ad bf_htole() in big-endian architectures
 **/
{
	struct bitfield *output = bfclone(input);
	if (!output)
		return NULL;
	unsigned int i;
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

static inline void uint16_htole_ip_(uint16_t * input, const unsigned int size)
/**
 * write something here!!!
 **/
{
	unsigned int i;
	for (i = 0; i < size; i++)
		input[i] = htole16(input[i]);
}

static inline void uint32_htole_ip_(uint32_t * input, const unsigned int size)
/**
 * write something here!!!
 **/
{
	unsigned int i;
	for (i = 0; i < size; i++)
		input[i] = htole32(input[i]);
}

static inline void uint64_htole_ip_(uint64_t * input, const unsigned int size)
/**
 * write something here!!!
 **/
{
	unsigned int i;
	for (i = 0; i < size; i++)
		input[i] = htole64(input[i]);
}

static inline void uint64tobf_be(const uint64_t * input,
				 struct bitfield *output, unsigned int size)
{
	memcpy(output->field, input, ((size - 1) / 64 + 1) * sizeof(uint64_t));
	uint64_htole_ip((uint64_t *) output->field, (size - 1) / 64 + 1);
	bf_letoh_ip(output);
	bfresize(output, size);
}

#else
/* little-endian systems (mixed endians?) */
#define bf_letoh_ip(x)
#define uint16_letoh_ip(x, y)
#define uint32_letoh_ip(x, y)
#define uint64_letoh_ip(x, y)
#define bf_htole(x) x
#define uint16_htole_ip(x, y)
#define uint32_htole_ip(x, y)
#define uint64_htole_ip(x, y)
#define uint64tobf_(x, y, z) uint64tobf_le(x, y, z)

static inline void uint64tobf_le(const uint64_t * input,
				 struct bitfield *output, unsigned int size)
{
	memcpy(output->field, input, (size - 1) / CHAR_BIT + 1);
}

#endif

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
	unsigned int input_len = strlen(input);
	struct bitfield *output = bfnew_quick(input_len);
	if (!output)
		return NULL;
	unsigned int bitnslots = BITNSLOTS(input_len);
	unsigned int i, j;
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

struct bitfield *short2bf(const unsigned short *input, unsigned int size)
{
	struct bitfield *output = NULL;
	if (sizeof(unsigned short) == 2)
		output = uint16tobf((uint16_t *) input, size);
	else if (sizeof(unsigned short) == 4)
		output = uint32tobf((uint32_t *) input, size);
	//else ?
	return output;
}

struct bitfield *long2bf(const unsigned long *input, unsigned int size)
{
	struct bitfield *output = bfnew_quick(size);
	if (!output)
		return NULL;
	memcpy(output->field, input, BITNSLOTS(size) * sizeof(unsigned long));
	return output;
}

struct bitfield *uint8tobf(const uint8_t * input, unsigned int size)
{
	struct bitfield *output = bfnew(size);
	if (!output)
		return NULL;
	int nr_bytes = (size - 1) / CHAR_BIT + 1;
	/* order ints in LE, memcpy to bifield, order result in host endian */
	memcpy(output->field, input, nr_bytes);
	bf_letoh_ip(output);
	return output;
}

struct bitfield *uint16tobf(const uint16_t * input, unsigned int size)
{
	struct bitfield *output = bfnew(size);
	if (!output)
		return NULL;
	memcpy(output->field, input, ((size - 1) / 16 + 1) * sizeof(uint16_t));
	uint16_htole_ip((uint16_t *) output->field, (size - 1) / 16 + 1);
	bf_letoh_ip(output);
	/**
	 * clear the tail, in case bfnew created a bitfield with non-zeroes AND
	 * memcpy did not cover the end of bitfield memory.
	 **/
	bfcleartail(output);
	return output;
}

struct bitfield *uint32tobf(const uint32_t * input, unsigned int size)
{
	struct bitfield *output = bfnew(size);
	if (!output)
		return NULL;
	memcpy(output->field, input, ((size - 1) / 32 + 1) * sizeof(uint32_t));
	if (sizeof(unsigned long) != sizeof(uint32_t)) {
		uint32_htole_ip((uint32_t *) output->field,
				(size - 1) / 32 + 1);
		bf_letoh_ip(output);
	}
	/**
	 * clear the tail, in case bfnew created a bitfield with non-zeroes AND
	 * memcpy did not cover the end of bitfield memory.
	 **/
	bfcleartail(output);
	return output;
}

struct bitfield *uint64tobf(const uint64_t * input, unsigned int size)
{
	/**
	 * if sizeof long equals 64 bits, copy as is on any architecture
	 * if sizeof long is less than 64 bits AND big-endian, than
	 * copy to a non-const array, swap to little-endian, copy the needed part and 
	 * swap the result back to big-endian
	 * if sizeof long is less than 64 bits AND little-endian, then just copy the needed part
	**/
	struct bitfield *output;
	if (sizeof(unsigned long) == sizeof(uint64_t)) {
		output = long2bf((unsigned long *)input, size);
		if (!output)
			return NULL;
	} else {
		output =
		    bfnew(((size - 1) / 64 + 1) * sizeof(uint64_t) * CHAR_BIT);
		if (!output)
			return NULL;
		uint64tobf_(input, output, size);
	}
	/**
	 * clear the tail, in case bfnew created a bitfield with non-zeroes AND
	 * memcpy did not cover the end of bitfield memory.
	 **/
	bfcleartail(output);
	return output;
}

void str2bf_ip(const char *input, struct bitfield *output)
{
	unsigned int input_len =
	    (strlen(input) < output->size) ? strlen(input) : output->size;
	unsigned int bitnslots = BITNSLOTS(input_len);
	unsigned int i, j;
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
	else if (sizeof(unsigned short) == 4)
		uint32tobf_ip((const uint32_t *)input, output);
	//else ?
}

void long2bf_ip(const unsigned long *input, struct bitfield *output)
{
	memcpy(output->field, input,
	       BITNSLOTS(output->size) * sizeof(unsigned long));
}

void uint8tobf_ip(const uint8_t * input, struct bitfield *output)
{
	int size = bfsize(output);
	int nr_bytes = (size - 1) / CHAR_BIT + 1;
	memcpy(output->field, input, nr_bytes);
	bf_letoh_ip(output);
}

void uint16tobf_ip(const uint16_t * input, struct bitfield *output)
{
	memcpy(output->field, input,
	       ((output->size - 1) / 16 + 1) * sizeof(uint16_t));
	uint16_htole_ip((uint16_t *) output->field,
			(output->size - 1) / 16 + 1);
	bf_letoh_ip(output);
}

void uint32tobf_ip(const uint32_t * input, struct bitfield *output)
{
	memcpy(output->field, input,
	       ((output->size - 1) / 32 + 1) * sizeof(uint32_t));
	if (sizeof(unsigned long) != sizeof(uint32_t)) {
		uint32_htole_ip((uint32_t *) output->field,
				(output->size - 1) / 32 + 1);
		bf_letoh_ip(output);
	}
}

void uint64tobf_ip(const uint64_t * input, struct bitfield *output)
{
	int long_slots =
	    (output->size - 1) / (sizeof(unsigned long) * CHAR_BIT) + 1;
	int uint64_slots = (output->size - 1) / 64 + 1;
/* rewrite for bigendian-only */
	if (sizeof(unsigned long) != sizeof(uint64_t)) {
		struct bitfield *tmp =
		    bfnew(uint64_slots * sizeof(uint64_t) * CHAR_BIT);
		memcpy(tmp->field, input, uint64_slots * sizeof(uint64_t));
		uint64_htole_ip((uint64_t *) tmp->field,
				(output->size - 1) / 64 + 1);
		bf_letoh_ip(tmp);
		memcpy(output->field, tmp->field,
		       long_slots * sizeof(unsigned long));
		bfdel(tmp);
	} else {
		memcpy(output->field, input, uint64_slots * sizeof(uint64_t));
	}
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
	unsigned int input_len = input->size;
	char *output = malloc(input_len + 1);
	if (!output)
		return NULL;
	unsigned int bitnslots = BITNSLOTS(input_len);
	unsigned int i, j;
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
	int i;
	int bitnslots = BITNSLOTS(input->size);
	unsigned long *output = malloc(bitnslots * sizeof(unsigned long));
	if (!output)
		return NULL;
	for (i = 0; i < bitnslots; i++)
		output[i] = input->field[i];
	return output;
}

inline uint8_t *bftouint8(const struct bitfield *input)
{
	int nr_bytes = (input->size - 1) / CHAR_BIT + 1;
	uint8_t *output = calloc(1, nr_bytes);
	if (!output)
		return NULL;
#if __BYTE_ORDER == __BIG_ENDIAN
	struct bitfield *tmp = bf_htole(input);
	if (!tmp) {
		free(output);
		return NULL;
	}
	memcpy(output, tmp->field, nr_bytes);
	bfdel(tmp);
#else
	memcpy(output, input->field, nr_bytes);
#endif
	return output;
}

inline uint16_t *bftouint16(const struct bitfield *input)
{
	int bitnslots = (input->size - 1) / 16 + 1;
	int nr_bytes = (input->size - 1) / CHAR_BIT + 1;
	uint16_t *output = calloc(1, bitnslots * sizeof(uint16_t));
	if (!output)
		return NULL;
#if __BYTE_ORDER == __BIG_ENDIAN
	struct bitfield *tmp = bf_htole(input);
	if (!tmp) {
		free(output);
		return NULL;
	}
	memcpy(output, tmp->field, nr_bytes);
	bfdel(tmp);
	uint16_letoh_ip(output, bitnslots);
#else
	memcpy(output, input->field, nr_bytes);
#endif
	return output;
}

inline uint32_t *bftouint32(const struct bitfield *input)
{
	int bitnslots = (input->size - 1) / 32 + 1;
	uint32_t *output;
	if (sizeof(uint32_t) == sizeof(unsigned long)) {
		output = malloc(bitnslots * sizeof(uint32_t));
		if (!output)
			return NULL;
		int i;
		for (i = 0; i < bitnslots; i++)
			output[i] = input->field[i];
	} else {
		int nr_bytes = (input->size - 1) / CHAR_BIT + 1;
		output = calloc(1, bitnslots * sizeof(uint32_t));
		if (!output)
			return NULL;
#if __BYTE_ORDER == __BIG_ENDIAN
		struct bitfield *tmp = bf_htole(input);
		if (!tmp) {
			free(output);
			return NULL;
		}
		memcpy(output, tmp->field, nr_bytes);
		bfdel(tmp);
		uint32_letoh_ip(output, bitnslots);
#else
		memcpy(output, input->field, nr_bytes);
#endif
	}
	return output;
}

inline uint64_t *bftouint64(const struct bitfield *input)
{
	int bitnslots = (input->size - 1) / 64 + 1;
	uint64_t *output;
	if (sizeof(uint64_t) == sizeof(unsigned long)) {
		output = malloc(bitnslots * sizeof(uint64_t));
		if (!output)
			return NULL;
		memcpy(output, input->field, bitnslots * sizeof(uint64_t));
	} else {
		int nr_bytes = (input->size - 1) / CHAR_BIT + 1;
		output = calloc(1, bitnslots * sizeof(uint64_t));
#if __BYTE_ORDER == __BIG_ENDIAN
		struct bitfield *tmp = bf_htole(input);
		if (!tmp) {
			free(output);
			return NULL;
		}
		memcpy(output, tmp->field, nr_bytes);
		bfdel(tmp);
		uint64_letoh_ip(output, bitnslots);
#else
		memcpy(output, input->field, nr_bytes);
#endif
	}
	return output;
}

void bf2str_ip(const struct bitfield *input, char *output)
{
	unsigned int bitnslots = BITNSLOTS(input->size);
	unsigned int i, j;
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
	else if (sizeof(unsigned short) == 4)
		bftouint32_ip(input, (uint32_t *) output);
	//else ?
}

void bf2long_ip(const struct bitfield *input, unsigned long *output)
{
	int i;
	int bitnslots = BITNSLOTS(input->size);
	for (i = 0; i < bitnslots; i++)
		output[i] = input->field[i];
}

inline void bftouint8_ip(const struct bitfield *input, uint8_t * output)
{
	int nr_bytes = (input->size - 1) / CHAR_BIT + 1;
#if __BYTE_ORDER == __BIG_ENDIAN
	struct bitfield *tmp = bf_htole(input);
	memcpy(output, tmp->field, nr_bytes);
	bfdel(tmp);
#else
	memcpy(output, input->field, nr_bytes);
#endif
}

inline void bftouint16_ip(const struct bitfield *input, uint16_t * output)
{
	int nr_bytes = (input->size - 1) / CHAR_BIT + 1;
#if __BYTE_ORDER == __BIG_ENDIAN
	struct bitfield *tmp = bf_htole(input);
	int bitnslots = (input->size - 1) / 16 + 1;
	memcpy(output, tmp->field, nr_bytes);
	bfdel(tmp);
	uint16_letoh_ip(output, bitnslots);
#else
	memcpy(output, input->field, nr_bytes);
#endif
}

inline void bftouint32_ip(const struct bitfield *input, uint32_t * output)
{
	if (sizeof(uint32_t) == sizeof(unsigned long)) {
		unsigned int i;
		for (i = 0; i < BITNSLOTS(input->size); i++)
			output[i] = input->field[i];
	} else {
		unsigned int nr_bytes = (input->size - 1) / CHAR_BIT + 1;
#if __BYTE_ORDER == __BIG_ENDIAN
		unsigned int bitnslots = (input->size - 1) / 32 + 1;
		struct bitfield *tmp = bf_htole(input);
		memcpy(output, tmp->field, nr_bytes);
		bfdel(tmp);
		uint32_letoh_ip(output, bitnslots);
#else
		memcpy(output, input->field, nr_bytes);
#endif
	}
}

inline void bftouint64_ip(const struct bitfield *input, uint64_t * output)
{
	if (sizeof(uint64_t) == sizeof(unsigned long)) {
		unsigned int i;
		for (i = 0; i < BITNSLOTS(input->size); i++)
			output[i] = input->field[i];
	} else {
		unsigned int nr_bytes = (input->size - 1) / CHAR_BIT + 1;
#if __BYTE_ORDER == __BIG_ENDIAN
		unsigned int bitnslots = (input->size - 1) / 64 + 1;
		struct bitfield *tmp = bf_htole(input);
		memcpy(output, tmp->field, nr_bytes);
		bfdel(tmp);
		uint64_letoh_ip(output, bitnslots);
#else
		memcpy(output, input->field, nr_bytes);
#endif
	}
}

/*
 * Create and delete bitfields
 */

struct bitfield *bfnew(const unsigned int size)
{
	struct bitfield *instance = malloc(sizeof(struct bitfield));
	if (!instance)
		return NULL;
	instance->size = size;
	instance->field = calloc(1, BITNSLOTS(size) * sizeof(unsigned long));
	return instance;
}

struct bitfield *bfnew_ones(const unsigned int size)
{
	struct bitfield *instance = malloc(sizeof(struct bitfield));
	if (!instance)
		return NULL;
	instance->size = size;
	instance->field = malloc(BITNSLOTS(size) * sizeof(unsigned long));
	bfsetall(instance);
	return instance;
}

struct bitfield *bfnew_quick(const unsigned int size)
{
	struct bitfield *instance = malloc(sizeof(struct bitfield));
	if (!instance)
		return NULL;
	instance->size = size;
	instance->field = malloc(BITNSLOTS(size) * sizeof(unsigned long));
	if (!instance->field) {
		free(instance);
		return NULL;
	}
	instance->field[BITNSLOTS(size) - 1] = 0UL;	//because the tail should be zeroed anyway
	return instance;
}

void bfdel(struct bitfield *instance)
{
	if (instance) {
		if (instance->field) {
			free(instance->field);
			instance->field = NULL;
		}
		free(instance);
		instance = NULL;
	}
}

/*
 * Operations with single bits
 */

unsigned int bfgetbit(const struct bitfield *instance, const unsigned int bit)
{
	/* might be good to check whether bit is within range */

	return BITGET(instance, bit);
}

void bfsetbit(struct bitfield *instance, const unsigned int bit)
{
	BITSET(instance, bit);
}

void bfclearbit(struct bitfield *instance, const unsigned int bit)
{
	BITCLEAR(instance, bit);
}

void bftogglebit(struct bitfield *instance, const unsigned int bit)
{
	BITTOGGLE(instance, bit);
}

/*
 * Logical operations with bitfields
 */

static inline void bfand__(struct bitfield *input1,
			   const struct bitfield *input2)
{
	/* If the inputs are different size, take the shorter, and ignore the difference.
	 * This way we'll surely have no error.
	 */
	int size = input1->size < input2->size ? input1->size : input2->size;
	int bitnslots = BITNSLOTS(size);
	int i;
	for (i = 0; i < bitnslots; i++)
		input1->field[i] &= input2->field[i];
}

struct bitfield *bfand_(unsigned int count, ...)
{
	unsigned int i;
	va_list args;
	va_start(args, count);
	struct bitfield *output = bfclone(va_arg(args, struct bitfield *));
	if (!output) {
		va_end(args);
		return NULL;
	}
	for (i = 1; i < count; i++) {
		bfand__(output, va_arg(args, struct bitfield *));
	}
	va_end(args);
	bfcleartail(output);
	return output;
}

struct bitfield *bfnot(const struct bitfield *input)
{
	int bitnslots = BITNSLOTS(input->size);
	int i;
	struct bitfield *output = bfnew(input->size);
	if (!output)
		return NULL;
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

static inline void bfor__(struct bitfield *input1,
			  const struct bitfield *input2)
{
	/* If the inputs are different size, take the shorter, and ignore the difference.
	 * This way we'll surely have no error.
	 */
	int size = input1->size < input2->size ? input1->size : input2->size;
	int bitnslots = BITNSLOTS(size);
	int i;
	for (i = 0; i < bitnslots; i++)
		input1->field[i] |= input2->field[i];
}

struct bitfield *bfor_(unsigned int count, ...)
{
	unsigned int i;
	va_list args;
	va_start(args, count);
	struct bitfield *output = bfclone(va_arg(args, struct bitfield *));
	if (!output) {
		va_end(args);
		return NULL;
	}
	for (i = 1; i < count; i++) {
		bfor__(output, va_arg(args, struct bitfield *));
	}
	va_end(args);
	bfcleartail(output);
	return output;
}

static inline void bfxor__(struct bitfield *input1,
			   const struct bitfield *input2)
{
	/* If the inputs are different size, take the shorter, and ignore the difference.
	 * This way we'll surely have no error.
	 */
	int size = input1->size < input2->size ? input1->size : input2->size;
	int bitnslots = BITNSLOTS(size);
	int i;
	for (i = 0; i < bitnslots; i++)
		input1->field[i] ^= input2->field[i];
}

struct bitfield *bfxor_(unsigned int count, ...)
{
	unsigned int i;
	va_list args;
	va_start(args, count);
	struct bitfield *output = bfclone(va_arg(args, struct bitfield *));
	if (!output) {
		va_end(args);
		return NULL;
	}
	for (i = 1; i < count; i++) {
		bfxor__(output, va_arg(args, struct bitfield *));
	}
	va_end(args);
	bfcleartail(output);
	return output;
}

/*
 * Manipulate bitfields
 */

static inline struct bitfield *bfcat__(const struct bitfield *input1,
				       const struct bitfield *input2)
{
	unsigned int i;
	unsigned int output_size = input1->size + input2->size;
	struct bitfield *output = bfnew(output_size);
	if (!output)
		return NULL;
	/* copy the first input to output as is */
	for (i = 0; i < BITNSLOTS(input1->size); i++) {
		output->field[i] |= input1->field[i];
	}

	/* find offset bit and offset slot */
	unsigned int offset_bit = input1->size % LONG_BIT;
	unsigned int offset_slot = input1->size / LONG_BIT;

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
	if (offset_bit == 0)
		return output;
	if ((output_size - 1) % LONG_BIT < (input2->size - 1) % LONG_BIT) {
		for (i = 0; i < BITNSLOTS(input2->size); i++) {
			output->field[i + offset_slot + 1] |=
			    (input2->field[i] >> (LONG_BIT - offset_bit));
		}
	} else if ((output_size - 1) % LONG_BIT > (input2->size - 1) % LONG_BIT) {
		for (i = 0; i < (BITNSLOTS(input2->size) - 1); i++) {
			output->field[i + offset_slot + 1] |=
			    (input2->field[i] >> (LONG_BIT - offset_bit));
		}
	}
	return output;
}

struct bitfield *bfcat_(unsigned int count, ...)
{
	unsigned int i;
	va_list args;
	va_start(args, count);
	struct bitfield *output = bfclone(va_arg(args, struct bitfield *));
	for (i = 1; i < count; i++) {

		struct bitfield *tmp =
		    bfcat__(output, va_arg(args, struct bitfield *));
		if (!tmp) {
			va_end(args);
			return NULL;
		}
		/* reassign *output to point to new struct without leaking memory */
		free(output->field);
		*output = *tmp;
		free(tmp);
	}
	va_end(args);
	return output;
}

inline unsigned int bf_count_arguments(char *s)
{
	unsigned i, argc = 1;
	for (i = 0; s[i]; i++)
		if (s[i] == ',')
			argc++;
	return argc;
}

void bfclearall(struct bitfield *instance)
{
	unsigned int i;
	for (i = 0; i < BITNSLOTS(instance->size); i++)
		instance->field[i] = 0UL;
}

struct bitfield *bfclone(const struct bitfield *input)
{
	unsigned int bitnslots = BITNSLOTS(input->size);
	/* not using bfnew, because calloc is slow and 0-ed memory not needed anyway */
	struct bitfield *output = malloc(sizeof(struct bitfield));
	if (!output)
		return NULL;
	output->size = input->size;
	output->field = malloc(bitnslots * sizeof(unsigned long));
	if (!output->field) {
		bfdel(output);
		return NULL;
	}
	memcpy(output->field, input->field, bitnslots * sizeof(unsigned long));
	return output;
}

unsigned int bfcmp(const struct bitfield *input1, const struct bitfield *input2,
		   char **errmsg)
{
	unsigned int retcode = 0;
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
	     0) ? -1UL : (1UL << (input1->size % LONG_BIT)) - 1UL;
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

unsigned int bfcpy(const struct bitfield *src, struct bitfield *dest)
{
	if (src->size > dest->size)
		return 1;
	unsigned int i;
	for (i = 0; i < BITNSLOTS(src->size); i++)
		dest->field[i] = src->field[i];
	return 0;
}

unsigned int bffs(const struct bitfield *instance)
{
	unsigned int i;
	unsigned int pos = 0;
	unsigned int bitnslots = BITNSLOTS(instance->size);
	unsigned int tmp;
	for (i = 0; i < bitnslots; i++) {
#ifdef __GNUC__
		/* if GCC or Clang, use builtins */
		tmp = __builtin_ffsl(instance->field[i]);
#elif defined(_DEFAULT_SOURCE) || defined(_GNU_SOURCE) || defined(__FreeBSD__) || defined(__APPLE__)
		/* GNU libc and FreeBSD 5.3+ (and OS X 10.4+) libc have ffsl */
		tmp = ffsl(instance->field[i]);
#else
		/* at least, every POSIX system has ffs */
		if ((instance->field[i] & (1 + ~instance->field[i])) <=
		    0xffffffff)
			tmp = ffs(instance->field[i]);
		else
			tmp = 32 + ffs(instance->field[i] >> 32);
#endif
		if (tmp) {
			pos = i * LONG_BIT + tmp;
			return pos;
		}
	}
	return pos;
}

unsigned int bffz(const struct bitfield *instance)
{
	unsigned int i;
	unsigned int pos = 0;
	unsigned int bitnslots = BITNSLOTS(instance->size);
	unsigned int tmp;
	for (i = 0; i < bitnslots; i++) {
#ifdef __GNUC__			/* if GCC or Clang, use builtins */
		tmp = __builtin_ffsl(~(instance->field[i]));
#elif defined(_DEFAULT_SOURCE) || defined(_GNU_SOURCE)	/* if Glibc, use extensions */
		tmp = ffsl(~(instance->field[i]));
#else
		if ((~instance->field[i] & (1 + instance->field[i])) <=
		    0xffffffff)
			tmp = ffs(~instance->field[i]);
		else
			tmp = 32 + ffs(~(instance->field[i]) >> 32);
#endif
		if (tmp) {
			pos = i * LONG_BIT + tmp;
			/* ignore clear bits in the tail */
			if (pos <= instance->size)
				return pos;
			else
				return 0;
		}
	}
	return pos;
}

unsigned int bfls(const struct bitfield *instance)
{
	int i;
#if defined(__FreeBSD__) || defined(__APPLE__)
	/* FreeBSD 5.3+ (and OS X 10.4+) libc have flsl */
	unsigned int bitnslots = BITNSLOTS(instance->size);
	unsigned int tmp;
	for (i = bitnslots - 1; i >= 0; i--) {
		tmp = flsl(instance->field[i]);
		if (tmp)
			return i * LONG_BIT + tmp;
	}
#else
	for (i = instance->size - 1; i >= 0; i--) {
		if (BITGET(instance, i))
			return i + 1;
	}
#endif
	return 0;
}

unsigned int bflz(const struct bitfield *instance)
{
	int i;
#if defined(__FreeBSD__) || defined(__APPLE__)
	/* FreeBSD 5.3+ (and OS X 10.4+) libc have flsl */
	unsigned int bitnslots = BITNSLOTS(instance->size);
	unsigned int tmp;
	for (i = bitnslots - 1; i >= 0; i--) {
		tmp = flsl(~instance->field[i]);
		if (tmp)
			return i * LONG_BIT + tmp;
	}
#else
	for (i = instance->size - 1; i >= 0; i--) {
		if (!BITGET(instance, i))
			return i + 1;
	}
#endif
	return 0;
}

unsigned int bfclo(const struct bitfield *instance)
{
	int i;
	unsigned int count = 0;
	unsigned int bitnslots = BITNSLOTS(instance->size);
	unsigned int tail = LONG_BIT - instance->size % LONG_BIT;
	unsigned int tmp;
#ifdef __GNUC__
	if (instance->field[bitnslots - 1])
		count =
		    __builtin_clzl(~(instance->field[bitnslots - 1] << tail));
	if (count < LONG_BIT - tail)
		return count;
	for (i = bitnslots - 2; i >= 0; i--) {
		if (~instance->field[i])
			return count + __builtin_clzl(~instance->field[i]);
		else
			count += LONG_BIT;
	}
#else				/* write your own */
	tmp = bflz(instance);
	if (tmp)
		count = instance->size - tmp;
	else
		count = instance->size;
#endif
	return count;
}

unsigned int bfclz(const struct bitfield *instance)
{
	int i;
	unsigned int count = 0;
	unsigned int bitnslots = BITNSLOTS(instance->size);
	unsigned int tail = LONG_BIT - instance->size % LONG_BIT;
	unsigned int tmp;
#ifdef __GNUC__
	if (instance->field[bitnslots - 1])
		return __builtin_clzl(instance->field[bitnslots - 1] << tail);
	else
		count = LONG_BIT - tail;
	for (i = bitnslots - 2; i >= 0; i--) {
		if (instance->field[i])
			return count + __builtin_clzl(instance->field[i]);
		else
			count += LONG_BIT;
	}
#else				/* write your own */
	tmp = bfls(instance);
	if (tmp)
		count = instance->size - tmp;
	else
		count = instance->size;
#endif
	return count;
}

unsigned int bfcto(const struct bitfield *instance)
{
#ifdef __GNUC__
	int i;
	unsigned int count = 0;
	unsigned int bitnslots = BITNSLOTS(instance->size);
	for (i = 0; i < bitnslots; i++) {
		if (instance->field[i] == 0) {
			return count;
		} else if (~instance->field[i] == 0) {
			count += LONG_BIT;
		} else {
			return count + __builtin_ctzl(~instance->field[i]);
		}
	}
	return count;
#else
	unsigned int ffz = bffz(instance);
	if (ffz)
		return ffz - 1;
	else
		return instance->size;
#endif
}

unsigned int bfctz(const struct bitfield *instance)
{
#ifdef __GNUC__
	int i;
	unsigned int count = 0;
	unsigned int bitnslots = BITNSLOTS(instance->size);
	for (i = 0; i < bitnslots - 1; i++) {
		if (instance->field[i] == 0) {
			count += LONG_BIT;
		} else if (~instance->field[i] == 0) {
			return count;
		} else {
			return count + __builtin_ctzl(instance->field[i]);
		}
	}
	if (instance->field[bitnslots - 1] == 0) {
		count += (instance->size - 1) % LONG_BIT + 1;
	} else if (~instance->field[bitnslots - 1] == 0) {
		return count;
	} else {
		count += __builtin_ctzl(instance->field[bitnslots - 1]);
	}
	return count;
#else
	unsigned int ffs = bffs(instance);
	if (ffs)
		return ffs - 1;
	else
		return instance->size;
#endif
}

unsigned int bfhamming(const struct bitfield *input1,
		       const struct bitfield *input2)
{
	struct bitfield *input_xored = bfxor(input1, input2);
	unsigned int hamming = bfpopcount(input_xored);
	bfdel(input_xored);
	return hamming;
}

unsigned int bfisempty(const struct bitfield *instance)
{
	unsigned int i;
	unsigned int bitnslots = BITNSLOTS(instance->size);
	for (i = 0; i < bitnslots; i++) {
		if (instance->field[i] != 0UL)
			return 1;
	}
	return 0;
}

unsigned int bfpopcount(const struct bitfield *instance)
{
	unsigned int bits = 0;
	unsigned int i;
	for (i = 0; i < BITNSLOTS(instance->size); i++) {
#if defined(HAVE_BUILTIN_POPCOUNTL)
		/* this is GCC and Clang only */
		bits += __builtin_popcountl(instance->field[i]);
#elif defined(HAVE_POPCOUNTL)
		bits += popcountl(instance->field[i]);
#elif defined(HAVE_POPCOUNT)
		bits += popcount((unsigned int)instance->field[i]);
		if (sizeof(unsigned int) != sizeof(unsigned long))
			bits +=
			    popcount((unsigned int)(instance->field[i] >> 32));
#else
		unsigned long m1, m2, m4, h01, tmp;
		if (sizeof(uint64_t) == sizeof(unsigned long)) {
			m1 = 0x5555555555555555L;
			m2 = 0x3333333333333333L;
			m4 = 0x0F0F0F0F0F0F0F0FL;
			h01 = 0x0101010101010101L;
		} else {
			m1 = 0x55555555;
			m2 = 0x33333333;
			m4 = 0x0F0F0F0F;
			h01 = 0x01010101;
		}
		tmp = instance->field[i];
		tmp -= (tmp >> 1) & m1;
		tmp = (tmp & m2) + ((tmp >> 2) & m2);
		tmp = (tmp + (tmp >> 4)) & m4;
		if (sizeof(uint64_t) == sizeof(unsigned long))
			bits += (tmp * h01) >> 56;
		else
			bits += (tmp * h01) >> 24;
#endif
	}
	return bits;
}

int bfpos(const struct bitfield *haystack, const struct bitfield *needle)
{
	unsigned int heystack_size = bfsize(haystack);
	unsigned int needle_size = bfsize(needle);
	int i;
	int diff = heystack_size - needle_size;

	/* needle longer than haystack; return error code -2 */
	if (diff < 0) {
		return -2;
	}
	for (i = 0; i <= diff; i++) {
		struct bitfield *needle_candidate =
		    bfsub(haystack, i, needle_size + i);
		unsigned int result;
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
	unsigned int i;
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

void bfresize(struct bitfield *instance, const unsigned int new_size)
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
	unsigned int size = instance->size;
	struct bitfield *tmp = bfnew_quick(size);
	if (!tmp)
		return;		//bad, we need to notify about failure...
	unsigned int i, j;
	unsigned int bitnslots = BITNSLOTS(size);
	for (i = 0; i < (bitnslots - 1); i++) {
		for (j = 0; j < LONG_BIT; j++) {
			if ((instance->field[i] >> j) & 1UL)
				BITSET(tmp, size - i * LONG_BIT - j - 1);
			else
				BITCLEAR(tmp, size - i * LONG_BIT - j - 1);
		}
	}
	unsigned int bits_in_last_input_slot = (size - 1) % LONG_BIT + 1;
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
	unsigned int size = input->size;
	unsigned int bitnslots = BITNSLOTS(size);
	unsigned int i;
	struct bitfield *output = bfnew_quick(size);
	if (!output)
		return NULL;
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
	bfshift_ip(output, tail);
	output->size = size;
	return output;
}

void bfsetall(struct bitfield *instance)
{
	unsigned int i;
	for (i = 0; i < BITNSLOTS(instance->size); i++)
		instance->field[i] = -1UL;
	bfcleartail(instance);
}

void bfshift_ip(struct bitfield *input, const int offset)
{
	int size = bfsize(input);
	if (size <= 1) {
		return;		/* input too short to shift */
	}
	/**
	 * This function "cuts" the bitfield into two and swaps them.
	 * If the offset is positive, the cutting point is counted from the
	 * beginning of the bit array, otherwise from the end.
	**/
	int offset_internal = offset % size;
	if (offset_internal == 0) {
		return;		/* no need to shift */
	}
	/* changing a negative offset to a positive equivalent */
	if (offset_internal < 0)
		offset_internal += size;
	struct bitfield *first_chunk = bfsub(input, 0, offset_internal);
	struct bitfield *second_chunk = bfsub(input, offset_internal, size);

	struct bitfield *tmp = bfcat(second_chunk, first_chunk);
	free(input->field);
	*input = *tmp;
	free(tmp);
	bfdel(first_chunk);
	bfdel(second_chunk);
}

struct bitfield *bfshift(const struct bitfield *input, const int offset)
{
	/**
	 * This function "cuts" the bitfield into two and swaps them.
	 * If the offset is positive, the cutting point is counted from the
	 * beginning of the bit array, otherwise from the end.
	**/
	unsigned int bitnslots = BITNSLOTS(input->size);
	/* removing extra rotations */
	int size = bfsize(input);
	int offset_internal = offset % size;
	if (offset_internal == 0) {
		/* nothing to shift */
		struct bitfield *output = bfnew(size);
		if (!output)
			return NULL;
		memcpy(output->field, input->field,
		       bitnslots * sizeof(unsigned long));
		return output;
	}
	if (offset_internal < 0)
		offset_internal += size;

	struct bitfield *first_chunk = bfsub(input, 0, offset_internal);
	struct bitfield *second_chunk = bfsub(input, offset_internal, size);
	struct bitfield *output = bfcat(second_chunk, first_chunk);
	bfdel(first_chunk);
	bfdel(second_chunk);
	return output;
}

unsigned int bfsize(const struct bitfield *instance)
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
	if (!input)
		return NULL;
	struct bitfield *output = bfnew(end - start);
	if (!output)
		return NULL;

	int start_offset = start % LONG_BIT;	//position of start bit inside slot
	int end_offset = (end - 1) % LONG_BIT;
	int start_slot = start / LONG_BIT;
	int end_slot = (end - 1) / LONG_BIT;	// the slot storing the last bit included into subfield
	int output_slots = BITNSLOTS(end - start);
//    unsigned_long mask_1 = (start_offset = 0) ? -1UL : (1UL << (LONG_BIT - offset)) - 1UL;

    /** filling output slots except the last one, because it may be underfull **/
	if (output_slots > 1) {
		if (start_offset != 0) {	// need to move them one by one
			int i;
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
	unsigned int size = input->size;
	unsigned int bitnslots = BITNSLOTS(size);
	unsigned int length_last_chunk = size % LONG_BIT;
	/* pick first potential candidate for output */
	struct bitfield *output = bfclone(input);
	if (!output)
		return NULL;
	/* will compare bitfields in chunks of length 1 unsigned long */
	unsigned long chunk_a, chunk_b;
	struct bitfield *tmp1, *tmp2;
	/* counters for bit offsets and slots/chunks/longs */
	unsigned int i, j;
	/* shift input string 1 position at a time and compare with best candidate (size - 1 comparisons) */
	for (i = 1; i <= size - 1; i++) {
		/* compare 1 slot/long at a time */
		for (j = bitnslots - 1;; j--) {
			/* special check for tail chunks (may be underfull) */
			if (j == bitnslots - 1 && length_last_chunk != 0) {
				/* this can probably be optimized */
				chunk_a = output->field[j];
				tmp1 =
				    bfshift(input,
					    size - length_last_chunk - i);
				if (!tmp1)
					goto error;
				tmp2 = bfsub(tmp1, 0, length_last_chunk);
				if (!tmp2) {
					bfdel(tmp1);
					goto error;
				}
				chunk_b = tmp2->field[0];
				bfdel(tmp1);
				bfdel(tmp2);
			} else {
				/* this can probably be optimized */
				chunk_a = output->field[j];
				tmp1 = bfshift(input, -i);
				chunk_b = tmp1->field[j];
				bfdel(tmp1);
			}
			/* compare. if a is greater, offset i becomes new best candidate. move to next i */
			if (chunk_a > chunk_b) {
				tmp1 = bfshift(input, -i);
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
 error:
	bfdel(output);
	return NULL;
}
