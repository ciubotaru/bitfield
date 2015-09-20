bitfield Version 0.1 September 1, 2015
======================================

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

    $ make check

The generic way to install the compiled files is:

    $ make install
    $ ldconfig

However, one might prefer to use distro-specific installation mechanism (like 
"checkinstall") instead.

Usage
-----

Using the functions provided by bitfield library in a project is straightforward.
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

Functions
---------

For function syntax, see "bitfield.h". For details on every function, see its 
manual page.

bf2char() converts a bitfield into a character string of ones and zeroes, 
starting from the least significant bit.

bfand() performs bitwise AND over a pair of bitfields.

bfcat() concatenates two bitfields into one.

bfclone() creates a copy of an existing bitfield.

bfcmp() compares two bitfields and returns 0 if same or non-zero and error 
message if different.

bfdel() destroys a bitfield structure and frees memory.

bfnew() creates an empty bitfield structure and returns a pointer to it.

bfnot() reverses all bits in a bitfield and return the result in new bitfield.

bfnot_ip() reverses all bits in a bitfield "in place".

bfor() performs bitwise inclusive OR over a pair of bitfields.

bfprint() prints a bitfield as a series of ones and zeroes.

bfshift() circular-shifts the contents of a bitfield and return the result in 
new bitfield.

bfshift_ip() circular-shifts the contents of a bitfield "in place".

bfsub() extracts a sub-bitfield from a bitfield.

bfxor() performs bitwise exclusive OR over a pair of bitfields.

char2bf() converts a character string of ones and zeroes into a bitfield, 
starting from the least significant bit.

Please, see "examples" directory for working examples.

Licensing
---------

bitfield is free software, and is released under the terms of the GNU General 
Public License version 3. Please see the file called LICENSE.
