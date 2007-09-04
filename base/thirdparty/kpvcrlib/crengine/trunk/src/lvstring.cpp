/*******************************************************

   CoolReader Engine

   lvstring.cpp:  string classes implementation

   (c) Vadim Lopatin, 2000-2006
   This source code is distributed under the terms of
   GNU General Public License
   See LICENSE file for details

*******************************************************/

#include "../include/lvstring.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#ifdef LINUX
#include <sys/time.h>
#endif


#if !defined(__SYMBIAN32__) && defined(_WIN32)
#include <windows.h>
#endif

#define LS_DEBUG_CHECK

// memory allocation slice
struct lstring_chunk_slice_t {
    lstring_chunk_t * pChunks; // first chunk
    lstring_chunk_t * pEnd;    // first free byte after last chunk
    lstring_chunk_t * pFree;   // first free chunk
    int used;
    lstring_chunk_slice_t( int size )
    {
        pChunks = (lstring_chunk_t *) malloc(sizeof(lstring_chunk_t) * size);
        pEnd = pChunks + size;
        pFree = pChunks;
        for (lstring_chunk_t * p = pChunks; p<pEnd; ++p)
        {
            p->nextfree = p+1;
            p->size = 0;
        }
        (pEnd-1)->nextfree = NULL;
    }
    ~lstring_chunk_slice_t()
    {
        free( pChunks );
    }
    inline lstring_chunk_t * alloc_chunk()
    {
        lstring_chunk_t * res = pFree;
        pFree = res->nextfree;
        return res;
    }
    inline bool free_chunk( lstring_chunk_t * pChunk )
    {
        if (pChunk < pChunks || pChunk >= pEnd)
            return false; // chunk does not belong to this slice
/*
#ifdef LS_DEBUG_CHECK
        if (!pChunk->size)
        {
            crFatalError(); // already freed!!!
        }
        pChunk->size = 0;
#endif
*/
        pChunk->nextfree = pFree;
        pFree = pChunk;
        return true;
    }
};

//#define FIRST_SLICE_SIZE 256
//#define MAX_SLICE_COUNT  20
#if (LDOM_USE_OWN_MEM_MAN == 1)
static lstring_chunk_slice_t * slices[MAX_SLICE_COUNT];
static int slices_count = 0;
static bool slices_initialized = false;
#endif

static lChar8 empty_str_8[] = {0};
static lstring_chunk_t empty_chunk_8(empty_str_8);
lstring_chunk_t * lString8::EMPTY_STR_8 = &empty_chunk_8;

static lChar16 empty_str_16[] = {0};
static lstring_chunk_t empty_chunk_16(empty_str_16);
lstring_chunk_t * lString16::EMPTY_STR_16 = &empty_chunk_16;

#if (LDOM_USE_OWN_MEM_MAN == 1)
static void init_ls_storage()
{
    slices[0] = new lstring_chunk_slice_t( FIRST_SLICE_SIZE );
    slices_count = 1;
    slices_initialized = true;
}

void free_ls_storage()
{
    if (!slices_initialized)
        return;
    for (int i=0; i<slices_count; i++)
    {
        delete slices[i];
    }
    slices_count = 0;
    slices_initialized = false;
}

lstring_chunk_t * lstring_chunk_t::alloc()
{
    if (!slices_initialized)
        init_ls_storage();
    // search for existing slice
    for (int i=slices_count-1; i>=0; --i)
    {
        if (slices[i]->pFree != NULL)
            return slices[i]->alloc_chunk();
    }
    // alloc new slice
    if (slices_count >= MAX_SLICE_COUNT)
        crFatalError();
    slices[slices_count++] = new lstring_chunk_slice_t( FIRST_SLICE_SIZE << (slices_count+1) );
    return slices[slices_count-1]->alloc_chunk();
}

void lstring_chunk_t::free( lstring_chunk_t * pChunk )
{
    for (int i=slices_count-1; i>=0; --i)
    {
        if (slices[i]->free_chunk(pChunk))
            return;
    }
    crFatalError(); // wrong pointer!!!
}
#endif

////////////////////////////////////////////////////////////////////////////
// Utility functions
////////////////////////////////////////////////////////////////////////////

inline size_t _lStr_len(const lChar16 * str)
{
    size_t len;
    for (len=0; *str; str++)
        len++;
    return len;
}

inline size_t _lStr_len(const lChar8 * str)
{
    size_t len;
    for (len=0; *str; str++)
        len++;
    return len;
}

inline size_t _lStr_nlen(const lChar16 * str, size_t maxcount)
{
    size_t len;
    for (len=0; *str && len<maxcount; str++)
        len++;
    return len;
}

inline size_t _lStr_nlen(const lChar8 * str, size_t maxcount)
{
    size_t len;
    for (len=0; *str && len<maxcount; str++)
        len++;
    return len;
}

inline size_t _lStr_cpy(lChar16 * dst, const lChar16 * src)
{
    size_t count;
    for ( count=0; (*dst++ = *src++); count++ )
        ;
    return count;
}

inline size_t _lStr_cpy(lChar8 * dst, const lChar8 * src)
{
    size_t count;
    for ( count=0; (*dst++ = *src++); count++ )
        ;
    return count;
}

inline size_t _lStr_cpy(lChar16 * dst, const lChar8 * src)
{
    size_t count;
    for ( count=0; (*dst++ = *src++); count++ )
        ;
    return count;
}

inline size_t _lStr_cpy(lChar8 * dst, const lChar16 * src)
{
    size_t count;
    for ( count=0; (*dst++ = (lChar8)*src++); count++ )
        ;
    return count;
}

inline size_t _lStr_ncpy(lChar16 * dst, const lChar16 * src, size_t maxcount)
{
    size_t count = 0;
    do
    {
        if (++count > maxcount)
        {
            *dst = 0;
            return count;
        }
    } while ((*dst++ = *src++));
    return count;
}

inline size_t _lStr_ncpy(lChar8 * dst, const lChar8 * src, size_t maxcount)
{
    size_t count = 0;
    do
    {
        if (++count > maxcount)
        {
            *dst = 0;
            return count;
        }
    } while ((*dst++ = *src++));
    return count;
}

inline void _lStr_memcpy(lChar16 * dst, const lChar16 * src, size_t count)
{
    while ( count-- > 0)
        (*dst++ = *src++);
}

inline void _lStr_memcpy(lChar8 * dst, const lChar8 * src, size_t count)
{
    while ( count-- > 0)
        (*dst++ = *src++);
}

inline void _lStr_memset(lChar16 * dst, lChar16 value, size_t count)
{
    while ( count-- > 0)
        *dst++ = value;
}

inline void _lStr_memset(lChar8 * dst, lChar8 value, size_t count)
{
    while ( count-- > 0)
        *dst++ = value;
}

size_t lStr_len(const lChar16 * str)
{
    return _lStr_len(str);
}

size_t lStr_len(const lChar8 * str)
{
    return _lStr_len(str);
}

size_t lStr_nlen(const lChar16 * str, size_t maxcount)
{
    return _lStr_nlen(str, maxcount);
}

size_t lStr_nlen(const lChar8 * str, size_t maxcount)
{
    return _lStr_nlen(str, maxcount);
}

size_t lStr_cpy(lChar16 * dst, const lChar16 * src)
{
    return _lStr_cpy(dst, src);
}

size_t lStr_cpy(lChar8 * dst, const lChar8 * src)
{
    return _lStr_cpy(dst, src);
}

size_t lStr_cpy(lChar16 * dst, const lChar8 * src)
{
    return _lStr_cpy(dst, src);
}

size_t lStr_ncpy(lChar16 * dst, const lChar16 * src, size_t maxcount)
{
    return _lStr_ncpy(dst, src, maxcount);
}

size_t lStr_ncpy(lChar8 * dst, const lChar8 * src, size_t maxcount)
{
    return _lStr_ncpy(dst, src, maxcount);
}

void lStr_memcpy(lChar16 * dst, const lChar16 * src, size_t count)
{
    _lStr_memcpy(dst, src, count);
}

void lStr_memcpy(lChar8 * dst, const lChar8 * src, size_t count)
{
    _lStr_memcpy(dst, src, count);
}

void lStr_memset(lChar16 * dst, lChar16 value, size_t count)
{
    _lStr_memset(dst, value, count);
}

void lStr_memset(lChar8 * dst, lChar8 value, size_t count)
{
    _lStr_memset(dst, value, count);
}

int lStr_cmp(const lChar16 * dst, const lChar16 * src)
{
    while ( *dst == *src)
    {
        if (! *dst )
            return 0;
        ++dst;
        ++src;
    }
    if ( *dst > *src )
        return 1;
    else
        return -1;
}

int lStr_cmp(const lChar8 * dst, const lChar8 * src)
{
    while ( *dst == *src)
    {
        if (! *dst )
            return 0;
        ++dst;
        ++src;
    }
    if ( *dst > *src )
        return 1;
    else
        return -1;
}

int lStr_cmp(const lChar16 * dst, const lChar8 * src)
{
    while ( *dst == (lChar16)*src)
    {
        if (! *dst )
            return 0;
        ++dst;
        ++src;
    }
    if ( *dst > (lChar16)*src )
        return 1;
    else
        return -1;
}

int lStr_cmp(const lChar8 * dst, const lChar16 * src)
{
    while ( (lChar16)*dst == *src)
    {
        if (! *dst )
            return 0;
        ++dst;
        ++src;
    }
    if ( (lChar16)*dst > *src )
        return 1;
    else
        return -1;
}

////////////////////////////////////////////////////////////////////////////
// lString16
////////////////////////////////////////////////////////////////////////////

