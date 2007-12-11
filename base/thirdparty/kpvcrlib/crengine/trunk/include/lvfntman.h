/** \file lvfntman.h
    \brief font manager interface

    CoolReader Engine

    (c) Vadim Lopatin, 2000-2006

    This source code is distributed under the terms of
    GNU General Public License.

    See LICENSE file for details.

*/

#ifndef __LV_FNT_MAN_H_INCLUDED__
#define __LV_FNT_MAN_H_INCLUDED__

#include <stdlib.h>
#include "crsetup.h"
#include "lvfnt.h"
#include "cssdef.h"
#include "lvstring.h"
#include "lvref.h"
#include "lvptrvec.h"
#include "hyphman.h"
#include "lvdrawbuf.h"

#if !defined(__SYMBIAN32__) && defined(_WIN32)
#include <windows.h>
#endif

class LVDrawBuf;

/** \brief base class for fonts

    implements single interface for font of any engine
*/
class LVFont
{
public:
    /// glyph properties structure
    struct glyph_info_t {
        lUInt8  blackBoxX;   ///< 0: width of glyph
        lUInt8  blackBoxY;   ///< 1: height of glyph black box
        lInt8   originX;     ///< 2: X origin for glyph
        lInt8   originY;     ///< 3: Y origin for glyph
        lUInt8  width;       ///< 4: full width of glyph
    };

    /** \brief get glyph info
        \param glyph is pointer to glyph_info_t struct to place retrieved info
        \return true if glyh was found 
    */
    virtual bool getGlyphInfo( lUInt16 code, glyph_info_t * glyph ) = 0;

    /** \brief measure text
        \param text is text string pointer
        \param len is number of characters to measure
        \return number of characters before max_width reached 
    */
    virtual lUInt16 measureText( 
                        const lChar16 * text, int len, 
                        lUInt16 * widths,
                        lUInt8 * flags,
                        int max_width,
                        lChar16 def_char
                     ) = 0;
    /** \brief measure text
        \param text is text string pointer
        \param len is number of characters to measure
        \return width of specified string 
    */
    virtual lUInt32 getTextWidth(
                        const lChar16 * text, int len
        ) = 0;

    /** \brief get glyph image in 1 byte per pixel format
        \param code is unicode character
        \param buf is buffer [width*height] to place glyph data
        \return true if glyph was found 
    */
    virtual bool getGlyphImage(lUInt16 code, lUInt8 * buf) = 0;
    /// returns font baseline offset
    virtual int getBaseline() = 0;
    /// returns font height
    virtual int getHeight() = 0;
    /// returns char width
    virtual int getCharWidth( lChar16 ch ) = 0;
    /// retrieves font handle
    virtual void * GetHandle() = 0;
    /// returns font typeface name
    virtual lString8 getTypeFace() = 0;
    /// returns font family id
    virtual css_font_family_t getFontFamily() = 0;
    /// draws text string
    virtual void DrawTextString( LVDrawBuf * buf, int x, int y, 
                       const lChar16 * text, int len, 
                       lChar16 def_char, lUInt32 * palette, bool addHyphen, lUInt32 flags=0 ) = 0;
    /// constructor
    LVFont() { }
    /// returns true if font is empty
    virtual bool IsNull() const = 0;
    virtual bool operator ! () const = 0;
    virtual void Clear() = 0;
    virtual ~LVFont() { }
};

typedef LVRef<LVFont> LVFontRef;


/// font manager interface class
class LVFontManager
{
public:
    /// garbage collector frees unused fonts
    virtual void gc() = 0;
    /// returns most similar font
    virtual LVFontRef GetFont(int size, int weight, bool italic, css_font_family_t family, lString8 typeface ) = 0;
    /// registers font by name
    virtual bool RegisterFont( lString8 name ) = 0;
    /// initializes font manager
    virtual bool Init( lString8 path ) = 0;
    virtual int GetFontCount() = 0;
    LVFontManager() { }
    virtual ~LVFontManager() { }
};

class LVBaseFont : public LVFont
{
protected:
    lString8 _typeface;
    css_font_family_t _family;
public:
    /// returns font typeface name
    virtual lString8 getTypeFace() { return _typeface; }
    /// returns font family id
    virtual css_font_family_t getFontFamily() { return _family; }
    /// draws text string
    virtual void DrawTextString( LVDrawBuf * buf, int x, int y, 
                       const lChar16 * text, int len, 
                       lChar16 def_char, lUInt32 * palette, bool addHyphen, lUInt32 flags=0 );
};

