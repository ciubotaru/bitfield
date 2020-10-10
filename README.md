bitfield
========

Version 1.0.1 (August, 2017)

bitfield is a library of functions for creating, modifying and destroying bit
fields (or bit arrays), i.e. series of zeroes and ones spread across an array
of storage units (unsigned long integers).

Installation
------------

To compile bitfield from source code:

    $ autoconf
    $ ./configure --prefix=DIRECTORY
    $ make

This will build both a static and a shared version. To build just one of them,
replace "make" with "make static" or "make shared".

To test the compiled library:

    $ make test

To build and run some example applications:

    $ make example

The generic way to install the compiled files is:

    $ make install
    $ ldconfig

However, one might prefer to use distro-specific installation mechanism (like
"checkinstall") instead.

Usage
-----

Using the functions provided by bitfield library in a project is
straightforward.
If bitfield library files are installed system-wide in standard locations, then 
one needs to (1) include a system version of the header file to the source

    #include <bitfield.h>

and (2) add the "-lbitfield" flag to compiler instructions

    gcc ... -lbitfield

Alternatively, if bitfield library is integrated into a project, then one needs
to (1) include a local version of the header file to the source

    #include "PATH_TO_HEADER/bitfield.h"

and (2) add the "-lbitfield" flag, along with the path to the header and path
to the library to compiler instructions

    gcc ... -I$(PATH_TO_HEADER) -L$(PATH_TO_LIBRARY) -lbitfield

Bitfield structure
------------------

Bit arrays are stored in data structures called 'bitfield'. A bitfield structure
has two elements: an array of unsigned long integers 'field' for storing the bits
and an integer 'size' for storing the number of bits in the array. This library
provides APIs for accessing and modifying bit arrays (see 'Functions').

Functions
---------

For function syntax, see "bitfield.h". For details on every function, see its
manual page.

Creating and deleting bit arrays:

bfnew() creates an empty bitfield structure, and returns a pointer to it

bfnew_ones() creates a bitfield structure, sets all its bits and returns a
pointer to it

bfnew_quick() creates a bitfield structure and returns a pointer to it

bfdel() destroys a bitfield structure and frees memory

Operations with bit arrays:

bfresize() resizes an existing bitfield

bfcat() concatenates several bitfields into one

bfclearall() clears all bits in a bitfield (i.e. fills it with zeroes)

bfclone() creates a copy of an existing bitfield

bfcpy() copies the contents of a bitfield into another pre-existing bitfield

bffs() finds the first set bit in a bitfield

bffz() finds the first clear bit in a bitfield

bfls() finds the last set bit in a bitfield

bflz() finds the last clear bit in a bitfield

bfclo() counts leading ones in a bitfield

bfhamming() counts the Hamming distance between two bitfields

bfisempty() checks whether all bit of an array are unset

bfnormalize() represents a bitfield as a smallest value of a closed ring

bfpopcount() counts the set bits in a bitfield

bfpos() checks whether an array of bits contains a sub-array

bfrev() reverses the order of bits in a bitfield and returns result in new
bitfield

bfrev_ip() reverses the order of bits in an existing bitfield

bfsetall() sets all bits in a bitfield (i.e. fills it with ones)

bfshift() circular-shifts the contents of a bitfield and returns the result in
new bitfield

bfshift_ip() circular-shifts the contents of an existing bitfield

bfsub() extracts a sub-bitfield in a new bitfield

Operations with individual bits:

bfgetbit() checks the state of a bit in a bitfield

bfsetbit() sets one bit in a bitfield

bfclearbit() clears one bit in a bitfield

bftogglebit() toggles (i.e. reverses the state of) a bit in a bitfield

Printing bit arrays:

bfprint_lsb() prints a bitfield as a series of ones and zeroes, left to right, the
least significant bit first

bfprint_msb() prints a bitfield as a series of ones and zeroes, left to right, the
most significant bit first

Logical operations with bit arrays:

bfand() performs bitwise AND over a pair of bitfields

bfnot() reverses all bits in a bitfield and return the result in a new bitfield

bfnot_ip() reverses all bits in an existing bitfield

bfor() performs bitwise inclusive OR over a pair of bitfields

bfxor() performs bitwise exclusive OR over a pair of bitfields

bfcmp() compares two bitfields and returns 0 if same or non-zero and error
message if different

Converting between different data types:

These functions convert between bitfield structure and standard data types.
Supported data types are: unsigned char, unsigned short, unsigned int, unsigned
long, unsigned long long, uint8_t, uint16_t, uint32_t and uint64_t. Unsigned
chars can be treated in two ways. Functions with "str" in their name treat each
char as a storage of one character, '0' or '1', while functions with "char" in
their name treat each char as a storage of several (usually 8) bits.

Bitfield-to-something functions:

bf2char() converts a bitfield structure into an array of unsigned chars

bf2str() converts into a character string of '1's and '0's

bf2short() converts into an array of short integers

bf2int() converts into an array of integers

bf2long() converts into an array of long integers

bf2ll() converts into an array of long long integers

bftouint8() converts into an array of uint8_t

bftouint16() converts into an array of uint16_t

bftouint32() converts into an array of uint32_t

bftouint64() converts into an array of uin64_t

"In-place" bitfield-to-something functions are same as above, except that
instead of creating a new array, these functions fill an existing one:

bf2char_ip()

bf2str_ip()

bf2short_ip()

bf2int_ip()

bf2long_ip()

bf2ll_ip()

bftouint8_ip()

bftouint16_ip()

bftouint32_ip()

bftouint64_ip()

"Something-to-bitfield" functions:

char2bf() converts an array of unsigned chars into a bitfield structure

str2bf() converts a character string of '1's and '0's

short2bf() converts an array of short integers

int2bf() converts an array of integers

long2bf() converts an array of long integers

ll2bf() converts an array of long long integers

uint8tobf() converts an array of uint8_t

uint16tobf() converts an array of uint16_t

uint32tobf() converts an array of uint32_t

uint64tobf() converts an array of uint64_t

"In-place" something-to-bitfield functions are same as above, except that
instead of creating a new bitfield, these functions fill an existing one:

char2bf_ip()

str2bf_ip()

short2bf_ip()

int2bf_ip()

long2bf_ip()

ll2bf_ip()

uint8tobf_ip()

uint16tobf_ip()

uint32tobf_ip()

uint64tobf_ip()

Miscellaneous functions:

bfsize() obtains the number of bits of a bitfield

Please, see "examples" directory for working examples.

Versioning
----------

The versioning scheme is MAJOR.MINOR.PATCH, where
* MAJOR version changes with incompatible API/ABI changes,
* MINOR version changes with backwards-compatible changes (like adding new functionality),
* PATCH version changes with backwards-compatible bug fixes.

Licensing
---------

bitfield is free software, and is released under the terms of the GNU General 
Public License version 3 or any later version. Please see the file called 
LICENSE.