void lString16::free()
{
    //assert(pchunk->buf16[pchunk->len]==0);
    ::free(pchunk->buf16);
#if (LDOM_USE_OWN_MEM_MAN == 1)
    for (int i=slices_count-1; i>=0; --i)
    {
        if (slices[i]->free_chunk(pchunk))
            return;
    }
    crFatalError(); // wrong pointer!!!
#else
    ::free(pchunk);
#endif
}

void lString16::alloc(size_t sz)
{
#if (LDOM_USE_OWN_MEM_MAN == 1)
    pchunk = lstring_chunk_t::alloc();
#else
    pchunk = (lstring_chunk_t*)::malloc(sizeof(lstring_chunk_t));
#endif
    pchunk->buf16 = (lChar16*) ::malloc( sizeof(lChar16) * (sz+1) );
    assert( pchunk->buf16!=NULL );
    pchunk->size = sz;
    pchunk->nref = 1;
}

lString16::lString16(const lChar16 * str)
{
    if (!str || !(*str))
    {
        pchunk = EMPTY_STR_16; 
        addref();
        return;
    }
    size_type len = _lStr_len(str);
    alloc( len );
    pchunk->len = len;
    _lStr_cpy( pchunk->buf16, str );
}

lString16::lString16(const lChar8 * str)
{
    if (!str || !(*str))
    {
        pchunk = EMPTY_STR_16; 
        addref();
        return;
    }
    size_type len = _lStr_len(str);
    alloc( len );
    pchunk->len = len;
    _lStr_cpy( pchunk->buf16, str );
}

lString16::lString16(const value_type * str, size_type count)
{
    if ( !str || !(*str) || count<=0 )
    {
        pchunk = EMPTY_STR_16; addref();
    }
    else
    {
        size_type len = _lStr_nlen(str, count);
        alloc(len);
        _lStr_ncpy( pchunk->buf16, str, len );
        pchunk->len = len;
    }
}

lString16::lString16(const lString16 & str, size_type offset, size_type count)
{
    if ( count > str.length() - offset )
        count = str.length() - offset;
    if (count<=0 || offset<0)
    {
        pchunk = EMPTY_STR_16; addref();
    }
    else
    {
        alloc(count);
        _lStr_memcpy( pchunk->buf16, str.pchunk->buf16+offset, count );
        pchunk->buf16[count]=0;
        pchunk->len = count;
    }
}

lString16 & lString16::assign(const lChar16 * str)
{
    if (!str || !(*str))
    {
        clear();
    }
    else
    {
        size_type len = _lStr_len(str);
        if (pchunk->nref==1)
        { 
            if (pchunk->size<=len)
            {
                // resize is necessary
                pchunk->buf16 = (lChar16*) ::realloc( pchunk->buf16, sizeof(lChar16)*(len+1) );
                pchunk->size = len+1;
            }
        }
        else
        {
            release();
            alloc(len);
        }
        _lStr_cpy( pchunk->buf16, str );
        pchunk->len = len;
    }
    return *this;
}

lString16 & lString16::assign(const lChar16 * str, size_type count)
{
    if ( !str || !(*str) || count<=0 )
    {
        clear();
    }
    else
    {
        size_type len = _lStr_nlen(str, count);
        if (pchunk->nref==1)
        { 
            if (pchunk->size<=len)
            {
                // resize is necessary
                pchunk->buf16 = (lChar16*) ::realloc( pchunk->buf16, sizeof(lChar16)*(len+1) );
                pchunk->size = len+1;
            }
        }
        else
        {
            release();
            alloc(len);
        }
        _lStr_ncpy( pchunk->buf16, str, count );
        pchunk->len = len;
    }
    return *this;
}

lString16 & lString16::assign(const lString16 & str, size_type offset, size_type count)
{
    if ( count > str.length() - offset )
        count = str.length() - offset;
    if (count<=0 || offset<0)
    {
        clear();
    }
    else
    {
        if (pchunk==str.pchunk)
        {
            if (&str != this)
            {
                release();
                alloc(count);
            }
            if (offset>0)
            {
                _lStr_memcpy( pchunk->buf16, str.pchunk->buf16+offset, count );
            }
            pchunk->buf16[count]=0;
        }
        else
        {
            if (pchunk->nref==1)
            { 
                if (pchunk->size<=count)
                {
                    // resize is necessary
                    pchunk->buf16 = (lChar16*) ::realloc( pchunk->buf16, sizeof(lChar16)*(count+1) );
                    pchunk->size = count+1;
                }
            }
            else
            {
                release();
                alloc(count);
            }
            _lStr_memcpy( pchunk->buf16, str.pchunk->buf16+offset, count );
            pchunk->buf16[count]=0;
        }
        pchunk->len = count;
    }
    return *this;
}

lString16 & lString16::erase(size_type offset, size_type count)
{
    if ( count > length() - offset )
        count = length() - offset;
    if (count<=0 || offset<0)
    {
        clear();
    }
    else
    {
        size_type newlen = length()-count;
        if (pchunk->nref==1)
        {
            _lStr_memcpy( pchunk->buf16+offset, pchunk->buf16+offset+count, newlen-offset+1 );
        }
        else
        {
            lstring_chunk_t * poldchunk = pchunk;
            release();
            alloc( newlen );
            _lStr_memcpy( pchunk->buf16, poldchunk->buf16, offset );
            _lStr_memcpy( pchunk->buf16+offset, poldchunk->buf16+offset+count, newlen-offset+1 );
        }
        pchunk->len = newlen;
        pchunk->buf16[newlen]=0;
    }
    return *this;
}

void lString16::reserve(size_type n)
{
    if (pchunk->nref==1)
    {
        if (pchunk->size<=n)
        {
            pchunk->buf16 = (lChar16*) ::realloc( pchunk->buf16, sizeof(lChar16)*(n+1) );
            pchunk->size = n;
        }
    }
    else
    {
        lstring_chunk_t * poldchunk = pchunk;
        release();
        alloc( n );
        _lStr_memcpy( pchunk->buf16, poldchunk->buf16, poldchunk->len+1 );
        pchunk->len = poldchunk->len;
    }
}

void lString16::lock( size_type newsize )
{
    if (pchunk->nref>1)
    {
        lstring_chunk_t * poldchunk = pchunk;
        release();
        alloc( newsize );
        size_type len = newsize;
        if (len>poldchunk->len)
            len = poldchunk->len;
        _lStr_memcpy( pchunk->buf16, poldchunk->buf16, len );
        pchunk->buf16[len]=0;
        pchunk->len = len;
    }
}

// lock string, allocate buffer and reset length to 0
void lString16::reset( size_type size )
{
    if (pchunk->nref>1 || pchunk->size<size)
    {
        release();
        alloc( size );
    }
    pchunk->buf16[0] = 0;
    pchunk->len = 0;
}

void lString16::resize(size_type n, lChar16 e)
{
    lock( n );
    if (n>=pchunk->size)
    {
        pchunk->buf16 = (lChar16*) ::realloc( pchunk->buf16, sizeof(lChar16)*(n+1) );
        pchunk->size = n;
    }
    // fill with data if expanded
    for (size_type i=pchunk->len; i<n; i++)
        pchunk->buf16[i] = e;
    pchunk->buf16[pchunk->len] = 0;
}

lString16 & lString16::append(const lChar16 * str)
{
    size_type len = _lStr_len(str);
    reserve( pchunk->len+len );
    _lStr_memcpy(pchunk->buf16+pchunk->len, str, len+1);
    pchunk->len += len;
    return *this;
}

lString16 & lString16::append(const lChar16 * str, size_type count)
{
    size_type len = _lStr_nlen(str, count);
    reserve( pchunk->len+len );
    _lStr_ncpy(pchunk->buf16+pchunk->len, str, len);
    pchunk->len += len;
    return *this;
}

lString16 & lString16::append(const lString16 & str)
{
    size_type len2 = pchunk->len + str.pchunk->len;
    reserve( len2 );
    _lStr_memcpy( pchunk->buf16+pchunk->len, str.pchunk->buf16, str.pchunk->len+1 );
    pchunk->len = len2;
    return *this;
}

lString16 & lString16::append(const lString16 & str, size_type offset, size_type count)
{
    if ( str.pchunk->len>offset )
    {
        if ( offset + count > str.pchunk->len )
            count = str.pchunk->len - offset;
        reserve( pchunk->len+count );
        _lStr_ncpy(pchunk->buf16 + pchunk->len, str.pchunk->buf16 + offset, count);
        pchunk->len += count;
        pchunk->buf16[pchunk->len] = 0;
    }
    return *this;
}

lString16 & lString16::append(size_type count, lChar16 ch)
{
    reserve( pchunk->len+count );
    _lStr_memset(pchunk->buf16+pchunk->len, ch, count);
    pchunk->len += count;
    pchunk->buf16[pchunk->len] = 0;
    return *this;
}

lString16 & lString16::insert(size_type p0, size_type count, lChar16 ch)
{
    if (p0>pchunk->len)
        p0 = pchunk->len;
    reserve( pchunk->len+count );
    for (size_type i=pchunk->len+count; i>p0; i--)
        pchunk->buf16[i] = pchunk->buf16[i-1];
    _lStr_memset(pchunk->buf16+p0, ch, count);
    pchunk->len += count;
    pchunk->buf16[pchunk->len] = 0;
    return *this;
}

lString16 lString16::substr(size_type pos, size_type n) const
{
    if (pos>=length())
        return lString16();
    if (pos+n>length())
        n = length() - pos;
    return lString16( pchunk->buf16+pos, n );
}