/* C++ wrapper class */
class LBitmapFont : public LVBaseFont
{
private:
    lvfont_handle m_font;
public:
    LBitmapFont() : m_font(NULL) { }
    virtual bool getGlyphInfo( lUInt16 code, LVFont::glyph_info_t * glyph );
    virtual lUInt16 measureText( 
                        const lChar16 * text, int len, 
                        lUInt16 * widths,
                        lUInt8 * flags,
                        int max_width,
                        lChar16 def_char
                     );
    /** \brief measure text
        \param text is text string pointer
        \param len is number of characters to measure
        \return width of specified string 
    */
    virtual lUInt32 getTextWidth(
                        const lChar16 * text, int len
        );
    /// returns font baseline offset
    virtual int getBaseline();
    /// returns font height
    virtual int getHeight();
    
    virtual bool getGlyphImage(lUInt16 code, lUInt8 * buf);
    
    /// returns char width
    virtual int getCharWidth( lChar16 ch )
    {
        glyph_info_t glyph;
        if ( getGlyphInfo(ch, &glyph) )
            return glyph.width;
        return 0;
    }

    virtual lvfont_handle GetHandle() { return m_font; }
    
    int LoadFromFile( const char * fname );
    
    // LVFont functions overrides
    virtual void Clear() { if (m_font) lvfontClose( m_font ); m_font = NULL; }
    virtual bool IsNull() const { return m_font==NULL; }
    virtual bool operator ! () const { return IsNull(); }
    virtual ~LBitmapFont() { Clear(); }
};

#if !defined(__SYMBIAN32__) && defined(_WIN32)
class LVBaseWin32Font : public LVBaseFont
{
protected:
    HFONT   _hfont;
    LOGFONT _logfont;
    int     _height;
    int     _baseline;
    LVColorDrawBuf _drawbuf;
    
public:    

    LVBaseWin32Font() : _hfont(NULL), _height(0), _baseline(0), _drawbuf(1,1) 
        { }
        
    virtual ~LVBaseWin32Font() { Clear(); }

    /// returns font baseline offset
    virtual int getBaseline()
    {
        return _baseline;
    }
    
    /// returns font height
    virtual int getHeight()
    {
        return _height;
    }
    
    /// retrieves font handle
    virtual void * GetHandle()
    {
        return (void*)_hfont;
    }
    
    /// returns char width
    virtual int getCharWidth( lChar16 ch )
    {
        glyph_info_t glyph;
        if ( getGlyphInfo(ch, &glyph) )
            return glyph.width;
        return 0;
    }
    /// returns true if font is empty
    virtual bool IsNull() const 
    {
        return (_hfont == NULL);
    }
    
    virtual bool operator ! () const
    {
        return (_hfont == NULL);
    }
    
    virtual void Clear();

    virtual bool Create( const LOGFONT & lf );

    virtual bool Create(int size, int weight, bool italic, css_font_family_t family, lString8 typeface );
    
};


class LVWin32DrawFont : public LVBaseWin32Font
{
private:
    int _hyphen_width;
public:

    LVWin32DrawFont() : _hyphen_width(0) { }

    /** \brief get glyph info
        \param glyph is pointer to glyph_info_t struct to place retrieved info
        \return true if glyh was found 
    */
    virtual bool getGlyphInfo( lUInt16 code, glyph_info_t * glyph );

    /** \brief measure text
        \param glyph is pointer to glyph_info_t struct to place retrieved info
        \return true if glyph was found 
    */
    virtual lUInt16 measureText( 
                        const lChar16 * text, int len, 
                        lUInt16 * widths,
                        lUInt8 * flags,
                        int max_width,
                        lChar16 def_char
                     );
    /** \brief measure text
        \param text is text string pointer
        \param len is number of characters to measure
        \return width of specified string 
    */
    virtual lUInt32 getTextWidth(
                        const lChar16 * text, int len
        );

    /// returns char width
    virtual int getCharWidth( lChar16 ch );

    /// draws text string
    virtual void DrawTextString( LVDrawBuf * buf, int x, int y, 
                       const lChar16 * text, int len, 
                       lChar16 def_char, lUInt32 * palette, bool addHyphen, lUInt32 flags=0 );
        
    /** \brief get glyph image in 1 byte per pixel format
        \param code is unicode character
        \param buf is buffer [width*height] to place glyph data
        \return true if glyph was found 
    */
    virtual bool getGlyphImage(lUInt16 code, lUInt8 * buf);
    
};

