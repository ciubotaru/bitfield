/**
 * File name: bitfield.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie@ciubotaru.tokyo>
 * License: General Public License, version 3 or later
 * Copyright 2015-2017, 2019-2021
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>		//for ffs()
#include <string.h>		//for ffsl()
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
 * convert storage units inside a bitfield from little endian to host.
 * needed after memcpy to bf on big endian machines
 **/
{
#if (STORAGE_UNIT_SIZE == 8)
	/* do nothing */
#elif (STORAGE_UNIT_SIZE == 16)
	uint16_letoh_ip(instance->field,
			BITNSLOTS(BFSIZE(instance)));
#elif (STORAGE_UNIT_SIZE == 32)
	uint32_letoh_ip(instance->field,
			BITNSLOTS(BFSIZE(instance)));
#elif (STORAGE_UNIT_SIZE == 64)
	uint64_letoh_ip(instance->field,
			BITNSLOTS(BFSIZE(instance)));
#else
#error "Unsupported storage unit size."
#endif
}

static inline void uint16_letoh_ip_(uint16_t * input, const unsigned int size)
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
 * convert storage units inside a bitfield from host to little endian.
 * needed after memcpy from bitfield on big endian machines
 * Used ad bf_htole() in big-endian architectures
 **/
{
	struct bitfield *output = bfclone(input);
	if (!output)
		return NULL;
	unsigned int i;
	for (i = 0; i < BITNSLOTS(BFSIZE(input)); i++) {
#if (STORAGE_UNIT_SIZE == 8)
		BITSLOTBYNO(output, i) = BITSLOTBYNO(input, i);
#elif (STORAGE_UNIT_SIZE == 16)
		BITSLOTBYNO(output, i) = htole16(BITSLOTBYNO(input, i));
#elif (STORAGE_UNIT_SIZE == 32)
		BITSLOTBYNO(output, i) = htole32(BITSLOTBYNO(input, i));
#elif (STORAGE_UNIT_SIZE == 64)
		BITSLOTBYNO(output, i) = htole64(BITSLOTBYNO(input, i));
#else
#error "Unsupported storage unit size."
#endif
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

static inline void bfcleartail(struct bitfield *instance)
{
	int tail = BFTAILSIZE(instance);
	if (tail != 0) {
		/* create a mask for the tail */
		storage_unit mask =
		    (STORAGE_UNIT_PREFIX(1) << tail) - STORAGE_UNIT_PREFIX(1);
		/* clear the extra bits */
		BITSLOTBYNO(instance, BITNSLOTS(BFSIZE(instance)) - 1) &= mask;
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
		for (j = 0; j < STORAGE_UNIT_SIZE; j++) {
			if (input[i * STORAGE_UNIT_SIZE + j] == '1')
				BITSLOTBYNO(output, i) |=
				    (STORAGE_UNIT_PREFIX(1) << j);
			else
				BITSLOTBYNO(output, i) &=
				    ~(STORAGE_UNIT_PREFIX(1) << j);
		}
	}
	for (j = 0; j < (input_len - 1) % STORAGE_UNIT_SIZE + 1; j++) {
		if (input[i * STORAGE_UNIT_SIZE + j] == '1')
			BITSLOTBYNO(output, i) |= (STORAGE_UNIT_PREFIX(1) << j);
		else
			BITSLOTBYNO(output, i) &= ~(STORAGE_UNIT_PREFIX(1) << j);
	}
	bfcleartail(output);
	return output;
}

struct bitfield *short2bf(const unsigned short *input, unsigned int size)
{
	struct bitfield *output = bfnew(size);
	if (!output)
		return NULL;
#if (SIZEOF_UNSIGNED_SHORT == 2)
	uint16tobf_ip((uint16_t *) input, output);
#elif (SIZEOF_UNSIGNED_SHORT == 4)
	uint32tobf_ip((uint32_t *) input, output);
#elif (SIZEOF_UNSIGNED_SHORT == 8)
	uint64tobf_ip((uint64_t *) input, output);
#else
#error "Unsupported size of 'short'."
#endif
	return output;
}

struct bitfield *int2bf(const unsigned int *input, unsigned int size)
{
	struct bitfield *output = bfnew(size);
	if (!output)
		return NULL;
#if (SIZEOF_UNSIGNED_INT == 2)
	uint16tobf_ip((uint16_t *) input, output);
#elif (SIZEOF_UNSIGNED_INT == 4)
	uint32tobf_ip((uint32_t *) input, output);
#elif (SIZEOF_UNSIGNED_INT == 8)
	uint64tobf_ip((uint64_t *) input, output);
#else
#error "Unsupported size of 'int'."
#endif
	return output;
}

struct bitfield *long2bf(const unsigned long *input, unsigned int size)
{
	struct bitfield *output = bfnew(size);
	if (!output)
		return NULL;
#if (SIZEOF_UNSIGNED_LONG == 2)
	uint16tobf_ip((uint16_t *) input, output);
#elif (SIZEOF_UNSIGNED_LONG == 4)
	uint32tobf_ip((uint32_t *) input, output);
#elif (SIZEOF_UNSIGNED_LONG == 8)
	uint64tobf_ip((uint64_t *) input, output);
#else
#error "Unsupported size of 'long'."
#endif
	return output;
}

struct bitfield *ll2bf(const unsigned long long *input, unsigned int size)
{
	struct bitfield *output = bfnew(size);
	if (!output)
		return NULL;
#if (SIZEOF_UNSIGNED_LONG_LONG == 2)
	uint16tobf_ip((uint16_t *) input, oitput);
#elif (SIZEOF_UNSIGNED_LONG_LONG == 4)
	uint32tobf_ip((uint32_t *) input, output);
#elif (SIZEOF_UNSIGNED_LONG_LONG == 8)
	uint64tobf_ip((uint64_t *) input, output);
#else
#error "Unsupported size of 'long long'."
#endif
	return output;
}

struct bitfield *uint8tobf(const uint8_t * input, unsigned int size)
{
	struct bitfield *output = bfnew(size);
	if (!output)
		return NULL;
	uint8tobf_ip(input, output);
	return output;
}

struct bitfield *uint16tobf(const uint16_t * input, unsigned int size)
{
	int tmp_slots = (size - 1) / 16 + 1;
	struct bitfield *output =
	    bfnew(tmp_slots * sizeof(uint16_t) * CHAR_BIT);;
	if (!output)
		return NULL;
	uint16tobf_ip(input, output);
	return output;
}

struct bitfield *uint32tobf(const uint32_t * input, unsigned int size)
{
	int tmp_slots = (size - 1) / 32 + 1;
	struct bitfield *output =
	    bfnew(tmp_slots * sizeof(uint32_t) * CHAR_BIT);
	if (!output)
		return NULL;
	uint32tobf_ip(input, output);
	return output;
}

struct bitfield *uint64tobf(const uint64_t * input, unsigned int size)
{
	struct bitfield *output = bfnew(size);
	if (!output)
		return NULL;
	uint64tobf_ip(input, output);
	return output;
}

void str2bf_ip(const char *input, struct bitfield *output)
{
	unsigned int input_len =
	    (strlen(input) < BFSIZE(output)) ? strlen(input) : BFSIZE(output);
	unsigned int bitnslots = BITNSLOTS(input_len);
	unsigned int i, j;
	for (i = 0; i < bitnslots - 1; i++) {
		for (j = 0; j < STORAGE_UNIT_SIZE; j++) {
			if (input[i * STORAGE_UNIT_SIZE + j] == '1')
				BITSLOTBYNO(output, i) |=
				    (STORAGE_UNIT_PREFIX(1) << j);
			else
				BITSLOTBYNO(output, i) &=
				    ~(STORAGE_UNIT_PREFIX(1) << j);
		}
	}
	for (j = 0; j < (input_len - 1) % STORAGE_UNIT_SIZE + 1; j++) {
		if (input[i * STORAGE_UNIT_SIZE + j] == '1')
			BITSLOTBYNO(output, i) |= (STORAGE_UNIT_PREFIX(1) << j);
		else
			BITSLOTBYNO(output, i) &= ~(STORAGE_UNIT_PREFIX(1) << j);
	}
}

void short2bf_ip(const unsigned short *input, struct bitfield *output)
{
#if (SIZEOF_UNSIGNED_SHORT == 2)
	uint16tobf_ip((const uint16_t *)input, output);
#elif (SIZEOF_UNSIGNED_SHORT == 4)
	uint32tobf_ip((const uint32_t *)input, output);
#elif (SIZEOF_UNSIGNED_SHORT == 8)
	uint64tobf_ip((const uint64_t *)input, output);
#else
#error "Unsupported size of 'short'."
#endif
}

void int2bf_ip(const unsigned int *input, struct bitfield *output)
{
#if (SIZEOF_UNSIGNED_INT== 2)
	uint16tobf_ip((const uint16_t *)input, output);
#elif (SIZEOF_UNSIGNED_INT == 4)
	uint32tobf_ip((const uint32_t *)input, output);
#elif (SIZEOF_UNSIGNED_INT == 8)
	uint64tobf_ip((const uint64_t *)input, output);
#else
#error "Unsupported size of 'int'."
#endif
}

void long2bf_ip(const unsigned long *input, struct bitfield *output)
{
#if (SIZEOF_UNSIGNED_LONG == 2)
	uint16tobf_ip((const uint16_t *)input, output);
#elif (SIZEOF_UNSIGNED_LONG == 4)
	uint32tobf_ip((const uint32_t *)input, output);
#elif (SIZEOF_UNSIGNED_LONG == 8)
	uint64tobf_ip((const uint64_t *)input, output);
#else
#error "Unsupported size of 'long'."
#endif
}

void ll2bf_ip(const unsigned long long *input, struct bitfield *output)
{
#if (SIZEOF_UNSIGNED_LONG_LONG == 2)
	uint16tobf_ip((const uint16_t *)input, output);
#elif (SIZEOF_UNSIGNED_LONG_LONG == 4)
	uint32tobf_ip((const uint32_t *)input, output);
#elif (SIZEOF_UNSIGNED_LONG_LONG == 8)
	uint64tobf_ip((const uint64_t *)input, output);
#else
#error "Unsupported size of 'long long'."
#endif
}

void uint8tobf_ip(const uint8_t * input, struct bitfield *output)
{
	int size = BFSIZE(output);
	int nr_bytes = (size - 1) / CHAR_BIT + 1;
	memcpy(output->field, input, nr_bytes);
	bf_letoh_ip(output);
}

void uint16tobf_ip(const uint16_t * input, struct bitfield *output)
{
	int input_slots = (BFSIZE(output) - 1) / 16 + 1;
	int output_slots = BITNSLOTS(BFSIZE(output));
	int min_bytes =
	    (input_slots * 2 <
	     output_slots * SIZEOF_UNSIGNED_LONG) ? input_slots *
	    2 : output_slots * SIZEOF_UNSIGNED_LONG;
#if (__BYTE_ORDER == __BIG_ENDIAN && STORAGE_UNIT_SIZE != 16)
	uint16_t *tmp = calloc(1, input_slots * 2);
	memcpy(tmp, input, input_slots * 2);
	uint16_htole_ip(tmp, input_slots);
	memcpy(output->field, tmp, min_bytes);
	free(tmp);
	bf_letoh_ip(output);
#else
	memcpy(output->field, input, min_bytes);
#endif
}

void uint32tobf_ip(const uint32_t * input, struct bitfield *output)
{
	int input_slots = (BFSIZE(output) - 1) / 32 + 1;
	int output_slots = BITNSLOTS(BFSIZE(output));
	int min_bytes =
	    (input_slots * 4 <
	     output_slots * SIZEOF_UNSIGNED_LONG) ? input_slots *
	    4 : output_slots * SIZEOF_UNSIGNED_LONG;
#if (__BYTE_ORDER == __BIG_ENDIAN && STORAGE_UNIT_SIZE != 32)
	uint32_t *tmp = calloc(1, input_slots * 4);
	memcpy(tmp, input, input_slots * 4);
	uint32_htole_ip(tmp, input_slots);
	memcpy(output->field, tmp, min_bytes);
	free(tmp);
	bf_letoh_ip(output);
#else
	memcpy(output->field, input, min_bytes);
#endif
}

void uint64tobf_ip(const uint64_t * input, struct bitfield *output)
{
	int input_slots = (BFSIZE(output) - 1) / 64 + 1;
	int output_slots = BITNSLOTS(BFSIZE(output));
	int min_bytes =
	    (input_slots * 8 <
	     output_slots * SIZEOF_UNSIGNED_LONG) ? input_slots *
	    8 : output_slots * SIZEOF_UNSIGNED_LONG;
#if (__BYTE_ORDER == __BIG_ENDIAN && STORAGE_UNIT_SIZE != 64)
	uint64_t *tmp = calloc(1, input_slots * 8);
	memcpy(tmp, input, input_slots * 8);
	uint64_htole_ip(tmp, input_slots);
	memcpy(output->field, tmp, min_bytes);
	free(tmp);
	bf_letoh_ip(output);
#else
	memcpy(output->field, input, min_bytes);
#endif
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
	unsigned int input_len = BFSIZE(input);
	char *output = malloc(input_len + 1);
	if (!output)
		return NULL;
	unsigned int bitnslots = BITNSLOTS(input_len);
	unsigned int i, j;
	for (i = 0; i < bitnslots - 1; i++) {
		for (j = 0; j < STORAGE_UNIT_SIZE; j++) {
			if ((BITSLOTBYNO(input, i) >> j) & 1LU)
				output[i * STORAGE_UNIT_SIZE + j] = '1';
			else
				output[i * STORAGE_UNIT_SIZE + j] = '0';
		}
	}
	for (j = 0; j < (input_len - 1) % STORAGE_UNIT_SIZE + 1; j++) {
		if ((BITSLOTBYNO(input, bitnslots - 1) >> j) & 1LU)
			output[(bitnslots - 1) * STORAGE_UNIT_SIZE + j] = '1';
		else
			output[(bitnslots - 1) * STORAGE_UNIT_SIZE + j] = '0';
	}
	output[input_len] = '\0';
	return output;
}

unsigned short *bf2short(const struct bitfield *input)
{
	int output_slots =
	    (BFSIZE(input) - 1) / (SIZEOF_UNSIGNED_SHORT * CHAR_BIT) + 1;
	unsigned short *output =
	    calloc(1, output_slots * SIZEOF_UNSIGNED_SHORT);
#if (SIZEOF_UNSIGNED_SHORT == 2)
	bftouint16_ip(input, (uint16_t *) output);
#elif (SIZEOF_UNSIGNED_SHORT== 4)
	bftouint32_ip(input, (uint32_t *) output);
#elif (SIZEOF_UNSIGNED_SHORT == 8)
	bftouint64_ip(input, (uint64_t *) output);
#else
#error "Unsupported size of 'short'."
#endif
	return output;
}

unsigned int *bf2int(const struct bitfield *input)
{
	int output_slots =
	    (BFSIZE(input) - 1) / (SIZEOF_UNSIGNED_INT * CHAR_BIT) + 1;
	unsigned int *output = calloc(1, output_slots * SIZEOF_UNSIGNED_INT);
#if (SIZEOF_UNSIGNED_INT == 2)
	bftouint16_ip(input, (uint16_t *) output);
#elif (SIZEOF_UNSIGNED_INT == 4)
	bftouint32_ip(input, (uint32_t *) output);
#elif (SIZEOF_UNSIGNED_INT == 8)
	bftouint64_ip(input, (uint64_t *) output);
#else
#error "Unsupported size of 'int'."
#endif
	return output;
}

unsigned long *bf2long(const struct bitfield *input)
{
	int output_slots =
	    (BFSIZE(input) - 1) / (SIZEOF_UNSIGNED_LONG * CHAR_BIT) + 1;
	unsigned long *output = calloc(1, output_slots * SIZEOF_UNSIGNED_LONG);
#if (SIZEOF_UNSIGNED_LONG == 2)
	bftouint16_ip(input, (uint16_t *) output);
#elif (SIZEOF_UNSIGNED_LONG == 4)
	bftouint32_ip(input, (uint32_t *) output);
#elif (SIZEOF_UNSIGNED_LONG == 8)
	bftouint64_ip(input, (uint64_t *) output);
#else
#error "Unsupported size of 'long'."
#endif
	return output;
}

unsigned long long *bf2ll(const struct bitfield *input)
{
	int output_slots =
	    (BFSIZE(input) - 1) / (SIZEOF_UNSIGNED_LONG_LONG * CHAR_BIT) + 1;
	unsigned long long *output =
	    calloc(1, output_slots * SIZEOF_UNSIGNED_LONG_LONG);
#if (SIZEOF_UNSIGNED_LONG_LONG == 2)
	bftouint16_ip(input, (uint16_t *) output);
#elif (SIZEOF_UNSIGNED_LONG_LONG == 4)
	bftouint32_ip(input, (uint32_t *) output);
#elif (SIZEOF_UNSIGNED_LONG_LONG == 8)
	bftouint64_ip(input, (uint64_t *) output);
#else
#error "Unsupported size of 'long long'."
#endif
	return output;
}

inline uint8_t *bftouint8(const struct bitfield *input)
{
	int nr_bytes = (BFSIZE(input) - 1) / CHAR_BIT + 1;
	uint8_t *output = calloc(1, nr_bytes);
	if (!output)
		return NULL;
	bftouint8_ip(input, output);
	return output;
}

inline uint16_t *bftouint16(const struct bitfield *input)
{
	int output_slots = (BFSIZE(input) - 1) / 16 + 1;
	uint16_t *output = calloc(1, output_slots * 2);
	if (!output)
		return NULL;
	bftouint16_ip(input, output);
	return output;
}

inline uint32_t *bftouint32(const struct bitfield *input)
{
	int output_slots = (BFSIZE(input) - 1) / 32 + 1;
	uint32_t *output = calloc(1, output_slots * 4);
	if (!output)
		return NULL;
	bftouint32_ip(input, output);
	return output;
}

inline uint64_t *bftouint64(const struct bitfield *input)
{
	int output_slots = (BFSIZE(input) - 1) / 64 + 1;
	uint64_t *output = calloc(1, output_slots * 8);
	if (!output)
		return NULL;
	bftouint64_ip(input, output);
	return output;
}

void bf2str_ip(const struct bitfield *input, char *output)
{
	unsigned int bitnslots = BITNSLOTS(BFSIZE(input));
	unsigned int i, j;
	for (i = 0; i < bitnslots - 1; i++) {
		for (j = 0; j < STORAGE_UNIT_SIZE; j++) {
			if ((BITSLOTBYNO(input, i) >> j) & 1LU)
				output[i * STORAGE_UNIT_SIZE + j] = '1';
			else
				output[i * STORAGE_UNIT_SIZE + j] = '0';
		}
	}
	for (j = 0; j < (BFSIZE(input) - 1) % STORAGE_UNIT_SIZE + 1; j++) {
		if ((BITSLOTBYNO(input, bitnslots - 1) >> j) & 1LU)
			output[(bitnslots - 1) * STORAGE_UNIT_SIZE + j] = '1';
		else
			output[(bitnslots - 1) * STORAGE_UNIT_SIZE + j] = '0';
	}
	output[BFSIZE(input)] = '\0';
}

void bf2short_ip(const struct bitfield *input, unsigned short *output)
{
#if (SIZEOF_UNSIGNED_SHORT == 2)
	bftouint16_ip(input, (uint16_t *) output);
#elif (SIZEOF_UNSIGNED_SHORT == 4)
	bftouint32_ip(input, (uint32_t *) output);
#elif (SIZEOF_UNSIGNED_SHORT == 8)
	bftouint64_ip(input, (uint64_t *) output);
#else
#error "Unsupported size of 'short'."
#endif
}

void bf2int_ip(const struct bitfield *input, unsigned int *output)
{
#if (SIZEOF_UNSIGNED_INT == 2)
	bftouint16_ip(input, (uint16_t *) output);
#elif (SIZEOF_UNSIGNED_INT == 4)
	bftouint32_ip(input, (uint32_t *) output);
#elif (SIZEOF_UNSIGNED_INT == 8)
	bftouint64_ip(input, (uint64_t *) output);
#else
#error "Unsupported size of 'int'."
#endif
}

void bf2long_ip(const struct bitfield *input, unsigned long *output)
{
#if (SIZEOF_UNSIGNED_LONG == 2)
	bftouint16_ip(input, (uint16_t *) output);
#elif (SIZEOF_UNSIGNED_LONG == 4)
	bftouint32_ip(input, (uint32_t *) output);
#elif (SIZEOF_UNSIGNED_LONG == 8)
	bftouint64_ip(input, (uint64_t *) output);
#else
#error "Unsupported size of 'long'."
#endif
}

void bf2ll_ip(const struct bitfield *input, unsigned long long *output)
{
#if (SIZEOF_UNSIGNED_LONG_LONG == 2)
	bftouint16_ip(input, (uint16_t *) output);
#elif (SIZEOF_UNSIGNED_LONG_LONG == 4)
	bftouint32_ip(input, (uint32_t *) output);
#elif (SIZEOF_UNSIGNED_LONG_LONG == 8)
	bftouint64_ip(input, (uint64_t *) output);
#else
#error "Unsupported size of 'long long'."
#endif
}

inline void bftouint8_ip(const struct bitfield *input, uint8_t * output)
{
	int nr_bytes = (BFSIZE(input) - 1) / CHAR_BIT + 1;
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
	int input_slots = BITNSLOTS(BFSIZE(input));
	int output_slots = (BFSIZE(input) - 1) / 16 + 1;
	int min_bytes =
	    (input_slots * SIZEOF_UNSIGNED_LONG <
	     output_slots * 2) ? input_slots *
	    SIZEOF_UNSIGNED_LONG : output_slots * 2;
#if (__BYTE_ORDER == __BIG_ENDIAN && STORAGE_UNIT_SIZE != 16)
	struct bitfield *tmp = bf_htole(input);
	memcpy(output, tmp->field, min_bytes);
	bfdel(tmp);
	uint16_letoh_ip(output, output_slots);
#else
	memcpy(output, input->field, min_bytes);
#endif
}

inline void bftouint32_ip(const struct bitfield *input, uint32_t * output)
{
	int input_slots = BITNSLOTS(BFSIZE(input));
	int output_slots = (BFSIZE(input) - 1) / 32 + 1;
	int min_bytes =
	    (input_slots * SIZEOF_UNSIGNED_LONG <
	     output_slots * 4) ? input_slots *
	    SIZEOF_UNSIGNED_LONG : output_slots * 4;
#if (__BYTE_ORDER == __BIG_ENDIAN && STORAGE_UNIT_SIZE != 32)
	struct bitfield *tmp = bf_htole(input);
	memcpy(output, tmp->field, min_bytes);
	bfdel(tmp);
	uint32_letoh_ip(output, output_slots);
#else
	memcpy(output, input->field, min_bytes);
#endif
}

inline void bftouint64_ip(const struct bitfield *input, uint64_t * output)
{
	int input_slots = BITNSLOTS(BFSIZE(input));
	int output_slots = (BFSIZE(input) - 1) / 64 + 1;
	int min_bytes =
	    (input_slots * SIZEOF_UNSIGNED_LONG <
	     output_slots * 8) ? input_slots *
	    SIZEOF_UNSIGNED_LONG : output_slots * 8;
#if (__BYTE_ORDER == __BIG_ENDIAN && STORAGE_UNIT_SIZE != 64)
	struct bitfield *tmp = bf_htole(input);
	memcpy(output, tmp->field, min_bytes);
	bfdel(tmp);
	uint64_letoh_ip(output, output_slots);
#else
	memcpy(output, input->field, min_bytes);
#endif
}

/*
 * Create and delete bitfields
 */

struct bitfield *bfnew(const unsigned int size)
{
	struct bitfield *instance = malloc(sizeof(struct bitfield));
	if (!instance)
		return NULL;
	BFSIZE(instance) = size;
	instance->field = calloc(1, BITNSLOTS(size) * sizeof(storage_unit));
	return instance;
}

struct bitfield *bfnew_ones(const unsigned int size)
{
	struct bitfield *instance = malloc(sizeof(struct bitfield));
	if (!instance)
		return NULL;
	BFSIZE(instance) = size;
	instance->field = malloc(BITNSLOTS(size) * sizeof(storage_unit));
	bfsetall(instance);
	return instance;
}

struct bitfield *bfnew_quick(const unsigned int size)
{
	struct bitfield *instance = malloc(sizeof(struct bitfield));
	if (!instance)
		return NULL;
	BFSIZE(instance) = size;
	instance->field = malloc(BITNSLOTS(size) * sizeof(storage_unit));
	if (!instance->field) {
		free(instance);
		return NULL;
	}
	BITSLOTBYNO(instance, BITNSLOTS(size) - 1) = 0UL;	//because the tail should be zeroed anyway
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
	int size = BFSIZE(input1) < BFSIZE(input2) ? BFSIZE(input1) : BFSIZE(input2);
	int bitnslots = BITNSLOTS(size);
	int i;
	for (i = 0; i < bitnslots; i++)
		BITSLOTBYNO(input1, i) &= BITSLOTBYNO(input2, i);
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
	int bitnslots = BITNSLOTS(BFSIZE(input));
	int i;
	struct bitfield *output = bfnew(BFSIZE(input));
	if (!output)
		return NULL;
	for (i = 0; i < bitnslots; i++)
		BITSLOTBYNO(output, i) = ~BITSLOTBYNO(input, i);
	/* make sure to clear the trailing bits, if there are any */
	bfcleartail(output);
	return output;
}

void bfnot_ip(struct bitfield *instance)
{
	int bitnslots = BITNSLOTS(BFSIZE(instance));
	int i;
	for (i = 0; i < bitnslots; i++)
		BITSLOTBYNO(instance, i) = ~BITSLOTBYNO(instance, i);
	/* make sure to clear the trailing bits, if there are any */
	bfcleartail(instance);
}

static inline void bfor__(struct bitfield *input1,
			  const struct bitfield *input2)
{
	/* If the inputs are different size, take the shorter, and ignore the difference.
	 * This way we'll surely have no error.
	 */
	int size = BFSIZE(input1) < BFSIZE(input2) ? BFSIZE(input1) : BFSIZE(input2);
	int bitnslots = BITNSLOTS(size);
	int i;
	for (i = 0; i < bitnslots; i++)
		BITSLOTBYNO(input1, i) |= BITSLOTBYNO(input2, i);
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
	int size = BFSIZE(input1) < BFSIZE(input2) ? BFSIZE(input1) : BFSIZE(input2);
	int bitnslots = BITNSLOTS(size);
	int i;
	for (i = 0; i < bitnslots; i++)
		BITSLOTBYNO(input1, i) ^= BITSLOTBYNO(input2, i);
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
	unsigned int output_size = BFSIZE(input1) + BFSIZE(input2);
	struct bitfield *output = bfnew(output_size);
	if (!output)
		return NULL;
	/* copy the first input to output as is */
	for (i = 0; i < BITNSLOTS(BFSIZE(input1)); i++) {
		BITSLOTBYNO(output, i) |= BITSLOTBYNO(input1, i);
	}

	/* find offset bit and offset slot */
	unsigned int offset_bit = BFTAILSIZE(input1);
	unsigned int offset_slot = BITSLOTNR(BFSIZE(input1));

	for (i = 0; i < BITNSLOTS(BFSIZE(input2)); i++) {
		BITSLOTBYNO(output, i + offset_slot) |=
		    (BITSLOTBYNO(input2, i) << offset_bit);
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
	if ((output_size - 1) % STORAGE_UNIT_SIZE <
	    (BFSIZE(input2) - 1) % STORAGE_UNIT_SIZE) {
		for (i = 0; i < BITNSLOTS(BFSIZE(input2)); i++) {
			BITSLOTBYNO(output, i + offset_slot + 1) |=
			    (BITSLOTBYNO(input2, i) >> (STORAGE_UNIT_SIZE - offset_bit));
		}
	} else if ((output_size - 1) % STORAGE_UNIT_SIZE >
		   (BFSIZE(input2) - 1) % STORAGE_UNIT_SIZE) {
		for (i = 0; i < (BITNSLOTS(BFSIZE(input2)) - 1); i++) {
			BITSLOTBYNO(output, i + offset_slot + 1) |=
			    (BITSLOTBYNO(input2, i) >> (STORAGE_UNIT_SIZE - offset_bit));
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
		bfdel(output);
		output = tmp;
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
	for (i = 0; i < BITNSLOTS(BFSIZE(instance)); i++)
		BITSLOTBYNO(instance, i) = 0UL;
}

struct bitfield *bfclone(const struct bitfield *input)
{
	unsigned int bitnslots = BITNSLOTS(BFSIZE(input));
	/* not using bfnew, because calloc is slow and 0-ed memory not needed anyway */
	struct bitfield *output = malloc(sizeof(struct bitfield));
	if (!output)
		return NULL;
	BFSIZE(output) = BFSIZE(input);
	output->field = malloc(bitnslots * sizeof(storage_unit));
	if (!output->field) {
		bfdel(output);
		return NULL;
	}
	memcpy(output->field, input->field, bitnslots * sizeof(storage_unit));
	return output;
}

unsigned int bfcmp(const struct bitfield *input1, const struct bitfield *input2,
		   char **errmsg)
{
	unsigned int retcode = 0;
	char *msg;
	if (BFSIZE(input1) != BFSIZE(input2)) {
		msg = "Can not compare bitfields of different length";
		retcode = 2;
		goto error;
	}

	int i;
	int bitnslots = BITNSLOTS(BFSIZE(input1));
	for (i = 0; i < (bitnslots - 1); i++) {	//leaving the last slot for later
		if (BITSLOTBYNO(input1, i) != BITSLOTBYNO(input2, i)) {
			msg = "Bitfields differ";
			retcode = 1;
			goto error;
		}
	}
	/* comparing the last slot using mask, because the tail bits may differ */
	storage_unit mask =
	    (BFTAILSIZE(input1) == 0) ?
	    -STORAGE_UNIT_PREFIX(1) :
	    (STORAGE_UNIT_PREFIX(1) << BFTAILSIZE(input1)) - STORAGE_UNIT_PREFIX(1);
	if ((BITSLOTBYNO(input1, BITSLOTNR((BFSIZE(input1) - 1))) & mask) !=
	    (BITSLOTBYNO(input2, BITSLOTNR(BFSIZE(input2) - 1)) & mask)) {
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
	if (BFSIZE(src) > BFSIZE(dest))
		return 1;
	unsigned int i;
	for (i = 0; i < BITNSLOTS(BFSIZE(src)); i++)
		BITSLOTBYNO(dest, i) = BITSLOTBYNO(src, i);
	return 0;
}

unsigned int bffs(const struct bitfield *instance)
{
	if (!instance)
		return 0;
	unsigned int i;
	unsigned int pos = 0;
	unsigned int bitnslots = BITNSLOTS(BFSIZE(instance));
	unsigned int tmp;
	for (i = 0; i < bitnslots; i++) {
#if (STORAGE_UNIT_SIZE <= (SIZEOF_UNSIGNED_INT * CHAR_BIT))
#if defined(HAVE_BUILTIN_FFS)
		tmp = __builtin_ffs((unsigned int) BITSLOTBYNO(instance, i));
#elif defined(HAVE_FFS)
		tmp = ffs((unsigned int) BITSLOTBYNO(instance, i));
#elif defined(HAVE_BUILTIN_FFSL)
		tmp = __builtin_ffsl((unsigned long) BITSLOTBYNO(instance, i));
#elif defined(HAVE_FFSL)
		tmp = ffsl((unsigned long) BITSLOTBYNO(instance, i));
#else
#error No ffs function available.
#endif
#else
#if defined(HAVE_BUILTIN_FFSL)
		tmp = __builtin_ffsl((unsigned long) BITSLOTBYNO(instance, i));
#elif defined(HAVE_FFSL)
		tmp = ffsl((unsigned long) BITSLOTBYNO(instance, i));
#else
#error No ffs function available.
#endif
#endif
		if (tmp) return i * STORAGE_UNIT_SIZE + tmp;
	}
	return pos;
}

unsigned int bffz(const struct bitfield *instance)
{
	if (!instance)
		return 0;
	struct bitfield *reversed = bfnot(instance);
	unsigned int pos = bffs(reversed);
	bfdel(reversed);
	return pos;
}

unsigned int bfls(const struct bitfield *instance)
{
	if (!instance)
		return 0;
	int i;
#if defined(HAVE_FLSL)
	/* FreeBSD 5.3+ (and OS X 10.4+) libc have flsl */
	unsigned int bitnslots = BITNSLOTS(BFSIZE(instance));
	unsigned int tmp;
	for (i = bitnslots - 1; i >= 0; i--) {
		tmp = flsl(BITSLOTBYNO(instance, i));
		if (tmp)
			return i * STORAGE_UNIT_SIZE + tmp;
	}
#else
	for (i = BFSIZE(instance) - 1; i >= 0; i--) {
		if (BITGET(instance, i))
			return i + 1;
	}
#endif
	return 0;
}

unsigned int bflz(const struct bitfield *instance)
{
	if (!instance)
		return 0;
	int i;
#if defined(HAVE_FLSL)
	/* FreeBSD 5.3+ (and OS X 10.4+) libc have flsl */
	unsigned int bitnslots = BITNSLOTS(BFSIZE(instance));
	unsigned int tmp;
	for (i = bitnslots - 1; i >= 0; i--) {
		tmp = flsl(~BITSLOTBYNO(instance, i));
		if (tmp)
			return i * STORAGE_UNIT_SIZE + tmp;
	}
#else
	for (i = BFSIZE(instance) - 1; i >= 0; i--) {
		if (!BITGET(instance, i))
			return i + 1;
	}
#endif
	return 0;
}

unsigned int bfclo(const struct bitfield *instance)
{
	if (!instance)
		return 0;
#if defined(HAVE_BUILTIN_CLZL)
	int i;
	unsigned int count = 0;
	unsigned long *data = (unsigned long *) instance->field;
	unsigned int last_slot_size =
	    (BFSIZE(instance) - 1) % (SIZEOF_UNSIGNED_LONG * CHAR_BIT) + 1;
	unsigned int bitnslots =
	    (BFSIZE(instance) - 1) / (SIZEOF_UNSIGNED_LONG * CHAR_BIT) + 1;
	unsigned long mask =
	    (1UL << (BFSIZE(instance) % (SIZEOF_UNSIGNED_LONG * CHAR_BIT))) - 1UL;
	/* there is at least one bit in the last slot, so empty mask means full mask */
	if (mask == 0UL)
		mask = ~(0UL);
	/* __builtin_clzl() has UB when input is 0, so we check it first */
	if (data[bitnslots - 1] == mask)
		count += last_slot_size;
	else if (data[bitnslots - 1])
		count =
		    __builtin_clzl(~
				   (data[bitnslots - 1] <<
				    (SIZEOF_UNSIGNED_LONG * CHAR_BIT -
				     last_slot_size)));
	if (count < last_slot_size)
		return count;
	for (i = bitnslots - 2; i >= 0; i--) {
		if (data[i] == ~0UL)
			count += SIZEOF_UNSIGNED_LONG * CHAR_BIT;
		else
			return count + __builtin_clzl(~data[i]);
	}
	return count;
#else				/* write your own */
	unsigned int tmp = bflz(instance);
	if (tmp)
		return BFSIZE(instance) - tmp;
	else
		return BFSIZE(instance);
#endif
}

unsigned int bfclz(const struct bitfield *instance)
{
	if (!instance)
		return 0;
#if defined(HAVE_BUILTIN_CLZL)
	int i;
	unsigned int count = 0;
	unsigned long *data = (unsigned long *) instance->field;
	unsigned int last_slot_size =
	    (BFSIZE(instance) - 1) % (SIZEOF_UNSIGNED_LONG * CHAR_BIT) + 1;
	unsigned int bitnslots =
	    (BFSIZE(instance) - 1) / (SIZEOF_UNSIGNED_LONG * CHAR_BIT) + 1;
	unsigned long mask =
	    (1UL << (BFSIZE(instance) % (SIZEOF_UNSIGNED_LONG * CHAR_BIT))) - 1UL;
	if (mask == 0UL)
		mask = ~0UL;
	if (data[bitnslots - 1])
		return
		    __builtin_clzl(data[bitnslots - 1] & mask) -
		    SIZEOF_UNSIGNED_LONG * CHAR_BIT + last_slot_size;
	else
		count = last_slot_size;
	for (i = bitnslots - 2; i >= 0; i--) {
		if (data[i])
			return count + __builtin_clzl(data[i]);
		else
			count += SIZEOF_UNSIGNED_LONG * CHAR_BIT;
	}
	return count;
#else				/* write your own */
	unsigned int tmp = bfls(instance);
	if (tmp)
		return BFSIZE(instance) - tmp;
	else
		return BFSIZE(instance);
#endif
}

unsigned int bfcto(const struct bitfield *instance)
{
	if (!instance)
		return 0;
#if defined(HAVE_BUILTIN_CTZL)
	int i;
	unsigned int count = 0;
	unsigned long *data = (unsigned long *) instance->field;
	unsigned int last_slot_size =
	    (BFSIZE(instance) - 1) % (SIZEOF_UNSIGNED_LONG * CHAR_BIT) + 1;
	unsigned int bitnslots =
	    (BFSIZE(instance) - 1) / (SIZEOF_UNSIGNED_LONG * CHAR_BIT) + 1;
	unsigned long mask =
	    (1UL << (BFSIZE(instance) % (SIZEOF_UNSIGNED_LONG * CHAR_BIT))) - 1UL;
	if (mask == 0UL) mask = ~0UL;
	for (i = 0; i < bitnslots - 1; i++) {
		if (data[i] == 0UL)
			return count;
		else if (data[i] == ~0UL)
			count += SIZEOF_UNSIGNED_LONG * CHAR_BIT;
		else
			return count + __builtin_ctzl(~data[i]);
	}
	if ((data[bitnslots - 1]) == 0UL)
		return count;
	else if (data[bitnslots - 1] == mask)
		count += last_slot_size;
	else
		count += __builtin_ctzl(~(data[bitnslots - 1] & mask));
	return count;
#else
	unsigned int ffz = bffz(instance);
	if (ffz)
		return ffz - 1;
	else
		return BFSIZE(instance);
#endif
}

unsigned int bfctz(const struct bitfield *instance)
{
	if (!instance)
		return 0;
#if defined(HAVE_BUILTIN_CTZL)
	int i;
	unsigned int count = 0;
	unsigned long *data = (unsigned long *) instance->field;
	unsigned int last_slot_size =
	    (BFSIZE(instance) - 1) % (SIZEOF_UNSIGNED_LONG * CHAR_BIT) + 1;
	unsigned int bitnslots =
	    (BFSIZE(instance) - 1) / (SIZEOF_UNSIGNED_LONG * CHAR_BIT) + 1;
	unsigned long mask =
	    (1UL << (BFSIZE(instance) % (SIZEOF_UNSIGNED_LONG * CHAR_BIT))) - 1UL;
	if (mask == 0UL) mask = ~0UL;
	for (i = 0; i < bitnslots - 1; i++) {
		if (data[i] == 0UL)
			count += (SIZEOF_UNSIGNED_LONG * CHAR_BIT);
		else if (data[i] == ~0UL)
			return count;
		else
			return count + __builtin_ctzl(data[i]);
	}
	if (data[bitnslots - 1] == 0UL)
		count += last_slot_size;
	else if (data[bitnslots - 1] == mask)
		return count;
	else
		count += __builtin_ctzl(data[bitnslots - 1] & mask);
	return count;
#else
	unsigned int ffs = bffs(instance);
	if (ffs)
		return ffs - 1;
	else
		return BFSIZE(instance);
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
	unsigned int bitnslots = BITNSLOTS(BFSIZE(instance));
	for (i = 0; i < bitnslots; i++) {
		if (BITSLOTBYNO(instance, i) != 0UL)
			return 1;
	}
	return 0;
}

unsigned int bfpopcount(const struct bitfield *instance)
{
	unsigned int bits = 0;
	unsigned int i;
	unsigned int bitnslots;
#if (defined(HAVE_BUILTIN_POPCOUNTL) || defined(HAVE_POPCOUNTL))
	bitnslots = (BFSIZE(instance) - 1) / (SIZEOF_UNSIGNED_LONG * CHAR_BIT) + 1;
	unsigned long *data = (unsigned long *) instance->field;
#elif defined(HAVE_POPCOUNT)
	bitnslots = (BFSIZE(instance) - 1) / (SIZEOF_UNSIGNED_INT * CHAR_BIT) + 1;
	unsigned int *data = (unsigned int *) instance->field;
#else
	bitnslots = BITNSLOTS(BFSIZE(instance));
	storage_unit m1, m2, m4, h01, tmp;
#endif
	for (i = 0; i < bitnslots; i++) {
#if defined(HAVE_BUILTIN_POPCOUNTL)
		/* this is GCC and Clang only */
		bits += __builtin_popcountl(data[i]);
#elif defined(HAVE_POPCOUNTL)
		bits += popcountl(data[i]);
#elif defined(HAVE_POPCOUNT)
		bits += popcount(data[i]);
#else
		if (sizeof(uint64_t) == sizeof(storage_unit)) {
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
		tmp = BITSLOTBYNO(instance, i);
		tmp -= (tmp >> 1) & m1;
		tmp = (tmp & m2) + ((tmp >> 2) & m2);
		tmp = (tmp + (tmp >> 4)) & m4;
		if (sizeof(uint64_t) == sizeof(storage_unit))
			bits += (tmp * h01) >> 56;
		else
			bits += (tmp * h01) >> 24;
#endif
	}
	return bits;
}

int bfpos(const struct bitfield *haystack, const struct bitfield *needle)
{
	unsigned int heystack_size = BFSIZE(haystack);
	unsigned int needle_size = BFSIZE(needle);
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

#include <inttypes.h>
void bfprint_lsb(const struct bitfield *instance)
{
	unsigned int i;
	for (i = 0; i < BFSIZE(instance); i++)
#if STORAGE_UNIT_SIZE == 64
		printf("%" PRId64 "",
		       (BITSLOTBYNO(instance, BITSLOTNR(i)) >> (i % STORAGE_UNIT_SIZE)) &
		       STORAGE_UNIT_PREFIX(1));
#else
		printf("%" PRId32 "",
		       (BITSLOTBYNO(instance, BITSLOTNR(i)) >> (i % STORAGE_UNIT_SIZE)) &
		       STORAGE_UNIT_PREFIX(1));
#endif
	/* maybe it would be quicker to convert bitfield to string (bf2str) and print it all at once */
}

void bfprint_msb(const struct bitfield *instance)
{
	int i;
	for (i = BFSIZE(instance) - 1; i >= 0; i--)
#if STORAGE_UNIT_SIZE == 64
		printf("%" PRId64 "",
#else
		printf("%" PRId32 "",
#endif
		       (BITSLOTBYNO(instance, BITSLOTNR(i)) >> (i % STORAGE_UNIT_SIZE)) &
		       STORAGE_UNIT_PREFIX(1));
}

void bfresize(struct bitfield *instance, const unsigned int new_size)
{
	int old_bitnslots = BITNSLOTS(BFSIZE(instance));
	int new_bitnslots = BITNSLOTS(new_size);
	BFSIZE(instance) = new_size;
	void *tmp;
	tmp = realloc(instance->field, new_bitnslots * sizeof(storage_unit));
	if (tmp != NULL)
		instance->field = tmp;
	if ((new_bitnslots < old_bitnslots) & (new_size % STORAGE_UNIT_SIZE !=
					       0))
		bfcleartail(instance);
}

void bfrev_ip(struct bitfield *instance)
{
	unsigned int size = BFSIZE(instance);
	struct bitfield *tmp = bfnew_quick(size);
	if (!tmp)
		return;		//bad, we need to notify about failure...
	unsigned int i, j;
	unsigned int bitnslots = BITNSLOTS(size);
	for (i = 0; i < (bitnslots - 1); i++) {
		for (j = 0; j < STORAGE_UNIT_SIZE; j++) {
			if ((BITSLOTBYNO(instance, i) >> j) & STORAGE_UNIT_PREFIX(1))
				BITSET(tmp,
				       size - i * STORAGE_UNIT_SIZE - j - 1);
			else
				BITCLEAR(tmp,
					 size - i * STORAGE_UNIT_SIZE - j - 1);
		}
	}
	unsigned int bits_in_last_input_slot =
	    (size - 1) % STORAGE_UNIT_SIZE + 1;
	for (j = 0; j < bits_in_last_input_slot; j++) {
		if ((BITSLOTBYNO(instance, bitnslots - 1) >> j) & STORAGE_UNIT_PREFIX(1))
			BITSET(tmp, bits_in_last_input_slot - j - 1);
		else
			BITCLEAR(tmp, bits_in_last_input_slot - j - 1);
	}
	memcpy(instance->field, tmp->field, bitnslots * sizeof(storage_unit));
	bfdel(tmp);
}

struct bitfield *bfrev(const struct bitfield *input)
{
	unsigned int size = BFSIZE(input);
	unsigned int bitnslots = BITNSLOTS(size);
	unsigned int i;
	struct bitfield *output = bfnew_quick(size);
	if (!output)
		return NULL;
	for (i = 0; i < bitnslots; i++) {
		/* taken from http://graphics.stanford.edu/~seander/bithacks.html#BitReverseObvious */
		storage_unit v = BITSLOTBYNO(input, bitnslots - i - 1);	// the source slot to be reversed
		BITSLOTBYNO(output, i) = v;	// the destination slot; will be reversed bits of v; first get LSB of v
		int s = STORAGE_UNIT_SIZE - 1;	// extra shift needed at end
		for (v >>= 1; v; v >>= 1) {
			BITSLOTBYNO(output, i) <<= STORAGE_UNIT_PREFIX(1);
			BITSLOTBYNO(output, i) |= v & STORAGE_UNIT_PREFIX(1);
			s--;
		}
		BITSLOTBYNO(output, i) <<= s;	// shift when v's highest bits are zero
	}
	int tail = bitnslots * STORAGE_UNIT_SIZE - size;
	BFSIZE(output) = bitnslots * STORAGE_UNIT_SIZE;
	bfshift_ip(output, tail);
	BFSIZE(output) = size;
	return output;
}

void bfsetall(struct bitfield *instance)
{
	unsigned int i;
	for (i = 0; i < BITNSLOTS(BFSIZE(instance)); i++)
		BITSLOTBYNO(instance, i) = -STORAGE_UNIT_PREFIX(1);
	bfcleartail(instance);
}

void bfshift_ip(struct bitfield *input, const int offset)
{
	if (!input) return;
	int size = BFSIZE(input);
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
	if (tmp) {
		free(input->field);
		input->field = tmp->field;
		free(tmp);
	}
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
	unsigned int bitnslots = BITNSLOTS(BFSIZE(input));
	/* removing extra rotations */
	int size = BFSIZE(input);
	int offset_internal = offset % size;
	if (offset_internal == 0) {
		/* nothing to shift */
		struct bitfield *output = bfnew(size);
		if (!output)
			return NULL;
		memcpy(output->field, input->field,
		       bitnslots * sizeof(storage_unit));
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

	int start_offset = start % STORAGE_UNIT_SIZE;	//position of start bit inside slot
	int end_offset = (end - 1) % STORAGE_UNIT_SIZE;
	int start_slot = BITSLOTNR(start);
	int end_slot = BITSLOTNR(end - 1);	// the slot storing the last bit included into subfield
	int output_slots = BITNSLOTS(end - start);
//    unsigned_long mask_1 = (start_offset = 0) ? -STORAGE_UNIT_PREFIX(1) : (STORAGE_UNIT_PREFIX(1) << (STORAGE_UNIT_SIZE - offset)) - STORAGE_UNIT_PREFIX(1);

    /** filling output slots except the last one, because it may be underfull **/
	if (output_slots > 1) {
		if (start_offset != 0) {	// need to move them one by one
			int i;
			for (i = 0; i < output_slots - 1; i++) {
				BITSLOTBYNO(output, i) =
				    BITSLOTBYNO(input, start_slot + i) >> start_offset;
				BITSLOTBYNO(output, i) |=
				    BITSLOTBYNO(input, start_slot + i + 1) << (STORAGE_UNIT_SIZE -
							start_offset);
			}
		} else
			memcpy(output->field, (input->field) + start_slot, (output_slots - 1) * sizeof(storage_unit));	// the easiest case
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
		BITSLOTBYNO(output, BITSLOTNR(end - start - 1)) =
		    BITSLOTBYNO(input, end_slot - 1) >> start_offset;
		storage_unit mask = (STORAGE_UNIT_PREFIX(1) << (end_offset + 1)) - STORAGE_UNIT_PREFIX(1);	// mask cannot be full, so no need to check
		BITSLOTBYNO(output, BITSLOTNR(end - start - 1)) |=
		    (BITSLOTBYNO(input, end_slot) & mask) << (STORAGE_UNIT_SIZE -
							  start_offset);
	} else if (start_offset == 0) {
		storage_unit mask =
		    (end_offset ==
		     STORAGE_UNIT_SIZE -
		     1) ? -STORAGE_UNIT_PREFIX(1) : (STORAGE_UNIT_PREFIX(1) <<
						     (end_offset + 1)) -
		    STORAGE_UNIT_PREFIX(1);
		BITSLOTBYNO(output, BITSLOTNR(end - start - 1)) =
		    (BITSLOTBYNO(input, end_slot) & mask);
	} else {
		storage_unit mask = (STORAGE_UNIT_PREFIX(1) << (end_offset - start_offset + 1)) - STORAGE_UNIT_PREFIX(1);	/* +1, because end_offset is the bit _included_ in selection */
		BITSLOTBYNO(output, BITSLOTNR(end - start - 1)) |=
		    (BITSLOTBYNO(input, end_slot) & (mask << start_offset)) >>
		    start_offset;
	}
	return output;
}

struct bitfield *bfnormalize(const struct bitfield *input)
{
	unsigned int size = BFSIZE(input);
	unsigned int bitnslots = BITNSLOTS(size);
	unsigned int length_last_chunk = BFTAILSIZE(input);
	/* pick first potential candidate for output */
	struct bitfield *output = bfclone(input);
	if (!output)
		return NULL;
	/* will compare bitfields in chunks of length 1 storage_unit */
	storage_unit chunk_a, chunk_b;
	struct bitfield *tmp1, *tmp2;
	/* counters for bit offsets and slots/chunks/longs */
	int i, j;
	/* shift input string 1 position at a time and compare with best candidate (size - 1 comparisons) */
	for (i = 1; i <= size - 1; i++) {
		/* compare 1 slot/long at a time */
		for (j = bitnslots - 1;; j--) {
			/* special check for tail chunks (may be underfull) */
			if (j == bitnslots - 1 && length_last_chunk != 0) {
				/* this can probably be optimized */
				chunk_a = BITSLOTBYNO(output, j);
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
				chunk_b = BITSLOTBYNO(tmp2, 0);
				bfdel(tmp1);
				bfdel(tmp2);
			} else {
				/* this can probably be optimized */
				chunk_a = BITSLOTBYNO(output, j);
				tmp1 = bfshift(input, -i);
				if (!tmp1) goto error;
				chunk_b = BITSLOTBYNO(tmp1, j);
				bfdel(tmp1);
			}
			/* compare. if a is greater, offset i becomes new best candidate. move to next i */
			if (chunk_a > chunk_b) {
				tmp1 = bfshift(input, -i);
				if (!tmp1) goto error;
				bfdel(output);
				output = tmp1;
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