lString16 & lString16::pack()
{
    if (pchunk->len + 4 < pchunk->size )
    {
        if (pchunk->nref>1)
        {
            lock(pchunk->len);
        }
        else
        {
            pchunk->buf16 = (lChar16 *) realloc( pchunk->buf16, sizeof(lChar16)*(pchunk->len+1) );
            pchunk->size = pchunk->len;
        }
    }
    return *this;
}

lString16 & lString16::trim()
{
    //
    size_t firstns;
    for (firstns = 0; firstns<pchunk->len && 
        (pchunk->buf16[firstns]==' ' || pchunk->buf16[firstns]=='\t'); ++firstns)
        ;
    if (firstns >= pchunk->len)
    {
        clear();
        return *this;
    }
    size_t lastns;
    for (lastns = pchunk->len-1; lastns>0 && 
        (pchunk->buf16[lastns]==' ' || pchunk->buf16[lastns]=='\t'); --lastns)
        ;
    size_t newlen = lastns-firstns+1;
    if (newlen == pchunk->len)
        return *this;
    if (pchunk->nref == 1)
    {
        if (firstns>0)
            lStr_memcpy( pchunk->buf16, pchunk->buf16+firstns, newlen );
        pchunk->buf16[newlen] = 0;
        pchunk->len = newlen;
    }
    else
    {
        lstring_chunk_t * poldchunk = pchunk;
        release();
        alloc( newlen );
        _lStr_memcpy( pchunk->buf16, poldchunk->buf16+firstns, newlen );
        pchunk->buf16[newlen] = 0;
        pchunk->len = newlen;
    }
    return *this;
}

int lString16::atoi() const
{
    int n = 0;
    atoi(n);
    return n;
}

bool lString16::atoi( int &n ) const
{
    int sgn = 1;
    const lChar16 * s = c_str();
    while (*s == ' ' || *s == '\t')
        s++;
    if (*s == '-')
    {
        sgn = -1;
        s++;
    }
    else if (*s == '+')
    {
        s++;
    }
    if ( !(*s>='0' && *s<='9') )
        return false;
    while (*s>='0' && *s<='9')
    {
        n = n * 10 + ( (*s++)-'0' );
    }
    if ( sgn<0 )
        n = -n;
    return *s=='\0' || *s==' ' || *s=='\t';
}

bool lString16::atoi( lInt64 &n ) const
{
    int sgn = 1;
    const lChar16 * s = c_str();
    while (*s == ' ' || *s == '\t')
        s++;
    if (*s == '-')
    {
        sgn = -1;
        s++;
    }
    else if (*s == '+')
    {
        s++;
    }
    if ( !(*s>='0' && *s<='9') )
        return false;
    while (*s>='0' && *s<='9')
    {
        n = n * 10 + ( (*s++)-'0' );
    }
    if ( sgn<0 )
        n = -n;
    return *s=='\0' || *s==' ' || *s=='\t';
}

#define STRING_HASH_MULT 75317
lUInt32 lString16::getHash() const
{
    lUInt32 res = 0;
    for (lUInt32 i=0; i<pchunk->len; i++)
        res = res * STRING_HASH_MULT + pchunk->buf16[i];
    return res;
}



void lString16Collection::reserve( size_t space )
{
    if ( count + space > size )
    {
        size = count + space + 64;
        chunks = (lstring_chunk_t * *)realloc( chunks, sizeof(lstring_chunk_t *) * size );
    }
}
size_t lString16Collection::add( const lString16 & str )
{
    reserve( 1 );
    chunks[count] = str.pchunk;
    str.addref();
    return count++;
}
void lString16Collection::clear()
{
    for (size_t i=0; i<count; i++)
    {
        ((lString16 *)chunks)[i].release();
    }
    if (chunks)
        free(chunks);
    chunks = NULL;
    count = 0;
    size = 0;
}

void lString16Collection::erase(int offset, int cnt)
{
    if (count<=0)
        return;
    if (offset<0 || offset+cnt>=(int)count)
        return;
    int i;
    for (i=offset; i<offset+cnt; i++)
    {
        ((lString16 *)chunks)[i].release();
    }
    for (i=offset+cnt; i<(int)count; i++)
    {
        chunks[i-cnt] = chunks[i];
    }
    count -= cnt;
    if (!count)
        clear();    
}

void lString8Collection::erase(int offset, int cnt)
{
    if (count<=0)
        return;
    if (offset<0 || offset+cnt>(int)count)
        return;
    int i;
    for (i=offset; i<offset+cnt; i++)
    {
        ((lString8 *)chunks)[i].release();
    }
    for (i=offset+cnt; i<(int)count; i++)
    {
        chunks[i-cnt] = chunks[i];
    }
    count -= cnt;
    if (!count)
        clear();    
}

void lString8Collection::reserve( size_t space )
{
    if ( count + space > size )
    {
        size = count + space + 64;
        chunks = (lstring_chunk_t * *)realloc( chunks, sizeof(lstring_chunk_t *) * size );
    }
}
size_t lString8Collection::add( const lString8 & str )
{
    reserve( 1 );
    chunks[count] = str.pchunk;
    str.addref();
    return count++;
}
void lString8Collection::clear()
{
    for (size_t i=0; i<count; i++)
    {
        ((lString8 *)chunks)[i].release();
    }
    if (chunks)
        free(chunks);
    chunks = NULL;
    count = 0;
    size = 0;
}

lUInt32 calcStringHash( const lChar16 * s )
{
    lUInt32 a = 2166136261;
    while (*s)
    {
        a = a * 16777619 ^ (*s++);
    }
    return a;
}

void lString16HashedCollection::addHashItem( int hashIndex, int storageIndex )
{
    if ( hash[ hashIndex ].index == -1 ) {
        hash[hashIndex].index = storageIndex; 
    } else {
        HashPair * np = (HashPair *)malloc(sizeof(HashPair));
        np->index = storageIndex;
        np->next = hash[hashIndex].next;
        hash[hashIndex].next = np;
    }
}

void lString16HashedCollection::clearHash()
{
    if ( hash ) {
        for ( unsigned i=0; i<hashSize; i++) {
            HashPair * p = hash[i].next;
            while ( p ) {
                HashPair * tmp = p->next;
                free( p );
                p = tmp;
            }
        }
        free( hash );
    }
    hash = NULL;
}

lString16HashedCollection::lString16HashedCollection( lUInt32 hash_size )
: hashSize(hash_size), hash(NULL)
{

    hash = (HashPair *)malloc( sizeof(HashPair) * hashSize );
    for ( unsigned i=0; i<hashSize; i++ )
        hash[i].clear();
}

lString16HashedCollection::~lString16HashedCollection()
{
    clearHash();
}

size_t lString16HashedCollection::find( const lChar16 * s )
{
    if ( !hash || !length() )
        return (size_t)-1;
    lUInt32 h = calcStringHash( s );
    lUInt32 n = h % hashSize;
    if ( hash[n].index!=-1 )
    {
        const lString16 & str = at( hash[n].index );
        if ( str == s )
            return hash[n].index;
        HashPair * p = hash[n].next;
        for ( ;p ;p = p->next ) {
            const lString16 & str = at( p->index );
            if ( str==s )
                return p->index;
        }
    }
    return (size_t)-1;
}

void lString16HashedCollection::reHash( int newSize )
{
    if ( hashSize == (lUInt32)newSize )
        return;
    clearHash();
    hashSize = newSize;
    if ( hashSize>0 ) {
        hash = (HashPair *)malloc( sizeof(HashPair) * hashSize );
        for ( unsigned i=0; i<hashSize; i++ )
            hash[i].clear();
    }
    for ( unsigned i=0; i<length(); i++ ) {
        lUInt32 h = calcStringHash( at(i).c_str() );
        lUInt32 n = h % hashSize;
        addHashItem( n, i );
    }
}

size_t lString16HashedCollection::add( const lChar16 * s )
{
    if ( !hash || hashSize < length()*2 ) {
        unsigned sz = 16;
        while ( sz<length() )
            sz <<= 1;
        sz <<= 1;
        reHash( sz );
    }
    lUInt32 h = calcStringHash( s );
    lUInt32 n = h % hashSize;
    if ( hash[n].index!=-1 )
    {
        const lString16 & str = at( hash[n].index );
        if ( str == s )
            return hash[n].index;
        HashPair * p = hash[n].next;
        for ( ;p ;p = p->next ) {
            const lString16 & str = at( p->index );
            if ( str==s )
                return p->index;
        }
    }
    lUInt32 i = lString16Collection::add( lString16(s) );
    addHashItem( n, i );
    return i;
}

const lString16 lString16::empty_str;


////////////////////////////////////////////////////////////////////////////
// lString8
////////////////////////////////////////////////////////////////////////////

void lString8::free()
{
    ::free(pchunk->buf8);
#if (LDOM_USE_OWN_MEM_MAN == 1)
    for (int i=slices_count-1; i>=0; --i)
    {
        if (slices[i]->free_chunk(pchunk))
            return;
    }
    crFatalError(); // wrong pointer!!!
#else
    ::free(pchunk);
#endif
}

void lString8::alloc(size_t sz)
{
#if (LDOM_USE_OWN_MEM_MAN == 1)
    pchunk = lstring_chunk_t::alloc();
#else
    pchunk = (lstring_chunk_t*)::malloc(sizeof(lstring_chunk_t));
#endif
    pchunk->buf8 = (lChar8*) ::malloc( sizeof(lChar8) * (sz+1) );
    assert( pchunk->buf8!=NULL );
    pchunk->size = sz;
    pchunk->nref = 1;
}

lString8::lString8(const lChar8 * str)
{
    if (!str || !(*str))
    {
        pchunk = EMPTY_STR_8; 
        addref();
        return;
    }
    size_type len = _lStr_len(str);
    alloc( len );
    pchunk->len = len;
    _lStr_cpy( pchunk->buf8, str );
}