struct glyph_t {
    lUInt8 *     glyph;
    lChar16      ch;
    bool         flgNotExists;
    bool         flgValid;
    LVFont::glyph_info_t gi;
    glyph_t *    next;
    glyph_t(lChar16 c)
    : glyph(NULL), ch(c), flgNotExists(false), flgValid(false), next(NULL)
    {
        memset( &gi, 0, sizeof(gi) );
    }
    ~glyph_t()
    {
        if (glyph)
            delete glyph;
    }
};

class GlyphCache
{
private:
    lUInt32 _size;
    glyph_t * * _hashtable;        
public:
    GlyphCache( lUInt32 size )
    : _size(size)
    {
        _hashtable = new glyph_t * [_size];
        for (lUInt32 i=0; i<_size; i++)
            _hashtable[i] = NULL;
    }
    void clear()
    {
        for (lUInt32 i=0; i<_size; i++)
        {
            glyph_t * p = _hashtable[i];
            while (p)
            {
                glyph_t * next = p->next;
                delete p;
                p = next;
            }
            _hashtable[i] = NULL;
        }
    }
    ~GlyphCache()
    {
        if (_hashtable)
        {
            clear();
            delete _hashtable;
        }
    }
    glyph_t * find( lChar16 ch )
    {
        lUInt32 index = (((lUInt32)ch)*113) % _size;
        glyph_t * p = _hashtable[index];
        // 3 levels
        if (!p)
            return NULL;
        if (p->ch == ch)
            return p;
        p = p->next;
        if (!p)
            return NULL;
        if (p->ch == ch)
            return p;
        p = p->next;
        if (!p)
            return NULL;
        if (p->ch == ch)
            return p;
        return NULL;
    }
    /// returns found or creates new
    glyph_t * get( lChar16 ch )
    {
        lUInt32 index = (((lUInt32)ch)*113) % _size;
        glyph_t * * p = &_hashtable[index];
        // 3 levels
        if (!*p)
        {
            return (*p = new glyph_t(ch));
        }
        if ((*p)->ch == ch)
        {
            return *p;
        }
        p = &(*p)->next;
        if (!*p)
        {
            return (*p = new glyph_t(ch));
        }
        if ((*p)->ch == ch)
        {
            return *p;
        }
        p = &(*p)->next;
        if (!*p)
        {
            return (*p = new glyph_t(ch));
        }
        if ((*p)->ch == ch)
        {
            return *p;
        }

        delete (*p);
        *p = NULL;

        glyph_t * pp = new glyph_t(ch);
        pp->next = _hashtable[index];
        _hashtable[index] = pp;
        return pp;
    }
    
};

class LVWin32Font : public LVBaseWin32Font
{
private:    
    lChar16 _unknown_glyph_index;
    GlyphCache _cache;
    
    static int GetGlyphIndex( HDC hdc, wchar_t code );
    
    glyph_t * GetGlyphRec( lChar16 ch );

public:
    /** \brief get glyph info
        \param glyph is pointer to glyph_info_t struct to place retrieved info
        \return true if glyh was found 
    */
    virtual bool getGlyphInfo( lUInt16 code, glyph_info_t * glyph );

    /** \brief measure text
        \param glyph is pointer to glyph_info_t struct to place retrieved info
        \return true if glyph was found 
    */
    virtual lUInt16 measureText( 
                        const lChar16 * text, int len, 
                        lUInt16 * widths,
                        lUInt8 * flags,
                        int max_width,
                        lChar16 def_char
                     );
    /** \brief measure text
        \param text is text string pointer
        \param len is number of characters to measure
        \return width of specified string 
    */
    virtual lUInt32 getTextWidth(
                        const lChar16 * text, int len
        );

    /** \brief get glyph image in 1 byte per pixel format
        \param code is unicode character
        \param buf is buffer [width*height] to place glyph data
        \return true if glyph was found 
    */
    virtual bool getGlyphImage(lUInt16 code, lUInt8 * buf);
    
    virtual void Clear();

    virtual bool Create( const LOGFONT & lf );

    virtual bool Create(int size, int weight, bool italic, css_font_family_t family, lString8 typeface );

    LVWin32Font() : _cache(256) {  }
    
    virtual ~LVWin32Font() { }
};

#endif


/// current font manager pointer
extern LVFontManager * fontMan;

/// initializes font manager
bool InitFontManager( lString8 path );

/// deletes font manager
bool ShutdownFontManager();

LVFontRef LoadFontFromFile( const char * fname );

#endif //__LV_FNT_MAN_H_INCLUDED__
