# The unifont.bin File Format

Up to April of 2019 the unifont.bin file was just a straight concatenation of glyph data and width bitmasks, with indexes into the file hard-coded in `glcdfont.c`. In the interest of making this format more general and future-proof, I'm adopting a more specific format that people can use to encode other Unicode fonts for microcontrollers. This document outlines that format.

## Bytes `0` through `7`: Font Header

This section describes global information about the font. Values are unsigned decimal integers unless hex or binary notation is given.

| Offset | Length | Typical Value | Contents |
|--------|--------|---------------|----------|
| 0      | 2      | 0x0000        | **Reserved**: two zero bytes. CircuitPython looks for width and height in these fields; `00 00` can serve as an indicator that this is a Unicode font, or we can use this for something else later. |
| 2      | 1      | 8             | Width of a standard-sized glyph. Marked as _w_ in the tables below. For the moment, the only supported value is 8. |
| 3      | 1      | 16            | Height of a standard-sized glyph. Marked as _h_ in the tables below. For the moment, the only supported value is 16. |
| 4      | 1      | 0b00000001    | Font-wide flags. Currently only the least significant bit matters: if flags & 1, the font has both single- and double-width glyphs, and a _multiplier_ of 2 is applied to any blocks that are not exclusively single-width. |
| 5      | 1      | 5             | Number of bitmasks (_numBitmasks_) available for each block. At the end of each block's glyph data, there are this many 32-byte (256-bit) bitmasks that describe some attribute of every character in the font. Currently we have five, described below.
| 6      | 2      | 223           | Number of blocks included in this file. Marked as _numBlocks_ in the following tables. |
| 8      | -      | -             | End of font header. |

## Bytes 8 through the start of font data : Block Headers

Each entry in this section is four bytes long and encodes information about the Unicode block that resides at this index.

| Offset              | Length | Typical Value | Contents |
|---------------------|--------|---------------|----------|
| 8                   | 1      | 0x00 - 0xFF   | The Unicode block at this index. For example, both the block "Arabic" (U+0600 - U+06FF) and the block "Linear A" (U+10600 to U+106FF) would have 0x06 in this field. |
| 9                   | 1      | 0x00 or 0x01  | The Unicode plane for the block at this index. The Basic Multilingual Plane is Plane 0; the Supplementary Multilingual Plane is Plane 1. There are 17, all told. In this example, the block "Arabic" (U+0600 - U+06FF) would have 0x00 in this field since it's block 06 in the BMP, whereas "Linear A" (U+10600 to U+106FF) would have 0x01 since it's block 06 in the SMP. |
| 10                  | 1      | 0b0000????    | Flags specific to this block. See Flags below. |
| 11                  | 1      | 0             | **Reserved for future use.** This wastes about 223 bytes but leaves plenty of room to expand in the future if need be. |
| ...                 | ...    | ...           | ... |
| 8 + (numBlocks * 4) | -      | -             | End of block headers, start of font data. |

Note that the value at offsets 8-9 can be read as a little-endian short.

### Flags

The flags byte encodes information that applies to the entire block. Currently four flags are defined:

| Flag position     | Meaning |
|-------------------|---------|
| 0b00000001 (1<<0) | This block contains non-spacing marks. You will need to check the spacing table to determine whether to advance. |
| 0b00000010 (1<<1) | This block contains exclusively single-width glyphs. You do not need to check the widths table; all spacing characters advance by _w_ pixels. |
| 0b00000100 (1<<2) | This block contains exclusively double-width glyphs. You do not need to check the widths table; all spacing characters advance by _w * multiplier_ pixels . |
| 0b00001000 (1<<3) | This block contains glyphs that can be mirrored when drawn in a right-to-left text run. If you are in RTL mode, you must check the mirroring table to determine whether to mirror the glyph. |
| 0b10000000 (1<<7) | **Reserved for internal use**. Once the font metadata is loaded into memory, the Arduino library uses this bit for internal purposes. Its value will be discarded, but should be set to 0. |

If both of the width flags are 0 (i.e. 0b00000000), the block contains a mixture of single- and double-width glyphs. You will need to check the width bitmasks at the end of the file to determine the appropriate advance. It is an invalid condition for a block to have both of the width flags set to 1 (i.e. 0b00000110); implementations may choose to silently ignore codepoints in blocks configured this way.