lString8::lString8(const lChar16 * str)
{
    if (!str || !(*str))
    {
        pchunk = EMPTY_STR_8; 
        addref();
        return;
    }
    size_type len = _lStr_len(str);
    alloc( len );
    pchunk->len = len;
    _lStr_cpy( pchunk->buf8, str );
}

lString8::lString8(const value_type * str, size_type count)
{
    if ( !str || !(*str) || count<=0 )
    {
        pchunk = EMPTY_STR_8; addref();
    }
    else
    {
        size_type len = _lStr_nlen(str, count);
        alloc(len);
        _lStr_ncpy( pchunk->buf8, str, len );
        pchunk->len = len;
    }
}

lString8::lString8(const lString8 & str, size_type offset, size_type count)
{
    if ( count > str.length() - offset )
        count = str.length() - offset;
    if (count<=0 || offset<0)
    {
        pchunk = EMPTY_STR_8; addref();
    }
    else
    {
        alloc(count);
        _lStr_memcpy( pchunk->buf8, str.pchunk->buf8+offset, count );
        pchunk->buf8[count]=0;
        pchunk->len = count;
    }
}

lString8 & lString8::assign(const lChar8 * str)
{
    if (!str || !(*str))
    {
        clear();
    }
    else
    {
        size_type len = _lStr_len(str);
        if (pchunk->nref==1)
        { 
            if (pchunk->size<=len)
            {
                // resize is necessary
                pchunk->buf8 = (lChar8*) ::realloc( pchunk->buf8, sizeof(lChar8)*(len+1) );
                pchunk->size = len+1;
            }
        }
        else
        {
            release();
            alloc(len);
        }
        _lStr_cpy( pchunk->buf8, str );
        pchunk->len = len;
    }
    return *this;
}

lString8 & lString8::assign(const lChar8 * str, size_type count)
{
    if ( !str || !(*str) || count<=0 )
    {
        clear();
    }
    else
    {
        size_type len = _lStr_nlen(str, count);
        if (pchunk->nref==1)
        { 
            if (pchunk->size<=len)
            {
                // resize is necessary
                pchunk->buf8 = (lChar8*) ::realloc( pchunk->buf8, sizeof(lChar8)*(len+1) );
                pchunk->size = len+1;
            }
        }
        else
        {
            release();
            alloc(len);
        }
        _lStr_ncpy( pchunk->buf8, str, count );
        pchunk->len = len;
    }
    return *this;
}

lString8 & lString8::assign(const lString8 & str, size_type offset, size_type count)
{
    if ( count > str.length() - offset )
        count = str.length() - offset;
    if (count<=0 || offset<0)
    {
        clear();
    }
    else
    {
        if (pchunk==str.pchunk)
        {
            if (&str != this)
            {
                release();
                alloc(count);
            }
            if (offset>0)
            {
                _lStr_memcpy( pchunk->buf8, str.pchunk->buf8+offset, count );
            }
            pchunk->buf8[count]=0;
        }
        else
        {
            if (pchunk->nref==1)
            { 
                if (pchunk->size<=count)
                {
                    // resize is necessary
                    pchunk->buf8 = (lChar8*) ::realloc( pchunk->buf8, sizeof(lChar8)*(count+1) );
                    pchunk->size = count+1;
                }
            }
            else
            {
                release();
                alloc(count);
            }
            _lStr_memcpy( pchunk->buf8, str.pchunk->buf8+offset, count );
            pchunk->buf8[count]=0;
        }
        pchunk->len = count;
    }
    return *this;
}

lString8 & lString8::erase(size_type offset, size_type count)
{
    if ( count > length() - offset )
        count = length() - offset;
    if (count<=0 || offset<0)
    {
        clear();
    }
    else
    {
        size_type newlen = length()-count;
        if (pchunk->nref==1)
        {
            _lStr_memcpy( pchunk->buf8+offset, pchunk->buf8+offset+count, newlen-offset+1 );
        }
        else
        {
            lstring_chunk_t * poldchunk = pchunk;
            release();
            alloc( newlen );
            _lStr_memcpy( pchunk->buf8, poldchunk->buf8, offset );
            _lStr_memcpy( pchunk->buf8+offset, poldchunk->buf8+offset+count, newlen-offset+1 );
        }
        pchunk->len = newlen;
        pchunk->buf8[newlen]=0;
    }
    return *this;
}

void lString8::reserve(size_type n)
{
    if (pchunk->nref==1)
    {
        if (pchunk->size<=n)
        {
            pchunk->buf8 = (lChar8*) ::realloc( pchunk->buf8, sizeof(lChar8)*(n+1) );
            pchunk->size = n;
        }
    }
    else
    {
        lstring_chunk_t * poldchunk = pchunk;
        release();
        alloc( n );
        _lStr_memcpy( pchunk->buf8, poldchunk->buf8, poldchunk->len+1 );
        pchunk->len = poldchunk->len;
    }
}

void lString8::lock( size_type newsize )
{
    if (pchunk->nref>1)
    {
        lstring_chunk_t * poldchunk = pchunk;
        release();
        alloc( newsize );
        size_type len = newsize;
        if (len>poldchunk->len)
            len = poldchunk->len;
        _lStr_memcpy( pchunk->buf8, poldchunk->buf8, len );
        pchunk->buf8[len]=0;
        pchunk->len = len;
    }
}

// lock string, allocate buffer and reset length to 0
void lString8::reset( size_type size )
{
    if (pchunk->nref>1 || pchunk->size<size)
    {
        release();
        alloc( size );
    }
    pchunk->buf8[0] = 0;
    pchunk->len = 0;
}

void lString8::resize(size_type n, lChar8 e)
{
    lock( n );
    if (n>=pchunk->size)
    {
        pchunk->buf8 = (lChar8*) ::realloc( pchunk->buf8, sizeof(lChar8)*(n+1) );
        pchunk->size = n;
    }
    // fill with data if expanded
    for (size_type i=pchunk->len; i<n; i++)
        pchunk->buf8[i] = e;
    pchunk->buf8[pchunk->len] = 0;
}

lString8 & lString8::append(const lChar8 * str)
{
    size_type len = _lStr_len(str);
    reserve( pchunk->len+len );
    _lStr_memcpy(pchunk->buf8+pchunk->len, str, len+1);
    pchunk->len += len;
    return *this;
}

lString8 & lString8::append(const lChar8 * str, size_type count)
{
    size_type len = _lStr_nlen(str, count);
    reserve( pchunk->len+len );
    _lStr_ncpy(pchunk->buf8+pchunk->len, str, len);
    pchunk->len += len;
    return *this;
}

lString8 & lString8::append(const lString8 & str)
{
    size_type len2 = pchunk->len + str.pchunk->len;
    reserve( len2 );
    _lStr_memcpy( pchunk->buf8+pchunk->len, str.pchunk->buf8, str.pchunk->len+1 );
    pchunk->len = len2;
    return *this;
}

lString8 & lString8::append(const lString8 & str, size_type offset, size_type count)
{
    if ( str.pchunk->len>offset )
    {
        if ( offset + count > str.pchunk->len )
            count = str.pchunk->len - offset;
        reserve( pchunk->len+count );
        _lStr_ncpy(pchunk->buf8 + pchunk->len, str.pchunk->buf8 + offset, count);
        pchunk->len += count;
        pchunk->buf8[pchunk->len] = 0;
    }
    return *this;
}

lString8 & lString8::append(size_type count, lChar8 ch)
{
    reserve( pchunk->len+count );
    _lStr_memset(pchunk->buf8+pchunk->len, ch, count);
    pchunk->len += count;
    pchunk->buf8[pchunk->len] = 0;
    return *this;
}

lString8 & lString8::insert(size_type p0, size_type count, lChar8 ch)
{
    if (p0>pchunk->len)
        p0 = pchunk->len;
    reserve( pchunk->len+count );
    for (size_type i=pchunk->len+count; i>p0; i--)
        pchunk->buf8[i] = pchunk->buf8[i-1];
    _lStr_memset(pchunk->buf8+p0, ch, count);
    pchunk->len += count;
    pchunk->buf8[pchunk->len] = 0;
    return *this;
}

lString8 lString8::substr(size_type pos, size_type n) const
{
    if (pos>=length())
        return lString8();
    if (pos+n>length())
        n = length() - pos;
    return lString8( pchunk->buf8+pos, n );
}

int lString8::pos(lString8 subStr) const
{
    if (subStr.length()>length())
        return -1;
    int l = subStr.length();
    int dl = length() - l;
    for (int i=0; i<=dl; i++)
    {
        int flg = 1;
        for (int j=0; j<l; j++)
            if (pchunk->buf8[i+j]!=subStr.pchunk->buf8[j])
            {
                flg = 0;
                break;
            }
        if (flg)
            return i;
    }
    return -1;
}

int lString16::pos(lString16 subStr) const
{
    if (subStr.length()>length())
        return -1;
    int l = subStr.length();
    int dl = length() - l;
    for (int i=0; i<=dl; i++)
    {
        int flg = 1;
        for (int j=0; j<l; j++)
            if (pchunk->buf16[i+j]!=subStr.pchunk->buf16[j])
            {
                flg = 0;
                break;
            }
        if (flg)
            return i;
    }
    return -1;
}

lString8 & lString8::pack()
{
    if (pchunk->len + 4 < pchunk->size )
    {
        if (pchunk->nref>1)
        {
            lock(pchunk->len);
        }
        else
        {
            pchunk->buf8 = (lChar8 *) realloc( pchunk->buf8, sizeof(lChar8)*(pchunk->len+1) );
            pchunk->size = pchunk->len;
        }
    }
    return *this;
}

