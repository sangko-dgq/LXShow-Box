import binascii

class CodepointInfo:
    def __init__(self):
        self.isNSM  = False
        self.isRTL  = False
        self.isLTR  = False
        self.isMirrored = False

class UnicodeBlock:
    def __init__(self, block_number):
        self.number = int(block_number, 16)
        self.include_in_progmem = block_number == '00'
        self.is_short_block = block_number in short_blocks
        self.glyphs = list()
        self.block_width_mode = None
        self.has_nonspacing_marks = False
        self.has_direction_changes = False
        self.has_mirroring = False
        self.spacings = [0] * 32
        self.rtl = [0] * 32
        self.ltr = [0] * 32
        self.widths = [0] * 32
        self.mirroring = [0] * 32
        
    def flags(self):
        flags = 0
        if self.has_nonspacing_marks:
            flags |= 1 << 0
        if self.block_width_mode == 1:
            flags |= 1 << 1
        elif self.block_width_mode == 2:
            flags |= 1 << 2
        if self.has_mirroring:
            flags |= 1 << 3
        if self.include_in_progmem:
            flags |= 1 << 7
        return flags

    def __repr__(self):
        return "\nBlock {}: {} {} glyphs with width mode {}\nWidth:   {}\nSpacing: {}\nLTR    : {}\nRTL    : {}\nMirror : {}".format(hex(self.number)[2:].upper(), bin(self.flags()), len(self.glyphs), self.block_width_mode, binascii.hexlify(bytearray(self.widths)), binascii.hexlify(bytearray(self.spacings)), binascii.hexlify(bytearray(self.ltr)), binascii.hexlify(bytearray(self.rtl)), binascii.hexlify(bytearray(self.mirroring)))


short_blocks = ["00", "01", "02", "1E", "1F", "28"]

bidi_info = dict()

for line in open(r'C:\Users\liyue\Documents\Arduino\libraries\Adafruit-GFX-Library\unifontconvert\UnicodeData.txt', 'r'):
    line = line.strip().split(";")
    if len(line) < 5:
        break
    codepoint = line[0]
    general_category = line[2]
    bidi_class = line[4]
    bidi_mirrored = line[9]
    info = CodepointInfo()
    info.isNSM = general_category in ["Mn", "Mc", "Me"] or bidi_class == "NSM"
    info.isLTR = bidi_class in ["L", "LRE", "LRO", "LRI"]
    info.isRTL = bidi_class in ["R", "AL", "RLE", "RLO", "RLI"]
    info.isMirrored = bidi_mirrored == "Y"
    bidi_info[codepoint] = info

blocks = dict()
current_block = None
has_single_width_glyphs = False
has_double_width_glyphs = False
force_ltr = False

