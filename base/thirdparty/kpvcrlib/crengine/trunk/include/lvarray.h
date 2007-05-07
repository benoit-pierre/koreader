/** \file lvarray.h
    \brief value array template

    Implements array of values.

    CoolReader Engine

    (c) Vadim Lopatin, 2000-2006
    This source code is distributed under the terms of
    GNU General Public License.

    See LICENSE file for details.

*/

#ifndef __LVARRAY_H_INCLUDED__
#define __LVARRAY_H_INCLUDED__

#include <stdlib.h>
#include <assert.h>

/** \brief template which implements vector of pointer

    Automatically deletes objects when vector items are destroyed.
*/
template <typename T >
class LVArray
{
    T * _array;
    int _size;
    int _count;
public:
    /// default constructor
    LVArray() : _array(NULL), _size(0), _count(0) {}
    /// creates array of given size
    LVArray( int len, T value )
    {
        _size = _count = len;
        _array = (T*) malloc( sizeof(T)*_size );
        for (int i=0; i<_count; i++)
            _array[i] = value;
    }
    LVArray( const LVArray & v )
    {
        _size = _count = v._count;
        if ( _size ) {
            _array = (T*) malloc( sizeof(T)*_size );
            for (int i=0; i<_count; i++)
                _array[i] = v._array[i];
        } else {
            _array = NULL;
        }
    }
    LVArray( const T * ptr, int len )
    {
        _size = _count = len;
        if ( _size ) {
            _array = (T*) malloc( sizeof(T)*_size );
            for (int i=0; i<_count; i++)
                _array[i] = ptr[i];
        } else {
            _array = NULL;
        }
    }
    LVArray & operator = ( const LVArray & v )
    {
        clear();
        _size = _count = v._count;
        if ( _size ) {
            _array = (T*) malloc( sizeof(T)*_size );
            for (int i=0; i<_count; i++)
                _array[i] = v._array[i];
        } else {
            _array = NULL;
        }
        return *this;
    }
    /// retrieves item from specified position
    T operator [] ( int pos ) const { return _array[pos]; }
    /// retrieves item reference from specified position
    T & operator [] ( int pos ) { return _array[pos]; }
    /// ensures that size of vector is not less than specified value
    void reserve( int size )
    {
        if ( size > _size )
        {
            _array = (T*)realloc( _array, size * sizeof( T ));
            for (int i=_size; i<size; i++)
                _array[i] = T();
            _size = size;
        }
    }
    /// sets item by index (extends vector if necessary)
    void set( int index, T item )
    {
        reserve( index );
        _array[index] = item;
    }
    /// returns size of buffer
    int size() { return _size; }
    /// returns number of items in vector
    int length() { return _count; }
    /// returns true if there are no items in vector
    bool empty() { return _count==0; }
    /// clears all items
    void clear()
    {
        if (_array)
        {
            free( _array );
            _array = NULL;
        }
        _size = 0;
        _count = 0;
    }
    /// copies range to beginning of array
    void trim( int pos, int count, int reserved )
    {
        if ( pos<0 || count<=0 || pos+count > _count )
            throw;
        int i;
        int new_sz = count;
        if (new_sz < reserved)
            new_sz = reserved;
        T* new_array = (T*)malloc( new_sz * sizeof( T ) );
        if (_array)
        {
            for ( i=0; i<count; i++ )
            {
                new_array[i] = _array[ pos + i ];
            }
            free( _array );
        }
        _array = new_array;
        _count = count;
        _size = new_sz;
    }
    /// removes several items from vector
    void erase( int pos, int count )
    {
        if ( pos<0 || count<=0 || pos+count > _count )
            throw;
        int i;
        for (i=pos+count; i<_count; i++)
        {
            _array[i-count] = _array[i];
        }
        _count -= count;
    }

    /// adds new item to end of vector
    void add( T item )
    { 
        insert( -1, item );
    }

    /// adds new item to end of vector
    void append( const T * items, int count )
    {
        reserve( _count + count );
        for (int i=0; i<count; i++)
            _array[ _count+i ] = items[i];
        _count += count;
    }
    
    T * addSpace( int count )
    {
        reserve( _count + count );
        T * ptr = _array + _count;
        _count += count;
        return ptr;
    }
    
    /// inserts new item to specified position
    void insert( int pos, T item )
    {
        if (pos<0 || pos>_count)
            pos = _count;
        if ( _count >= _size )
            reserve( _count * 3 / 2  + 8 );
        for (int i=_count; i>pos; --i)
            _array[i] = _array[i-1];
        _array[pos] = item;
        _count++;
    }
    /// returns array pointer
    T * ptr() { return _array; }
    /// destructor
    ~LVArray() { clear(); }
};

template <typename T >
class LVArrayQueue
{
private:
    LVArray<T> m_buf;
    int inpos;
public:
    LVArrayQueue()
        : inpos(0)
    {
    }
    
    /// returns pointer to reserved space of specified size
    T * prepareWrite( int size )
    {
        if ( m_buf.length() + size > m_buf.size() )
        {
            if ( inpos > (m_buf.length() + size) / 2 )
            {
                // trim
                m_buf.erase(0, inpos);
                inpos = 0;
            }
        }
        return m_buf.addSpace( size );
    }
    
    /// writes data to end of queue
    void write( const T * data, int size )
    {
        T * buf = prepareWrite( size );
        for (int i=0; i<size; i++)
            buf[i] = data[i];
    }
    
    int length()
    {
        return m_buf.length() - inpos;
    }
    
    /// returns pointer to data to be read
    T * peek() { return m_buf.ptr() + inpos; }
    
    /// reads data from start of queue
    void read( T * data, int size )
    {
        if ( size > length() )
            size = length();
        for ( int i=0; i<size; i++ )
            data[i] = m_buf[inpos + i];
        inpos += size;
    }
    
    /// skips data from start of queue
    void skip( int size )
    {
        if ( size > length() )
            size = length();
        inpos += size;
    }
};



#endif
