/**
 * File name: bitfield.c
 * Project name: bitfield, a bit array manipulation library written in C
 * URL: https://github.com/ciubotaru/bitfield
 * Author: Vitalie Ciubotaru <vitalie at ciubotaru dot tk>
 * License: General Public License, version 3
 * Date: September 1, 2015
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "bitfield.h"
#include "bitfield-internals.h"

inline void bfcleartail(struct bitfield *);	/* sets unused bits to zero */

struct bitfield *bfnew(const int size)
{
	struct bitfield *instance = malloc(sizeof(struct bitfield));
	instance->size = size;
	instance->field = calloc(1, BITNSLOTS(size) * sizeof(unsigned long));
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

struct bitfield *bfnew_ones(const int size)
{
	struct bitfield *instance = malloc(sizeof(struct bitfield));
	instance->size = size;
	instance->field = malloc(BITNSLOTS(size) * sizeof(unsigned long));
	bfsetall(instance);
	return instance;
}

void bfdel(struct bitfield *instance)
{
	free(instance->field);
	free(instance);
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
	char *msg;
	if (input1->size != input2->size) {
		msg = "Can not compare bitfields of different length";
		*errmsg = malloc(strlen(msg) + 1);
		memcpy(*errmsg, msg, strlen(msg) + 1);
		return 2;
	}

	int i;
	int bitnslots = BITNSLOTS(input1->size);
	for (i = 0; i < (bitnslots - 1); i++) {	//leaving the last slot for later
		if (input1->field[i] != input2->field[i]) {
			goto differ;
		}
	}
/* comparing the last slot using mask, because the tail bits may differ */
	unsigned long mask =
	    ((input1->size % LONG_BIT) ==
	     0) ? -1UL : (1UL << input1->size % LONG_BIT) - 1UL;
	if ((input1->field[(input1->size - 1) / LONG_BIT] & mask) !=
	    ((input2->field)[(input2->size - 1) / LONG_BIT] & mask))
		goto differ;
	return 0;
 differ:
	msg = "Bitfields differ";
	*errmsg = malloc(strlen(msg) + 1);
	memcpy(*errmsg, msg, strlen(msg) + 1);
	return 1;
}

void bfprint(const struct bitfield *instance)
{
	int i;
	for (i = 0; i < instance->size; i++)
		printf("%lu",
		       (instance->field[i / LONG_BIT] >> (i % LONG_BIT)) & 1UL);
	printf("\n");
}

void char2bf(const char *input, struct bitfield *output)
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

void bf2char(const struct bitfield *input, char *output)
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
	bfdel(tmp);
	return output;
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

int bfcpy(const struct bitfield *src, struct bitfield *dest)
{
	if (src->size > dest->size)
		return 1;
	int i;
	for (i = 0; i < BITNSLOTS(src->size); i++)
		dest->field[i] = src->field[i];
	return 0;
}

void bfclearall(struct bitfield *instance)
{
	int i;
	for (i = 0; i < BITNSLOTS(instance->size); i++)
		instance->field[i] = 0UL;
}

void bfsetall(struct bitfield *instance)
{
	int i;
	for (i = 0; i < BITNSLOTS(instance->size); i++)
		instance->field[i] = -1UL;
	bfcleartail(instance);
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

void bfsetbit(struct bitfield *instance, int bit)
{
	BITSET(instance, bit);
}

void bfclearbit(struct bitfield *instance, int bit)
{
	BITCLEAR(instance, bit);
}

struct bitfield *bfrev(const struct bitfield *input)
{
	struct bitfield *output = bfnew_quick(input->size);
	int i, j;
	int bitnslots = BITNSLOTS(input->size);
	for (i = 0; i < (bitnslots - 1); i++)
	{
		for (j = 0; j < LONG_BIT; j++)
		{
			if ((input->field[i] >> j) & 1UL)
				BITSET(output, input->size - i * LONG_BIT - j - 1);
			else
				BITCLEAR(output, input->size - i * LONG_BIT - j - 1);
		}
	}
	int bits_in_last_input_slot = (input->size - 1) % LONG_BIT + 1;
	for (j = 0; j < bits_in_last_input_slot; j++)
	{
		if ((input->field[bitnslots - 1] >> j) & 1UL)
			BITSET(output, bits_in_last_input_slot - j - 1);
		else
			BITCLEAR(output, bits_in_last_input_slot - j - 1);
	}
	return output;
}

void bfrev_ip(struct bitfield *instance)
{
	int size = instance->size;
	struct bitfield *tmp = bfnew_quick(size);
	int i, j;
	int bitnslots = BITNSLOTS(size);
	for (i = 0; i < (bitnslots - 1); i++)
	{
		for (j = 0; j < LONG_BIT; j++)
		{
			if ((instance->field[i] >> j) & 1UL)
				BITSET(tmp, size - i * LONG_BIT - j - 1);
			else
				BITCLEAR(tmp, size - i * LONG_BIT - j - 1);
		}
	}
	int bits_in_last_input_slot = (size - 1) % LONG_BIT + 1;
	for (j = 0; j < bits_in_last_input_slot; j++)
	{
		if ((instance->field[bitnslots - 1] >> j) & 1UL)
			BITSET(tmp, bits_in_last_input_slot - j - 1);
		else
			BITCLEAR(tmp, bits_in_last_input_slot - j - 1);
	}
	memcpy(instance->field, tmp->field, bitnslots * sizeof(unsigned long));
	bfdel(tmp);
}

int bfsize(const struct bitfield *instance)
{
	return instance->size;
}

int bfgetbit(const struct bitfield *instance, const int bit)
{
	/* might be good to check whether bit is within range */

	return BITGET(instance, bit);
}

void bftogglebit(struct bitfield *instance, const int bit)
{
	BITTOGGLE(instance, bit);
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

int bfhamming(const struct bitfield *input1, const struct bitfield *input2)
{
	int hamming = bfpopcount(bfxor(input1, input2));
	return hamming;
}