## The Font Data

This is simply a concatenation of 256 glyphs per block for each of the blocks, with _numBitmasks_ bitmasks at the end of each. For exclusively single-width blocks, a character is `w*h/8` bytes long (16 bytes for Unifont). For double- or mixed-width blocks, a character is `w*h*multiplier/8` bytes (for Unifont, that's 32).

There are always 256 glyphs in a page, giving a page width of 4096 or 8192 for Unifont.

The following table shows the offset for the first block in Unifont.bin. Block 0 is single-width, so its glyphs are 16 bytes long and its bitmasks begin 4096 bytes after `FONTDATA_START` (which is`8+(numBlocks*4)` or `900`).

| Offset                                    | Length | Contents |
|-------------------------------------------|--------|----------|
| `FONTDATA_START`                          | 16     | Glyph for codepoint 00 in the block described at index 0. |
| `FONTDATA_START` + 16                     | 16     | Glyph for codepoint 01 in the block described at index 0. |
| `FONTDATA_START` + 32                     | 16     | Glyph for codepoint 02 in the block described at index 0. |
| ...                                       | ...    | ... |
| `FONTDATA_START` + 4080                   | 16     | Glyph for codepoint FF in the block described at index 0. |
| `FONTDATA_START` + 4096                   | 32     | Spacing bitmask for codepoints in the block described at index 0. |
| `FONTDATA_START` + 4128                   | 32     | Width bitmask for codepoints in the block described at index 0. |
| `FONTDATA_START` + 4160                   | 32     | LTR bitmask for codepoints in the block described at index 0. |
| `FONTDATA_START` + 4192                   | 32     | RTL bitmask for codepoints in the block described at index 0. |
| `FONTDATA_START` + 4224                   | 32     | RTL mirroring bitmask for codepoints in the block described at index 0. |
| `FONTDATA_START` + 4256                   | 16     | Glyph for codepoint 00 in the block described at index 1. |
| `FONTDATA_START` + 4272                   | 16     | Glyph for codepoint 01 in the block described at index 1. |
| `FONTDATA_START` + 4288                   | 16     | Glyph for codepoint 02 in the block described at index 1. |
| ...                                       | ...    | ... |

On and on, except that double-width blocks will have a glyph length of 32. With the width, height, multiplier, flags and number of bitmasks for each block, it is simple to loop through once and calculate the offset for each block when loading the font, and then cache those values for quick reference.

### Bitmasks

Each bitmask field is 32 bytes long, describing 8 glyphs per byte:

|           | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  | ... |
|-----------|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|-----|
| Character | 00 | 01 | 02 | 03 | 04 | 05 | 06 | 07 | 08 | 09 | 0A | 0B | 0C | 0D | 0E | 0F | ... |

Given a character index `charindex` within a block, you can calculate the byte you need to check with `index = BITMASK_START + charindex / 8`. Then test the bit like so: `state = bitmasks[index] & (1 << (7 - charindex % 8))`. The meaning of a bit in each bitmask is described below.

| Table     | Meaning |
|-----------|---------|
| Spacing   | 1 if the cursor should advance after drawing the glyph; 0 if it should stay in the same place. |
| Width     | 0 if the character is _w_ pixels wide; 1 if the character is _w*2_ pixels wide. |
| LTR       | 1 if the character should force us into left-to-right mode. |
| RTL       | 1 if the character should force us into right-to-left mode. **NOTE:** If both LTR and RTL are 0, the character can be laid out in either direction and does not force any mode change. It is an invalid condition for both RTL and LTR to be set to 1. |
| Mirroring | 1 if the character should be mirrored in right-to-left mode. |

So, for example, here are the bitmasks for Block 0:

    Spacing : ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
    Width   : 0000000000000000000000000000000000000000000000000000000000000000
    LTR     : 00000000000000007fffffe07fffffe00000000000200420fffffefffffffeff
    RTL     : 0000000000000000000000000000000000000000000000000000000000000000
    Mirror  : 0000000000c0000a000000140000001400000000001000100000000000000000

You can see that all of the codepoints in this block are spacing marks, all of them are single-width, and none of them force RTL layout direction. Some of the glyphs force LTR layout direction, and some of them support bidirectional mirroring.
