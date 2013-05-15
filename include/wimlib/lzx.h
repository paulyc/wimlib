#ifndef _WIMLIB_LZX_H
#define _WIMLIB_LZX_H

#include "wimlib/assert.h"
#include "wimlib/types.h"

//#define ENABLE_LZX_DEBUG
#ifdef ENABLE_LZX_DEBUG
#	define LZX_DEBUG DEBUG
#else
#	define LZX_DEBUG(format, ...)
#endif

/* Constants, most of which are defined by the LZX specification: */

/* The smallest and largest allowed match lengths. */
#define LZX_MIN_MATCH                2
#define LZX_MAX_MATCH                257

/* Number of values an uncompressed literal byte can represent. */
#define LZX_NUM_CHARS                256

/* Each LZX block begins with 3 bits that determines the block type.  Below are
 * the valid block types.  Values 0, and 4 through 7, are invalid. */
#define LZX_BLOCKTYPE_VERBATIM       1
#define LZX_BLOCKTYPE_ALIGNED        2
#define LZX_BLOCKTYPE_UNCOMPRESSED   3

#define LZX_NUM_PRIMARY_LENS         7	/* this one missing from spec! */

/* NOTE: There are really 51 position slots in the LZX format as a whole, but
 * only 30 are needed to allow for the window to be up to 32768 bytes long,
 * which is the maximum in the WIM format. */
#define LZX_NUM_POSITION_SLOTS       30

/* Read the LZX specification for information about the Huffman trees used in
 * the LZX compression format.  Basically there are 4 of them: The main tree,
 * the length tree, the pre tree, and the aligned tree.  The main tree and
 * length tree are given at the beginning of VERBATIM and ALIGNED blocks as a
 * list of *_NUM_SYMBOLS code length values.  They are read using the
 * read_code_lens() function and built using the make_decode_table() function.
 * The decode table is not a real tree but rather a table that we can index by
 * some number of bits (*_TABLEBITS) of the input to quickly look up the symbol
 * corresponding to a Huffman code.
 *
 * The ALIGNED tree is only present on ALIGNED blocks.
 *
 * A PRETREE is used to encode the code lengths for the main tree and the length
 * tree.  There is a separate pretree for each half of the main tree.  */

#define LZX_MAINTREE_NUM_SYMBOLS	 (LZX_NUM_CHARS + \
					(LZX_NUM_POSITION_SLOTS << 3))
#define LZX_MAINTREE_TABLEBITS		11

#define LZX_LENTREE_NUM_SYMBOLS		249
#define LZX_LENTREE_TABLEBITS		10

#define LZX_PRETREE_NUM_SYMBOLS		20
#define LZX_PRETREE_TABLEBITS		6
#define LZX_PRETREE_ELEMENT_SIZE	4

#define LZX_ALIGNEDTREE_NUM_SYMBOLS	8
#define LZX_ALIGNEDTREE_TABLEBITS	7
#define LZX_ALIGNEDTREE_ELEMENT_SIZE 	3

/* Maximum allowed length of a Huffman code. */
#define LZX_MAX_CODEWORD_LEN		16

/* For the LZX-compressed blocks in WIM files, this value is always used as the
 * filesize parameter for the call instruction (0xe8 byte) preprocessing, even
 * though the blocks themselves are not this size, and the size of the actual
 * file resource in the WIM file is very likely to be something entirely
 * different as well.  */
#define LZX_WIM_MAGIC_FILESIZE		12000000

#define USE_LZX_EXTRA_BITS_ARRAY

#ifdef USE_LZX_EXTRA_BITS_ARRAY
extern const u8 lzx_extra_bits[LZX_NUM_POSITION_SLOTS];
#endif

/* Given the number of a LZX position slot, return the number of extra bits that
 * are needed to encode the match offset. */
static inline unsigned
lzx_get_num_extra_bits(unsigned position_slot)
{
#ifdef USE_LZX_EXTRA_BITS_ARRAY
	/* Use a table */
	return lzx_extra_bits[position_slot];
#else
	/* Calculate directly using a shift and subtraction. */
	wimlib_assert(position_slot >= 2 && position_slot <= 37);
	return (position_slot >> 1) - 1;
#endif
}

extern const u32 lzx_position_base[LZX_NUM_POSITION_SLOTS];

/* Least-recently used queue for match offsets. */
struct lru_queue {
	u32 R0;
	u32 R1;
	u32 R2;
};

#endif /* _WIMLIB_LZX_H */