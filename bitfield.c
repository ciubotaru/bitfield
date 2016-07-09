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
#include <assert.h>
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

/*
 * Convert integer data types, all unsigned, to bitfield structures, with
 * in-place equivalents:
 * char
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

struct bitfield *int2bf(const unsigned int *input, int size)
{
	struct bitfield *output = bfnew(size);
	int bitnslots = (size - 1) / INT_BIT + 1;
	memcpy(output->field, input, bitnslots * sizeof(unsigned int));
	/**
	 * clear the tail, in case bfnew created a bitfield with non-zeroes AND
	 * memcpy did not cover the end of bitfield memory.
	 **/
	bfcleartail(output);
	return output;
}

void int2bf_ip(const unsigned int *input, struct bitfield *output)
{
	int size = bfsize(output);
	int bitnslots = (size - 1) / INT_BIT + 1;
	memcpy(output->field, input, bitnslots * sizeof(unsigned int));
	return;
}

struct bitfield *long2bf(const unsigned long *input, int size)
{
	struct bitfield *output = bfnew(size);
	int bitnslots = BITNSLOTS(size);
	memcpy(output->field, input, bitnslots * sizeof(unsigned long));
	return output;
}

void long2bf_ip(const unsigned long *input, struct bitfield *output)
{
	int size = bfsize(output);
	int bitnslots = BITNSLOTS(size);
	memcpy(output->field, input, bitnslots * sizeof(unsigned long));
	return;
}

/*
 * Convert bitfield structures to integer data types, all unsigned, with 
 * in-place equivalents:
 * char
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

unsigned int *bf2int(const struct bitfield *input)
{
	int bitnslots = (input->size - 1) / INT_BIT + 1;
	unsigned int *output = calloc(1, bitnslots * sizeof(unsigned int));
	memcpy(output, input->field, bitnslots * sizeof(unsigned int));
	return output;
}

void bf2int_ip(const struct bitfield *input, unsigned int *output)
{
	int bitnslots = (input->size - 1) / INT_BIT + 1;
	memcpy(output, input->field, bitnslots * sizeof(unsigned int));
	return;
}

unsigned long *bf2long(const struct bitfield *input)
{
	int bitnslots = BITNSLOTS(input->size);
	unsigned long *output = calloc(1, bitnslots * sizeof(unsigned long));
	memcpy(output, input->field, bitnslots * sizeof(unsigned long));
	return output;
}

void bf2long_ip(const struct bitfield *input, unsigned long *output)
{
	int bitnslots = BITNSLOTS(input->size);
	memcpy(output, input->field, bitnslots * sizeof(unsigned long));
	return;
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

struct bitfield *bfcat(const struct bitfield *input1,
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
	if (offset_bit != 0) {
		for (i = 0; i < BITNSLOTS(input2->size); i++) {
			output->field[i + offset_slot + 1] |= (input2->field[i] >> (LONG_BIT - offset_bit));	// what if output doesn't stretch to i + offset_slot + 1???
		}
	}
	return output;
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
	int hamming = bfpopcount(bfxor(input1, input2));
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

void bfprint(const struct bitfield *instance)
{
	int i;
	for (i = 0; i < instance->size; i++)
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
	/* positive offset moves the last offset characters to the beginning */
	int bitnslots = BITNSLOTS(input->size);
	/* removing extra rotations */
	int offset_internal = offset % input->size;
	if (offset_internal == 0) {
		return;		/* nothing to shift */
	}
	/* changing a negative offset to a positive equivalent */
	if (offset < 0)
		offset_internal += input->size;
	struct bitfield *first_chunk =
	    bfsub(input, 0, input->size - offset_internal);
	struct bitfield *second_chunk =
	    bfsub(input, input->size - offset_internal, input->size);

	struct bitfield *tmp = bfcat(second_chunk, first_chunk);
	memcpy(input->field, tmp->field, bitnslots * sizeof(unsigned long));
	bfdel(tmp);
}

struct bitfield *bfshift(const struct bitfield *input, const int offset)
{
	/* positive offset moves the last offset characters to the beginning */
	int bitnslots = BITNSLOTS(input->size);
	/* removing extra rotations */
	int offset_internal = offset % input->size;
	struct bitfield *output = bfnew(input->size);
	if (offset_internal == 0) {
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

	struct bitfield *tmp = bfcat(second_chunk, first_chunk);
	memcpy(output->field, tmp->field, bitnslots * sizeof(unsigned long));
	bfdel(first_chunk);
	bfdel(second_chunk);
	bfdel(tmp);
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
	/* counters for bit offsets and slots/chunks/longs */
	int i, j;
	/* shift input string 1 position at a time and compare with best candidate (size - 1 comparisons) */
	for (i = 1; i <= size - 1; i++) {
		/* compare 1 slot/long at a time */
		for (j = bitnslots - 1; j >= 0; j--) {
			/* special check for tail chunks (may be underfull) */
			if (j == bitnslots - 1 && length_last_chunk != 0) {
				chunk_a = output->field[j];
				chunk_b = bfsub(bfshift(input, length_last_chunk + i), 0, length_last_chunk)->field[0]; /* this can be optimized */
			} else {
				chunk_a = output->field[j];
				chunk_b = bfshift(input, i)->field[j];  /* this can be optimized */
			}
			/* compare. if a is greater, offset i becomes new best candidate. move to next i */
			if (chunk_a > chunk_b) {
				output = bfshift(input, i);
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
