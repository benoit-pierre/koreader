/** \file lvtypes.h
    \brief CREngine common types definition

    (c) Vadim Lopatin, 2000-2006
    This source code is distributed under the terms of
    GNU General Public License.
    See LICENSE file for details.
*/

#ifndef LVTYPES_H_INCLUDED
#define LVTYPES_H_INCLUDED

#include <stdlib.h>
#include "crsetup.h"

typedef long int lInt32;            ///< signed 32 bit int
typedef unsigned long int lUInt32;  ///< unsigned 32 bit int

typedef short int lInt16;           ///< signed 16 bit int
typedef unsigned short int lUInt16; ///< unsigned 16 bit int

typedef signed char lInt8;          ///< signed 8 bit int
typedef unsigned char lUInt8;       ///< unsigned 8 bit int

typedef wchar_t lChar16;            ///< 16 bit char
typedef char lChar8;                ///< 8 bit char

#if defined(_WIN32) && !defined(CYGWIN)
typedef __int64 lInt64;             ///< signed 64 bit int
typedef unsigned __int64 lUInt64;   ///< unsigned 64 bit int
#else
typedef long long int lInt64;       ///< signed 64 bit int
typedef unsigned long long int lUInt64; ///< unsigned 64 bit int
#endif

/// platform-dependent path separator
#ifdef _WIN32
#define PATH_SEPARATOR_CHAR '\\'
#elif __SYMBIAN32__
#define PATH_SEPARATOR_CHAR '\\'
#else
#define PATH_SEPARATOR_CHAR '/'
#endif

/// point
class lvPoint {
public:
    int x;
    int y;
    lvPoint() : x(0), y(0) { }
    lvPoint(int nx, int ny) : x(nx), y(ny) { }
};

/// rectangle
class lvRect {
public:
    int left;
    int top;
    int right;
    int bottom;
    lvRect() : left(0), top(0), right(0), bottom(0) { }
    lvRect( int x0, int y0, int x1, int y1) : left(x0), top(y0), right(x1), bottom(y1) { }
    lvPoint topLeft() const { return lvPoint( left, top ); }
    lvPoint bottomRight() const { return lvPoint( right, bottom ); }
    int width() const { return right - left; }
    int height() const { return bottom - top; }
    void shrink( int delta ) { left+=delta; right-=delta; top+=delta; bottom-=delta; }
    void extend( int delta ) { shrink(-delta); }
    bool isPointInside( lvPoint & pt )
    {
        return left<=pt.x && top<=pt.y && right>pt.x && bottom > pt.y;
    }
};

class lvColor
{
    lUInt32 value;
public:
    lvColor( lUInt32 cl ) : value(cl) { }
    lvColor(  lUInt32 r, lUInt32 g, lUInt32 b ) : value(((r&255)<<16) | ((g&255)<<8) | (b&255)) { }
    lvColor( lUInt32 r, lUInt32 g, lUInt32 b, lUInt32 a ) : value(((a&255)<<24) | ((r&255)<<16) | ((g&255)<<8) | (b&255)) { }
    operator lUInt32 () const { return value; }
    lUInt32 get() const { return value; }
    lUInt8 r() const { return (value>>16)&255; }
    lUInt8 g() const { return (value>>8)&255; }
    lUInt8 b() const { return (value)&255; }
    lUInt8 a() const { return (value>>24)&255; }
};

/// byte order convertor
class lvByteOrderConv {
    bool _lsf;
public:
    lvByteOrderConv()
    {
        union {
            lUInt16 word;
            lUInt8 bytes[2];
        } test;
        test.word = 1;
        _lsf = test.bytes[0]!=0;
    }
    /// reverse 32 bit word
    inline static lUInt32 rev( lUInt32 w )
    {
        return 
            ((w&0xFF000000)>>24)|
            ((w&0x00FF0000)>>8) |
            ((w&0x0000FF00)<<8) |
            ((w&0x000000FF)<<24);
    }
    /// reverse 16bit word
    inline static lUInt16 rev( lUInt16 w )
    {
        return 
            (lUInt16)(
            ((w&0xFF00)>>8)|
            ((w&0x00FF)<<8) );
    }
    /// make 32 bit word least-significant-first byte order (Intel)
    lUInt32 lsf( lUInt32 w )
    {
        return ( _lsf ) ? w : rev(w);
    }
    /// make 32 bit word most-significant-first byte order (PPC)
    lUInt32 msf( lUInt32 w )
    {
        return ( !_lsf ) ? w : rev(w);
    }
    /// make 16 bit word least-significant-first byte order (Intel)
    lUInt16 lsf( lUInt16 w )
    {
        return ( _lsf ) ? w : rev(w);
    }
    /// make 16 bit word most-significant-first byte order (PPC)
    lUInt16 msf( lUInt16 w )
    {
        return ( !_lsf ) ? w : rev(w);
    }
    void rev( lUInt32 * w )
    {
        *w = rev(*w);
    }
    void rev( lUInt16 * w )
    {
        *w = rev(*w);
    }
    void msf( lUInt32 * w )
    {
        if ( _lsf )
            *w = rev(*w);
    }
    void lsf( lUInt32 * w )
    {
        if ( !_lsf )
            *w = rev(*w);
    }
    void msf( lUInt32 * w, int len )
    {
        if ( _lsf ) {
            for ( int i=0; i<len; i++)
                w[i] = rev(w[i]);
        }
    }
    void lsf( lUInt32 * w, int len )
    {
        if ( !_lsf ) {
            for ( int i=0; i<len; i++)
                w[i] = rev(w[i]);
        }
    }
    void msf( lUInt16 * w, int len )
    {
        if ( _lsf ) {
            for ( int i=0; i<len; i++)
                w[i] = rev(w[i]);
        }
    }
    void lsf( lUInt16 * w, int len )
    {
        if ( !_lsf ) {
            for ( int i=0; i<len; i++)
                w[i] = rev(w[i]);
        }
    }
    void msf( lUInt16 * w )
    {
        if ( _lsf )
            *w = rev(*w);
    }
    void lsf( lUInt16 * w )
    {
        if ( !_lsf )
            *w = rev(*w);
    }
    bool lsf()
    {
        return (_lsf);
    }
    bool msf()
    {
        return (!_lsf);
    }
};

#endif//LVTYPES_H_INCLUDED