lString8 & lString8::trim()
{
    //
    size_t firstns;
    for (firstns = 0; 
            firstns<pchunk->len && 
            (pchunk->buf8[firstns]==' ' || 
            pchunk->buf8[firstns]=='\t'); 
            ++firstns)
        ;
    if (firstns >= pchunk->len)
    {
        clear();
        return *this;
    }
    size_t lastns;
    for (lastns = pchunk->len-1; 
            lastns>0 && 
            (pchunk->buf8[lastns]==' ' || pchunk->buf8[lastns]=='\t'); 
            --lastns)
        ;
    size_t newlen = lastns-firstns+1;
    if (newlen == pchunk->len)
        return *this;
    if (pchunk->nref == 1)
    {
        if (firstns>0)
            lStr_memcpy( pchunk->buf8, pchunk->buf8+firstns, newlen );
        pchunk->buf8[newlen] = 0;
        pchunk->len = newlen;
    }
    else
    {
        lstring_chunk_t * poldchunk = pchunk;
        release();
        alloc( newlen );
        _lStr_memcpy( pchunk->buf8, poldchunk->buf8+firstns, newlen );
        pchunk->buf8[newlen] = 0;
        pchunk->len = newlen;
    }
    return *this;
}

int lString8::atoi() const
{
    int sgn = 1;
    int n = 0;
    const lChar8 * s = c_str();
    while (*s == ' ' || *s == '\t')
        s++;
    if (*s == '-')
    {
        sgn = -1;
        s++;
    }
    else if (*s == '+')
    {
        s++;
    }
    while (*s>='0' && *s<='9')
    {
        n = n * 10 + ( (*s)-'0' );
    }
    return (sgn>0)?n:-n;
}

// constructs string representation of integer
lString8 lString8::itoa( int n )
{
    lChar8 buf[16];
    int i=0;
    int negative = 0;
    if (n==0)
        return lString8("0");
    else if (n<0)
    {
        negative = 1;
        n = -n;
    }
    for ( ; n; n/=10 )
    {
        buf[i++] = '0' + (n%10);
    }
    lString8 res;
    res.reserve(i+negative);
    if (negative)
        res.append(1, '-');
    for (int j=i-1; j>=0; j--)
        res.append(1, buf[j]);
    return res;
}

// constructs string representation of integer
lString8 lString8::itoa( unsigned int n )
{
    lChar8 buf[16];
    int i=0;
    if (n==0)
        return lString8("0");
    for ( ; n; n/=10 )
    {
        buf[i++] = '0' + (n%10);
    }
    lString8 res;
    res.reserve(i);
    for (int j=i-1; j>=0; j--)
        res.append(1, buf[j]);
    return res;
}

// constructs string representation of integer
lString16 lString16::itoa( int n )
{
    return itoa( (lInt64)n );
}

// constructs string representation of integer
lString16 lString16::itoa( lInt64 n )
{
    lChar16 buf[16];
    int i=0;
    int negative = 0;
    if (n==0)
        return lString16("0");
    else if (n<0)
    {
        negative = 1;
        n = -n;
    }
    for ( ; n; n/=10 )
    {
        buf[i++] = (lChar16)('0' + (n%10));
    }
    lString16 res;
    res.reserve(i+negative);
    if (negative)
        res.append(1, L'-');
    for (int j=i-1; j>=0; j--)
        res.append(1, buf[j]);
    return res;
}

bool lvUnicodeIsAlpha( lChar16 ch )
{
    if ( ch<128 ) {
        if ( (ch>='a' && ch<='z') || (ch>='A' && ch<='Z') )
            return true;
    } else if ( ch>=0xC0 && ch<=0x1ef9) {
        return true;
    }
    return false;
}


lString16 & lString16::uppercase()
{
    lStr_uppercase( modify(), length() );
    return *this;
}

lString16 & lString16::lowercase()
{
    lStr_lowercase( modify(), length() );
    return *this;
}

void lStr_uppercase( lChar16 * str, int len )
{
    for ( int i=0; i<len; i++ ) {
        lChar16 ch = str[i];
        if ( ch>='a' && ch<='z' ) {
            str[i] = ch - 0x20;
        } else if ( ch>=0xE0 && ch<=0xFF ) {
            str[i] = ch - 0x20;
        } else if ( ch>=0x430 && ch<=0x44F ) {
            str[i] = ch - 0x20;
        }
    }
}

void lStr_lowercase( lChar16 * str, int len )
{
    for ( int i=0; i<len; i++ ) {
        lChar16 ch = str[i];
        if ( ch>='A' && ch<='Z' ) {
            str[i] = ch + 0x20;
        } else if ( ch>=0xC0 && ch<=0xDF ) {
            str[i] = ch + 0x20;
        } else if ( ch>=0x410 && ch<=0x42F ) {
            str[i] = ch + 0x20;
        }
    }
}

void lString16Collection::parse( lString16 string, lChar16 delimiter, bool flgTrim )
{
    int wstart=0;
    for ( unsigned i=0; i<=string.length(); i++ ) {
        if ( i==string.length() || string[i]==delimiter ) {
            lString16 s( string.substr( wstart, i-wstart) );
            if ( flgTrim )
                s.trimDoubleSpaces(false, false, false);
            if ( !flgTrim || !s.empty() )
                add( s );
            wstart = i+1;
        }
    }
}

lString16 & lString16::trimDoubleSpaces( bool allowStartSpace, bool allowEndSpace, bool removeEolHyphens )
{
    if ( empty() )
        return *this;
    lChar16 * buf = modify();
    lChar16 * psrc = buf;
    lChar16 * pdst = buf;
    int state = 0; // 0=beginning, 1=after space, 2=after non-space
    while (*psrc ) {
        lChar16 ch = *psrc++;
        if ( ch==' ' || ch=='\t' ) {
            if ( state==2 ) {
                if ( *psrc || allowEndSpace ) // if not last
                    *pdst++ = ' ';
            } else if ( state==0 && allowStartSpace ) {
                *pdst++ = ' ';
            }
            state = 1;
        } else if ( ch=='\r' || ch=='\n' ) {
            if ( state==2 ) {
                if ( removeEolHyphens && pdst>(buf+1) && *(pdst-1)=='-' && lvUnicodeIsAlpha(*(pdst-2)) )
                    pdst--; // remove hyphen at end of line
                if ( *psrc || allowEndSpace ) // if not last
                    *pdst++ = ' ';
            } else if ( state==0 && allowStartSpace ) {
                *pdst++ = ' ';
            }
            state = 1;
        } else {
            *pdst++ = ch;
            state = 2;
        }
    }
    if ( pdst==buf ) {
        clear();
        return *this;
    }
    if ( pdst==psrc ) {
        // was not changed
        return *this;
    }
    // truncated: erase extra characters
    int chars_to_delete = psrc-pdst;
    erase( length()-chars_to_delete, chars_to_delete );
    return *this;
}

// constructs string representation of integer
lString16 lString16::itoa( unsigned int n )
{
    return itoa( (lUInt64) n );
}

// constructs string representation of integer
lString16 lString16::itoa( lUInt64 n )
{
    lChar16 buf[16];
    int i=0;
    if (n==0)
        return lString16("0");
    for ( ; n; n/=10 )
    {
        buf[i++] = (lChar16)('0' + (n%10));
    }
    lString16 res;
    res.reserve(i);
    for (int j=i-1; j>=0; j--)
        res.append(1, buf[j]);
    return res;
}


lUInt32 lString8::getHash() const
{
    lUInt32 res = 0;
    for (lUInt32 i=0; i<pchunk->len; i++)
        res = res * STRING_HASH_MULT + pchunk->buf8[i];
    return res;
}

const lString8 lString8::empty_str;

int Utf8CharCount( const lChar8 * str )
{
    int count = 0;
    lUInt8 ch;
    while ( (ch=*str++) ) {
        if ( (ch & 0x80) == 0 ) {
        } else if ( (ch & 0xE0) == 0xC0 ) {
            if ( !(ch=*str++) )
                break;
        } else {
            if ( !(ch=*str++) )
                break;
            if ( !(ch=*str++) )
                break;
        }
        count++;
    }
    return count;
}

int Utf8ByteCount( const lChar16 * str )
{
    int count = 0;
    lUInt16 ch;
    while ( (ch=*str++) ) {
        if ( (ch & 0xFF80) == 0 ) {
            count++;
        } else if ( (ch & 0xF800) == 0 ) {
            count += 2;
        } else {
            count += 3;
        }
    }
    return count;
}

lString16 Utf8ToUnicode( const lString8 & str )
{
    lString16 dst;
    if (str.empty())
      return dst;
    const char * s = str.c_str();
    int len = Utf8CharCount( s );
    if (!len)
      return dst;
    dst.reserve( len );
    lUInt16 ch;
    while ( (ch=*s++) ) {
        if ( (ch & 0x80) == 0 ) {
            dst << ch;
        } else if ( (ch & 0xE0) == 0xC0 ) {
            lChar16 d = (ch & 0x1F) << 6;
            if ( !(ch=*s++) )
                break;
            d |= (ch & 0x3F);
            dst << d;
        } else {
            lChar16 d = (ch & 0x0F) << 12;
            if ( !(ch=*s++) )
                break;
            d |= (ch & 0x3F) << 6;
            if ( !(ch=*s++) )
                break;
            d |= (ch & 0x3F);
            dst << d;
        }
    }
    return dst;
}