for line in open(r'C:\Users\liyue\Documents\Arduino\libraries\Adafruit-GFX-Library\unifontconvert\unifont.hex', 'r'):
    line = line.strip().split(":")
    if len(line) < 2:
        break
    codepoint = line[0]
    data = line[1]
    block_name = codepoint[:2]
    char_hex = codepoint[2:]
    char_num = int(char_hex, 16)

    # handle moving to the next block
    if block_name not in blocks:
        # do housekeeping on the block we just closed
        if current_block is not None:
            if current_block.is_short_block:
                # hard-coded from experience, only six blocks are exclusively single-width
                # the conditional would be: if has_single_width_glyphs and not has_double_width_glyphs
                current_block.block_width_mode = 1
            elif has_single_width_glyphs and has_double_width_glyphs:
                current_block.block_width_mode = 0
            else:
                current_block.block_width_mode = 2
        # now create the new block and clear previous state
        blocks[block_name] = UnicodeBlock(block_name)
        current_block = blocks[block_name]
        has_single_width_glyphs = False
        has_double_width_glyphs = False

    if data[:8] == "00007FFE":
        if current_block.is_short_block:
            current_block.glyphs.append(bytearray(b'\xff') * 16)
        else:
            current_block.glyphs.append(bytearray(b'\xff') * 32)
    elif data[:8] == "AAAA0001":
        if current_block.is_short_block:
            current_block.glyphs.append(bytearray(b'\x00') * 16)
        else:
            current_block.glyphs.append(bytearray(b'\x00') * 32)
    else:
        current_block.glyphs.append(bytes.fromhex(data))

    if len(data) > 32 and not current_block.is_short_block:
        has_double_width_glyphs = True
        current_block.widths[char_num // 8] |= 1 << (7 - char_num % 8)
    else:
        has_single_width_glyphs = True

    # this check looks a little backwards because the bit is set to 0 for non spacing marks and 1 for spacing marks.
    if codepoint in bidi_info and bidi_info[codepoint].isNSM:
        current_block.has_nonspacing_marks = True
    else:
        current_block.spacings[char_num // 8] |= 1 << (7 - char_num % 8)

    # UnicodeData.txt has some ranges (3400-4DB5, 4E00-9FEF, AC00-D7A3) where they don't
    # provide lines for each because the same properties apply to all. We only care that
    # they're all LTR. This will apply until the last glyph in the range is encountered.
    if codepoint in ["3400", "4E00", "AC00"]:
        force_ltr = True

    if codepoint in bidi_info and bidi_info[codepoint].isRTL:
        current_block.has_direction_changes = True
        current_block.rtl[char_num // 8] |= 1 << (7 - char_num % 8)
    if force_ltr or codepoint in bidi_info and bidi_info[codepoint].isLTR:
        current_block.has_direction_changes = True
        current_block.ltr[char_num // 8] |= 1 << (7 - char_num % 8)

    # End of LTR override.
    if codepoint in ["4DB5", "9FEF", "D7A3"]:
        force_ltr = False

    if codepoint in bidi_info and bidi_info[codepoint].isMirrored:
        current_block.has_mirroring = True
        current_block.mirroring[char_num // 8] |= 1 << (7 - char_num % 8)


# block FF needs two more glyphs to make all the tables the same length.
current_block.glyphs.append(bytearray(b'\xff') * 32)
current_block.glyphs.append(bytearray(b'\xff') * 32)
# finally the little bit of housekeeping we missed on the last iteration of the loop:
current_block.spacings[31] |= 1 << 1
current_block.spacings[31] |= 1
current_block.block_width_mode = 0

def toggle_progmem_blocks():
    done = False
    while not done:
        for block_name in blocks:
            block = blocks[block_name]
            print("{}: {}".format(block_name, '✅' if block.include_in_progmem else '❌'), end = '\t')
            if (block.number + 1) % 16 == 0:
                print()
        block_to_toggle = input("Toggle which block? ('done' when finished) ").upper()
        # add leading zero if it was missing
        if len(block_to_toggle) == 1:
            block_to_toggle = "0{}".format(block_to_toggle)
        if block_to_toggle == '00':
            print("Block 00 must always reside in PROGMEM.")
        elif block_to_toggle == 'DONE':
            return
        elif block_to_toggle not in blocks:
            print("Invalid block.")
        else:
            blocks[block_to_toggle].include_in_progmem ^= True

def generate_unifont_c():
    outfile = open(r'C:\Users\liyue\Documents\Arduino\libraries\Adafruit-GFX-Library\unifontconvert\glcdfont.c', 'w')
    print("""#ifndef FONT8x16_C
#define FONT8x16_C

#include "glcdfont.h"

#ifdef __AVR__
 #include <avr/io.h>
 #include <avr/pgmspace.h>
#elif defined(ESP8266)
 #include <pgmspace.h>
#else
 #define PROGMEM
#endif

#include <stdint.h>

// GNU Unifont 8x16 font
""", file=outfile)
    for block_name in blocks:
        block = blocks[block_name]
        if block.include_in_progmem:
            print("static const uint8_t block_{}_data[] PROGMEM = {{".format(block_name), file=outfile)
            char_num = 0
            for glyph in block.glyphs:
                if block_name == '00' and char_num < 0x20:
                    char_num += 1
                    continue
                glyph_text = binascii.hexlify(glyph).decode('ascii').upper()
                bytes = [glyph_text[i:i+2] for i in range(0, len(glyph_text), 2)]
                print("    ", end='', file=outfile)
                for byte in bytes:
                    print("0x{}".format(byte), end=", ", file=outfile)
                if len(glyph) < 32 and block.block_width_mode == 0:
                    # it's a waste of space but we have to pad out these blocks so they're easily indexed.
                    for i in range(0, 16):
                        print("0x00", end=", ", file=outfile)
                print(" // Code point {}{:02X}".format(block_name, char_num), file=outfile)
                char_num += 1
            if block.number == 0:
                pass
            elif block.has_nonspacing_marks or block.block_width_mode == 0 or block.has_direction_changes or block.has_mirroring:
                # we need to include all bitmasks if any need to be consulted
                print("\n    // Character spacing bitmasks\n    ", end='', file=outfile)
                for byte in block.spacings:
                    print("0x{:02X}".format(byte), end=", ", file=outfile)
                print("\n    // Character width bitmasks\n    ", end='', file=outfile)
                for byte in block.widths:
                    print("0x{:02X}".format(byte), end=", ", file=outfile)
                print("\n    // LTR bitmasks\n    ", end='', file=outfile)
                for byte in block.ltr:
                    print("0x{:02X}".format(byte), end=", ", file=outfile)
                print("\n    // RTL bitmasks\n    ", end='', file=outfile)
                for byte in block.rtl:
                    print("0x{:02X}".format(byte), end=", ", file=outfile)
                print("\n    // RTL Mirroring bitmasks\n    ", end='', file=outfile)
                for byte in block.mirroring:
                    print("0x{:02X}".format(byte), end=", ", file=outfile)
            print("\n};\n", file=outfile)

    print("const UnifontInclusion BlocksInProgmem[] = {", file=outfile)

    for block_name in blocks:
        block = blocks[block_name]
        if block.include_in_progmem:
            print("    {0x", end='', file=outfile)
            print("{}".format(block_name), end='', file=outfile)
            print(", {block_", end='', file=outfile)
            print("{}".format(block_name), end='', file=outfile)
            print("_data, ", end='', file=outfile)
            print("{:#010b}}}".format(block.flags()), end='', file=outfile)
            print("},", file=outfile)
    print("};", file=outfile)
    print("\n#endif // FONT8x16_C", file=outfile)


def generate_unifont_bin():
    output = bytearray()

    # Global Font Header
    output.append(0)           # Reserved byte
    output.append(0)           # Reserved byte
    output.append(8)           # Glyph width in pixels
    output.append(16)          # Glyph height in pixels
    output.append(1)           # Flags. 0b00000001 indicates the presence of double-width glyphs.
    output.append(5)           # Number of bitmasks per block. We have spacing, width, LTR, RTL and mirroring.
    output.append(len(blocks)) # Number of blocks in font, lower byte of two-byte short.
    output.append(0)           # upper byte of numBlocks

    # Block Headers
    for block_name in blocks:
        block = blocks[block_name]
        output.append(block.number) # Unicode block number within plane.
        output.append(0)            # Unicode plane number. This script only handles Plane 0.
        output.append(block.flags() & 0x7F)  # Flags for this block. Zero out high bit.
        output.append(0)            # Reserved for future use.

    # Font Data
    for block_name in blocks:
        block = blocks[block_name]
        for glyph in block.glyphs:
            output += glyph
            if len(glyph) < 32 and block.block_width_mode == 0:
                orig_len = len(output)
                for i in range(0, 16):
                    output += b'\x00'
        output += bytearray(block.spacings)
        output += bytearray(block.widths)
        output += bytearray(block.ltr)
        output += bytearray(block.rtl)
        output += bytearray(block.mirroring)
    outfile = open('unifont.bin', 'wb')
    outfile.write(output)

while 1:
    print("\033[;1mUnifont Converter\033[0;0m\n▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔")
    print("Commands:")
    print("\t1. Print block information")
    print("\t2. Select blocks to include in glcdfont.c")
    print("\t3. Generate glcdfont.c")
    print("\t4. Generate unifont.bin")
    print("\tQ. Quit")
    command = input("What do you want to do? ")
    if command == '1':
        for block_name in blocks:
            print(blocks[block_name])
        print("\nLoaded data for {} blocks.\n\n".format(len(blocks)))
    elif command == '2':
        toggle_progmem_blocks()
    elif command == '3':
        generate_unifont_c()
    elif command == '4':
        generate_unifont_bin()
    elif command.upper() == 'Q':
        exit(0)
