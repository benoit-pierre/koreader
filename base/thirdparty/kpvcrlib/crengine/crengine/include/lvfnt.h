/** \file lvfnt.h
    \brief Grayscale Bitmap Font engine

    CoolReader Engine

    (c) Vadim Lopatin, 2000-2006

    This source code is distributed under the terms of
    GNU General Public License.

    See LICENSE file for details.

   \section Unicode greyscale bitmap font file structure

   (pointers are byte offsets from file beginning)

   - <font file>::
      - <file header>      -- lvfont_header_t
      - <decode table>     -- huffman decode table
      - <glyph chunk 1>    -- lvfont_glyph_t * [64]
      - ...
      - <glyph chunk N>    -- lvfont_glyph_t * [64]
   - <file header>::
      - <signature>        -- 4 bytes
      - <version>          -- 4 bytes
      - <fontName>         -- 64 bytes
      - <copyright>        -- 64 bytes
      - <fileSize>         -- 4 bytes
      - <fontAttributes>   -- 8 bytes
      - <font ranges>      -- lvfont_glyph_t * * [1024], 0 if no chunk
   - <glyph chunk>::
      - <glyph table>      -- lvfont_glyph_t * [64], 0 if no glyph
      - <glyph 1>          -- lvfont_glyph_t [arbitrary size]
      - ...
      - <glyph M>          -- lvfont_glyph_t [arbitrary size]

*/

#ifndef __LVFNT_H_INCLUDED__
#define __LVFNT_H_INCLUDED__

#include "crsetup.h"

// These lower than 0x0100 (that fit in a lUint8) may be set by lvfntman's measureText()
// (to possibly get some informative flags back from harfbuzz) and hyphman's hyphenate().
// (These should be changed or dropped with care, as they may be used by some other parts of CoolReader)
#define LCHAR_IS_SPACE               0x0001 ///< flag: this char is one of the unicode space chars.
                                            //         It is set only on the normal space and the normal non-breakable
                                            //         space (spaces that can have their widths expanded or shrunk).
                                            //         It is not set on the unicode fixed width spaces.
#define LCHAR_ALLOW_WRAP_AFTER       0x0002 ///< flag: line break after this char is allowed.
                                            //         It is set on all spaces, except non-breakable ones.
                                            //         It is set on soft-hyphen.
                                            //         It is not set on CJK chars.
#define LCHAR_DEPRECATED_WRAP_AFTER  0x0004 ///< flag: line break after this char is possible but deprecated
                                            //         When not using libunibreak: it is set on '-' and other unicode hyphens.
                                            //         When using libunibreak: set on all text inside "white-space: nowrap"
#define LCHAR_ALLOW_HYPH_WRAP_AFTER  0x0008 ///< flag: line break after this char is allowed with addition of hyphen
                                            //         It is set by Hyphman when finding hyphenation points in a word.
#define LCHAR_MANDATORY_NEWLINE      0x0010 ///< flag: this char must start with new line
#define LCHAR_IS_CLUSTER_TAIL        0x0020 ///< flag: this char is a tail of a cluster (eg. ligature,
                                            //         whose glyph is carried by first char)
                                            //         It is set by harfbuzz when used.

// (This one is actually not set by lvfntman)
#define LCHAR_LOCKED_SPACING         0x0040 ///< flag: forbid any letter spacing tweak on this char
                                            //         (for cursive scripts like arabic, and special cases)
#define LCHAR__AVAILABLE_BIT_08__    0x0080

/// The next ones, not fitting in a lUInt8, should only be set and used by lvtextfm
#define LCHAR_IS_OBJECT              0x0100 ///< flag: this char is object (image, float)
#define LCHAR_IS_COLLAPSED_SPACE     0x0200 ///< flag: this char is a space that should not be rendered
#define LCHAR_IS_TO_IGNORE           0x0400 ///< flag: this char is to be ignored/skipped in text measurement and drawing
#define LCHAR_IS_RTL                 0x0800 ///< flag: this char is part of a RTL segment
#define LCHAR_IS_CJK                 0x1000 ///< flag: this char is CJK
#define LCHAR_IS_FLEXIBLE_WIDTH_CJK  0x2000 ///< flag: this char is a CJK fullwidth char that can have its
                                            ///        nominal width modified (mostly small punctuation)
#define LCHAR__AVAILABLE_BIT_15__    0x4000
#define LCHAR__AVAILABLE_BIT_16__    0x8000

#endif