lString8 UnicodeToUtf8( const lString16 & str )
{
    lString8 dst;
    if (str.empty())
      return dst;
    const lChar16 * s = str.c_str();
    int len = Utf8ByteCount( s );
    dst.reserve( len );
    lUInt16 ch;
    while ( (ch=*s++) ) {
        if ( (ch & 0xFF80) == 0 ) {
            dst << (lUInt8)ch;
        } else if ( (ch & 0xF800) == 0 ) {
            dst << (lUInt8) ( ((ch >> 6) & 0x1F) | 0xC0 );
            dst << (lUInt8) ( ((ch ) & 0x3F) | 0x80 );
        } else {
            dst << (lUInt8) ( ((ch >> 12) & 0x0F) | 0xE0 );
            dst << (lUInt8) ( ((ch >> 6) & 0x3F) | 0x80 );
            dst << (lUInt8) ( ((ch ) & 0x3F) | 0x80 );
        }
    }
    return dst;
}


lString8 UnicodeTo8Bit( const lString16 & str, const lChar8 * * table )
{
    lString8 buf;
    buf.reserve( str.length() );
    for ( int i=0; i<(int)str.length(); i++ ) {
        lChar16 ch = str[i];
        const lChar8 * p = table[ (ch>>8) & 255 ];
        if ( p ) {
            buf += p[ ch&255 ];
        } else {
            buf += '?';
        }
    }
    return buf;
}

#if !defined(__SYMBIAN32__) && defined(_WIN32)

lString8 UnicodeToLocal( const lString16 & str )
{
   lString8 dst;
   if (str.empty())
      return dst;
   char def_char[]="?";
   int usedDefChar = false;
   int len = WideCharToMultiByte(
      CP_ACP,
      WC_COMPOSITECHECK | WC_DISCARDNS 
       | WC_SEPCHARS | WC_DEFAULTCHAR,
      str.c_str(),
      str.length(),
      NULL,
      0,
      def_char,
      &usedDefChar
      );
   if (len)
   {
      dst.insert(0, len, ' ');
      WideCharToMultiByte(
         CP_ACP,
         WC_COMPOSITECHECK | WC_DISCARDNS 
          | WC_SEPCHARS | WC_DEFAULTCHAR,
         str.c_str(),
         str.length(),
         dst.modify(),
         len,
         def_char,
         &usedDefChar
         );
   }
   return dst;
}

lString16 LocalToUnicode( const lString8 & str )
{
   lString16 dst;
   if (str.empty())
      return dst;
   int len = MultiByteToWideChar(
      CP_ACP,
      0,
      str.c_str(),
      str.length(),
      NULL,
      0
      );
   if (len)
   {
      dst.insert(0, len, ' ');
      MultiByteToWideChar(
         CP_ACP,
         0,
         str.c_str(),
         str.length(),
         dst.modify(),
         len
         );
   }
   return dst;
}

#else

lString8 UnicodeToLocal( const lString16 & str )
{
    return UnicodeToUtf8( str );
}

lString16 LocalToUnicode( const lString8 & str )
{
    return Utf8ToUnicode( str );
}

#endif

//0x410
static const char * russian_capital[32] =
{
"A", "B", "V", "G", "D", "E", "ZH", "Z", "I", "j", "K", "L", "M", "N", "O", "P", "R", 
"S", "T", "U", "F", "H", "TS", "CH", "SH", "SH", "\'", "Y", "\'", "E", "YU", "YA"
};
static const char * russian_small[32] =
{
"a", "b", "v", "g", "d", "e", "zh", "z", "i", "j", "k", "l", "m", "n", "o", "p", "r", 
"s", "t", "u", "f", "h", "ts", "ch", "sh", "sh", "\'", "y", "\'", "e", "yu", "ya"
};
static const char * getCharTranscript( lChar16 ch )
{
    if ( ch>=0x410 && ch<0x430 )
        return russian_capital[ch-0x410];
    else if (ch>=0x430 && ch<0x450)
        return russian_small[ch-0x430];
    else if (ch==0x450)
        return "E";
    else if ( ch==0x451 )
        return "e";
    return "?";
}


lString8  UnicodeToTranslit( const lString16 & str )
{
    lString8 buf;
	if ( str.empty() )
		return buf;
    buf.reserve( str.length()*5/4 );
    for ( unsigned i=0; i<str.length(); i++ ) {
		lChar16 ch = str[i];
        if ( ch>=32 && ch<=127 ) {
            buf.append( 1, (lChar8)ch );
        } else {
            const char * trans = getCharTranscript(ch);
            buf.append( trans );
        }
	}
    buf.pack();
    return buf;
}



static lUInt16 char_props[] = {
// 0x0000:
0,0,0,0, 0,0,0,0, CH_PROP_SPACE,CH_PROP_SPACE,CH_PROP_SPACE,0, CH_PROP_SPACE,CH_PROP_SPACE,0,0,
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
// 0x0020:
CH_PROP_SPACE, // ' '
CH_PROP_PUNCT, // '!'
0, // '\"'
CH_PROP_SIGN, // '#'
CH_PROP_SIGN, // '$'
CH_PROP_SIGN, // '%'
CH_PROP_SIGN, // '&'
CH_PROP_SIGN, // '\''
0, // '('
0, // ')'
CH_PROP_SIGN, // '*'
CH_PROP_SIGN, // '+'
CH_PROP_PUNCT, // ','
CH_PROP_SIGN, // '-'
CH_PROP_PUNCT, // '.'
CH_PROP_SIGN, // '/'
// 0x0030:
CH_PROP_DIGIT, // '0'
CH_PROP_DIGIT, // '1'
CH_PROP_DIGIT, // '2'
CH_PROP_DIGIT, // '3'
CH_PROP_DIGIT, // '4'
CH_PROP_DIGIT, // '5'
CH_PROP_DIGIT, // '6'
CH_PROP_DIGIT, // '7'
CH_PROP_DIGIT, // '8'
CH_PROP_DIGIT, // '9'
CH_PROP_DIGIT, // ':'
CH_PROP_DIGIT, // ';'
CH_PROP_DIGIT, // '<'
CH_PROP_DIGIT, // '='
CH_PROP_DIGIT, // '>'
CH_PROP_DIGIT, // '?'
// 0x0040:
CH_PROP_SIGN,  // '@'
CH_PROP_UPPER | CH_PROP_VOWEL,     // 'A'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'B'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'C'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'D'
CH_PROP_UPPER | CH_PROP_VOWEL, // 'E'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'F'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'G'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'H'
CH_PROP_UPPER | CH_PROP_VOWEL, // 'I'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'J'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'K'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'L'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'M'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'N'
CH_PROP_UPPER | CH_PROP_VOWEL, // 'O'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'P'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'Q'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'R'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'S'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'T'
CH_PROP_UPPER | CH_PROP_VOWEL, // 'U'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'V'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'W'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'X'
CH_PROP_UPPER | CH_PROP_VOWEL, // 'Y'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'Z'
CH_PROP_SIGN, // '['
CH_PROP_SIGN, // '\'
CH_PROP_SIGN, // ']'
CH_PROP_SIGN, // '^'
CH_PROP_SIGN, // '_'
// 0x0060:
CH_PROP_SIGN,  // '`'
CH_PROP_UPPER | CH_PROP_VOWEL,     // 'a'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'b'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'c'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'd'
CH_PROP_UPPER | CH_PROP_VOWEL, // 'e'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'f'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'g'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'h'
CH_PROP_UPPER | CH_PROP_VOWEL, // 'i'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'j'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'k'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'l'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'm'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'n'
CH_PROP_UPPER | CH_PROP_VOWEL, // 'o'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'p'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'q'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'r'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 's'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 't'
CH_PROP_UPPER | CH_PROP_VOWEL, // 'u'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'v'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'w'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'x'
CH_PROP_UPPER | CH_PROP_VOWEL, // 'y'
CH_PROP_UPPER | CH_PROP_CONSONANT, // 'z'
CH_PROP_SIGN, // '{'
CH_PROP_SIGN, // '|'
CH_PROP_SIGN, // '}'
CH_PROP_SIGN, // '~'
CH_PROP_SIGN, // ' '
// 0x0080:
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
// 0x0090:
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
// 0x00A0:
CH_PROP_SPACE, // 00A0 nbsp
CH_PROP_PUNCT, // 00A1 inverted !
CH_PROP_SIGN,  // 00A2 
CH_PROP_SIGN,  // 00A3 
CH_PROP_SIGN,  // 00A4 
CH_PROP_SIGN,  // 00A5 
CH_PROP_SIGN,  // 00A6 
CH_PROP_SIGN,  // 00A7 
CH_PROP_SIGN,  // 00A8 
CH_PROP_SIGN,  // 00A9
CH_PROP_SIGN,  // 00AA 
CH_PROP_SIGN,  // 00AB 
CH_PROP_SIGN,  // 00AC 
CH_PROP_SIGN,  // 00AD 
CH_PROP_SIGN,  // 00AE 
CH_PROP_SIGN,  // 00AF
// 0x00A0:
CH_PROP_SIGN,  // 00B0 degree
CH_PROP_SIGN,  // 00B1
CH_PROP_SIGN,  // 00B2 
CH_PROP_SIGN,  // 00B3 
CH_PROP_SIGN,  // 00B4 
CH_PROP_SIGN,  // 00B5 
CH_PROP_SIGN,  // 00B6 
CH_PROP_SIGN,  // 00B7 
CH_PROP_SIGN,  // 00B8 
CH_PROP_SIGN,  // 00B9
CH_PROP_SIGN,  // 00BA 
CH_PROP_SIGN,  // 00BB 
CH_PROP_SIGN,  // 00BC 
CH_PROP_SIGN,  // 00BD 
CH_PROP_SIGN,  // 00BE 
CH_PROP_PUNCT, // 00BF
// 0x00C0:
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00C0 A`
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00C1 A'
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00C2 A^
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00C3 A"
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00C4 A:
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00C5 Ao
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00C6 AE
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 00C7 C~
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00C8 E`
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00C9 E'
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00CA E^
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00CB E:
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00CC I`
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00CD I'
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00CE I^
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00CF I:
// 0x00D0:
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 00D0 D-
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 00D1 N-
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00D2 O`
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00D3 O'
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00D4 O^
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00D5 O"
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00D6 O:
CH_PROP_SIGN,  // 00D7 x
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00D8 O/
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00D9 U`
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00DA U'
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00DB U^
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00DC U:
CH_PROP_UPPER | CH_PROP_VOWEL,  // 00DD Y'
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 00DE P thorn
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 00DF ss
// 0x00E0:
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00E0 a`
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00E1 a'
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00E2 a^
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00E3 a"
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00E4 a:
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00E5 ao
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00E6 ae
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 00E7 c~
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00E8 e`
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00E9 e'
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00EA e^
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00EB e:
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00EC i`
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00ED i'
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00EE i^
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00EF i:
// 0x00F0:
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 00F0 eth
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 00F1 n~
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00F2 o`
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00F3 o'
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00F4 o^
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00F5 o"
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00F6 o:
CH_PROP_SIGN,  // 00F7 / (%)
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00F8 o/
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00F9 u`
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00FA u'
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00FB u^
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00FC u:
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00FD y'
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 00FE p thorn
CH_PROP_LOWER | CH_PROP_VOWEL,  // 00FF y:
// 0x0100:
CH_PROP_UPPER | CH_PROP_VOWEL,  // 0100 A_
CH_PROP_LOWER | CH_PROP_VOWEL,  // 0101 a_
CH_PROP_UPPER | CH_PROP_VOWEL,  // 0102 Au
CH_PROP_LOWER | CH_PROP_VOWEL,  // 0103 au
CH_PROP_UPPER | CH_PROP_VOWEL,  // 0104 A,
CH_PROP_LOWER | CH_PROP_VOWEL,  // 0105 a,
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0106 C'
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0107 c'
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0108 C^
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0109 c^
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 010A C.
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 010B c.
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 010C Cu
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 010D cu
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 010E Du
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 010F d'

CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0110 D-
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0111 d-
CH_PROP_UPPER | CH_PROP_VOWEL,  // 0112 E_
CH_PROP_LOWER | CH_PROP_VOWEL,  // 0113 e_
CH_PROP_UPPER | CH_PROP_VOWEL,  // 0114 Eu
CH_PROP_LOWER | CH_PROP_VOWEL,  // 0115 eu
CH_PROP_UPPER | CH_PROP_VOWEL,  // 0116 E.
CH_PROP_LOWER | CH_PROP_VOWEL,  // 0117 e.
CH_PROP_UPPER | CH_PROP_VOWEL,  // 0118 E,
CH_PROP_LOWER | CH_PROP_VOWEL,  // 0119 e,
CH_PROP_UPPER | CH_PROP_VOWEL,  // 011A Ev
CH_PROP_LOWER | CH_PROP_VOWEL,  // 011B ev
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 011C G^
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 011D g^
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 011E Gu
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 011F Gu

CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0120 G.
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0121 g.
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0122 G,
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0123 g,
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0124 H^
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0125 h^
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0126 H-
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0127 h-
CH_PROP_UPPER | CH_PROP_VOWEL,  // 0128 I~
CH_PROP_LOWER | CH_PROP_VOWEL,  // 0129 i~
CH_PROP_UPPER | CH_PROP_VOWEL,  // 012A I_
CH_PROP_LOWER | CH_PROP_VOWEL,  // 012B i_
CH_PROP_UPPER | CH_PROP_VOWEL,  // 012C Iu
CH_PROP_LOWER | CH_PROP_VOWEL,  // 012D iu
CH_PROP_UPPER | CH_PROP_VOWEL,  // 012E I,
CH_PROP_LOWER | CH_PROP_VOWEL,  // 012F i,

CH_PROP_UPPER | CH_PROP_VOWEL,  // 0130 I.
CH_PROP_LOWER | CH_PROP_VOWEL,  // 0131 i-.
CH_PROP_UPPER | CH_PROP_VOWEL,  // 0132 IJ
CH_PROP_LOWER | CH_PROP_VOWEL,  // 0133 ij
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0134 J^
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0135 j^
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0136 K,
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0137 k,
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0138 k (kra)
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0139 L'
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 013A l'
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 013B L,
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 013C l,
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 013D L'
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 013E l'
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 013F L.

CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0140 l.
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0141 L/
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0142 l/
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0143 N'
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0144 n'
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0145 N,
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0146 n,
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0147 Nv
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0148 nv
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0149 `n
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 014A Ng
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 014B ng
CH_PROP_UPPER | CH_PROP_VOWEL,  // 014C O_
CH_PROP_LOWER | CH_PROP_VOWEL,  // 014D o-.
CH_PROP_UPPER | CH_PROP_VOWEL,  // 014E Ou
CH_PROP_LOWER | CH_PROP_VOWEL,  // 014F ou

CH_PROP_UPPER | CH_PROP_VOWEL,  // 0150 O"
CH_PROP_LOWER | CH_PROP_VOWEL,  // 0151 o"
CH_PROP_UPPER | CH_PROP_VOWEL,  // 0152 Oe
CH_PROP_LOWER | CH_PROP_VOWEL,  // 0153 oe
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0154 R'
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0155 r'
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0156 R,
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0157 r,
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0158 Rv
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0159 rv
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 015A S'
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 015B s'
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 015C S^
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 015D s^
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 015E S,
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 015F s,

CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0160 Sv
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0161 sv
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0162 T,
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0163 T,
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0164 Tv
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0165 Tv
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0166 T-
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0167 T-
CH_PROP_UPPER | CH_PROP_VOWEL,  // 0168 U~
CH_PROP_LOWER | CH_PROP_VOWEL,  // 0169 u~
CH_PROP_UPPER | CH_PROP_VOWEL,  // 016A U_
CH_PROP_LOWER | CH_PROP_VOWEL,  // 016B u_
CH_PROP_UPPER | CH_PROP_VOWEL,  // 016C Uu
CH_PROP_LOWER | CH_PROP_VOWEL,  // 016D uu
CH_PROP_UPPER | CH_PROP_VOWEL,  // 016E Uo
CH_PROP_LOWER | CH_PROP_VOWEL,  // 016F uo

CH_PROP_UPPER | CH_PROP_VOWEL,  // 0170 U"
CH_PROP_LOWER | CH_PROP_VOWEL,  // 0171 u"
CH_PROP_UPPER | CH_PROP_VOWEL,  // 0172 U,
CH_PROP_LOWER | CH_PROP_VOWEL,  // 0173 u,
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0174 W^
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0175 w^
CH_PROP_UPPER | CH_PROP_VOWEL,  // 0176 Y,
CH_PROP_LOWER | CH_PROP_VOWEL,  // 0177 y,
CH_PROP_UPPER | CH_PROP_VOWEL,  // 0178 Y:
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0179 Z'
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 017A z'
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 017B Z.
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 017C z.
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 017D Zv
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 017E zv
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 017F s long
// 0x0180:
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
// 0x0190:
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
// 0x01A0:
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
// 0x01B0:
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
// 0x01C0:
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
// 0x01D0:
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
// 0x01E0:
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
// 0x01F0:
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
// 0x0200:
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
// 0x0300:
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
// 0x0400:
0,  // 0400
CH_PROP_UPPER | CH_PROP_VOWEL,      // 0401 cyrillic E:
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0402 cyrillic Dje
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0403 cyrillic Gje
CH_PROP_UPPER | CH_PROP_VOWEL,      // 0404 cyrillic ukr Ie
CH_PROP_UPPER | CH_PROP_CONSONANT,  // 0405 cyrillic Dze
CH_PROP_UPPER | CH_PROP_VOWEL,      // 0406 cyrillic ukr I
CH_PROP_UPPER | CH_PROP_VOWEL,      // 0407 cyrillic ukr I:
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 0408 cyrillic J
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 0409 cyrillic L'
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 040A cyrillic N'
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 040B cyrillic Th
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 040C cyrillic K'
0,      // 040D cyrillic
CH_PROP_UPPER | CH_PROP_VOWEL,      // 040E cyrillic Yu
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 040F cyrillic Dzhe
// 0x0410:
CH_PROP_UPPER | CH_PROP_VOWEL,      // 0410 cyrillic A
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 0411 cyrillic B
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 0412 cyrillic V
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 0413 cyrillic G
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 0414 cyrillic D
CH_PROP_UPPER | CH_PROP_VOWEL,      // 0415 cyrillic E
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 0416 cyrillic Zh
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 0417 cyrillic Z
CH_PROP_UPPER | CH_PROP_VOWEL,      // 0418 cyrillic I
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 0419 cyrillic YI
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 041A cyrillic K
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 041B cyrillic L
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 041C cyrillic M
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 041D cyrillic N
CH_PROP_UPPER | CH_PROP_VOWEL,      // 041E cyrillic O
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 041F cyrillic P
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 0420 cyrillic R
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 0421 cyrillic S
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 0422 cyrillic T
CH_PROP_UPPER | CH_PROP_VOWEL,      // 0423 cyrillic U
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 0424 cyrillic F
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 0425 cyrillic H
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 0426 cyrillic C
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 0427 cyrillic Ch
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 0428 cyrillic Sh
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 0429 cyrillic Sch
CH_PROP_UPPER | CH_PROP_ALPHA_SIGN,      // 042A cyrillic Hard sign
CH_PROP_UPPER | CH_PROP_VOWEL,      // 042B cyrillic Y
CH_PROP_UPPER | CH_PROP_ALPHA_SIGN,      // 042C cyrillic Soft sign
CH_PROP_UPPER | CH_PROP_VOWEL,      // 042D cyrillic EE
CH_PROP_UPPER | CH_PROP_VOWEL,      // 042E cyrillic Yu
CH_PROP_UPPER | CH_PROP_VOWEL,      // 042F cyrillic Ya
// 0x0430:
CH_PROP_LOWER | CH_PROP_VOWEL,      // 0430 cyrillic A
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 0431 cyrillic B
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 0432 cyrillic V
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 0433 cyrillic G
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 0434 cyrillic D
CH_PROP_LOWER | CH_PROP_VOWEL,      // 0435 cyrillic E
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 0436 cyrillic Zh
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 0437 cyrillic Z
CH_PROP_LOWER | CH_PROP_VOWEL,      // 0438 cyrillic I
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 0439 cyrillic YI
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 043A cyrillic K
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 043B cyrillic L
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 043C cyrillic M
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 043D cyrillic N
CH_PROP_LOWER | CH_PROP_VOWEL,      // 043E cyrillic O
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 043F cyrillic P
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 0440 cyrillic R
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 0441 cyrillic S
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 0442 cyrillic T
CH_PROP_LOWER | CH_PROP_VOWEL,      // 0443 cyrillic U
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 0444 cyrillic F
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 0445 cyrillic H
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 0446 cyrillic C
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 0447 cyrillic Ch
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 0448 cyrillic Sh
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 0449 cyrillic Sch
CH_PROP_LOWER | CH_PROP_ALPHA_SIGN,     // 044A cyrillic Hard sign
CH_PROP_LOWER | CH_PROP_VOWEL,      // 044B cyrillic Y
CH_PROP_LOWER | CH_PROP_ALPHA_SIGN,     // 044C cyrillic Soft sign
CH_PROP_LOWER | CH_PROP_VOWEL,      // 044D cyrillic EE
CH_PROP_LOWER | CH_PROP_VOWEL,      // 044E cyrillic Yu
CH_PROP_LOWER | CH_PROP_VOWEL,      // 044F cyrillic Ya
0,      // 0450 cyrillic
CH_PROP_LOWER | CH_PROP_VOWEL,      // 0451 cyrillic e:
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0452 cyrillic Dje
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0453 cyrillic Gje
CH_PROP_LOWER | CH_PROP_VOWEL,      // 0454 cyrillic ukr Ie
CH_PROP_LOWER | CH_PROP_CONSONANT,  // 0455 cyrillic Dze
CH_PROP_LOWER | CH_PROP_VOWEL,      // 0456 cyrillic ukr I
CH_PROP_LOWER | CH_PROP_VOWEL,      // 0457 cyrillic ukr I:
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 0458 cyrillic J
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 0459 cyrillic L'
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 045A cyrillic N'
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 045B cyrillic Th
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 045C cyrillic K'
0,      // 045D cyrillic
CH_PROP_LOWER | CH_PROP_VOWEL,      // 045E cyrillic Yu
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 045F cyrillic Dzhe
// 0x0460:
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
// 0x0490:
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 0490 cyrillic G'
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 0491 cyrillic g'
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 0492 cyrillic G-
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 0493 cyrillic g-
0,      // 0494 cyrillic
0,      // 0495 cyrillic
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 0496 cyrillic Zh,
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 0497 cyrillic zh,
0,      // 0498 cyrillic
0,      // 0499 cyrillic
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 049A cyrillic K,
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 049B cyrillic k,
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 049C cyrillic K|
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 049D cyrillic k|
0,      // 049E cyrillic
0,      // 049F cyrillic
0,      // 04A0 cyrillic
0,      // 04A1 cyrillic
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 04A2 cyrillic H,
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 04A3 cyrillic n,
0,      // 04A4 cyrillic
0,      // 04A5 cyrillic
0,      // 04A6 cyrillic
0,      // 04A7 cyrillic
0,      // 04A8 cyrillic
0,      // 04A9 cyrillic
0,      // 04AA cyrillic
0,      // 04AB cyrillic
0,      // 04AC cyrillic
0,      // 04AD cyrillic
CH_PROP_UPPER | CH_PROP_VOWEL,      // 04AE cyrillic Y
CH_PROP_LOWER | CH_PROP_VOWEL,      // 04AF cyrillic y
CH_PROP_UPPER | CH_PROP_VOWEL,      // 04B0 cyrillic Y-
CH_PROP_LOWER | CH_PROP_VOWEL,      // 04B1 cyrillic y-
CH_PROP_UPPER | CH_PROP_CONSONANT,      // 04B2 cyrillic X,
CH_PROP_LOWER | CH_PROP_CONSONANT,      // 04B3 cyrillic x,
};


void lStr_getCharProps( const lChar16 * str, int sz, lUInt16 * props )
{
    const lChar16 maxchar = sizeof(char_props) / sizeof( lUInt16 );
    for ( int i=0; i<sz; i++ ) {
        int ch = str[i];
        props[i] = (ch<maxchar) ? char_props[ch] : 0;
    }
}

lUInt16 lGetCharProps( lChar16 ch )
{
    const lChar16 maxchar = sizeof(char_props) / sizeof( lUInt16 );
    return (ch<maxchar) ? char_props[ch] : 0;
}



CRLog * CRLog::CRLOG = NULL;
void CRLog::setLogger( CRLog * logger )
{
    if ( CRLOG!=NULL ) {
        delete CRLOG;
    }
    CRLOG = logger;
}

void CRLog::setLogLevel( CRLog::log_level level )
{
    if ( !CRLOG )
        return;
    warn( "Changing log level from %d to %d", CRLOG->curr_level, level );
    CRLOG->curr_level = level;
}

CRLog::log_level CRLog::getLogLevel( CRLog::log_level level )
{
    if ( !CRLOG )
        return LL_INFO;
    return CRLOG->curr_level;
}

bool CRLog::isLogLevelEnabled( CRLog::log_level level )
{
    if ( !CRLOG )
        return false;
    return (CRLOG->curr_level >= level);
}

void CRLog::fatal( const char * msg, ... )
{
    if ( !CRLOG )
        return;
    va_list args;
    va_start( args, msg );
    CRLOG->log( "FATAL", msg, args );
    va_end(args);
}

void CRLog::error( const char * msg, ... )
{
    if ( !CRLOG || CRLOG->curr_level<LL_ERROR )
        return;
    va_list args;
    va_start( args, msg );
    CRLOG->log( "ERROR", msg, args );
    va_end(args);
}

void CRLog::warn( const char * msg, ... )
{
    if ( !CRLOG || CRLOG->curr_level<LL_WARN )
        return;
    va_list args;
    va_start( args, msg );
    CRLOG->log( "WARN", msg, args );
    va_end(args);
}

void CRLog::info( const char * msg, ... )
{
    if ( !CRLOG || CRLOG->curr_level<LL_INFO )
        return;
    va_list args;
    va_start( args, msg );
    CRLOG->log( "WARN", msg, args );
    va_end(args);
}

void CRLog::debug( const char * msg, ... )
{
    if ( !CRLOG || CRLOG->curr_level<LL_DEBUG )
        return;
    va_list args;
    va_start( args, msg );
    CRLOG->log( "DEBUG", msg, args );
    va_end(args);
}

void CRLog::trace( const char * msg, ... )
{
    if ( !CRLOG || CRLOG->curr_level<LL_TRACE )
        return;
    va_list args;
    va_start( args, msg );
    CRLOG->log( "TRACE", msg, args );
    va_end(args);
}

CRLog::~CRLog()
{
}

class CRFileLogger : public CRLog
{
protected:
    FILE * f;
    bool autoClose;
    bool autoFlush;
    virtual void log( const char * level, const char * msg, va_list args )
    {
        if ( !f )
            return;
#ifdef LINUX
        struct timeval tval;
        gettimeofday( &tval, NULL );
        int ms = tval.tv_usec;
        time_t t = tval.tv_sec;
#else
        time_t t = (time_t)time(0);
        int ms = 0;
#endif
        tm * bt = localtime(&t);
        fprintf(f, "%04d/%02d/%02d %02d:%02d:%02d.%04d %s ", bt->tm_year+1900, bt->tm_mon+1, bt->tm_mday, bt->tm_hour, bt->tm_min, bt->tm_sec, ms/100, level);
        vfprintf( f, msg, args );
        fprintf(f, "\n" );
        if ( autoFlush )
            fflush( f );
    }
public:
    CRFileLogger( FILE * file, bool _autoClose, bool _autoFlush )
    : f(file), autoClose(_autoClose), autoFlush( _autoFlush )
    {
        info( "Started logging" );
    }

    CRFileLogger( const char * fname, bool _autoFlush )
    : f(fopen( fname, "at" )), autoClose(true), autoFlush( _autoFlush )
    {
        info( "Started logging" );
    }

    virtual ~CRFileLogger() {
        if ( f && autoClose ) {
            info( "Stopped logging" );
            fclose( f );
        }
        f = NULL;
    }
};

void CRLog::setFileLogger( const char * fname, bool autoFlush )
{
    setLogger( new CRFileLogger( fname, autoFlush ) );
}

void CRLog::setStdoutLogger()
{
    setLogger( new CRFileLogger( (FILE*)stdout, false, true ) );
}

void CRLog::setStderrLogger()
{
    setLogger( new CRFileLogger( (FILE*)stderr, false, true ) );
}

