/*******************************************************

   CoolReader Engine

   lvmemman.cpp:  memory manager implementation

   (c) Vadim Lopatin, 2000-2006
   This source code is distributed under the terms of
   GNU General Public License
   See LICENSE file for details

*******************************************************/

#include "crsetup.h"
#include "lvmemman.h"

#include "lvstream.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static char file_to_remove_on_crash[2048] = "";

void crSetFileToRemoveOnFatalError(const char * filename) {
	strcpy(file_to_remove_on_crash, filename == NULL ? "" : filename);
}

void crFatalError( int code, const char * errorText )
{
	if (file_to_remove_on_crash[0])
		LVDeleteFile(Utf8ToUnicode(lString8(file_to_remove_on_crash)));
    fprintf( stderr, "FATAL ERROR #%d: %s\n", code, errorText );
    abort();
}

ref_count_rec_t ref_count_rec_t::null_ref(NULL);
ref_count_rec_t ref_count_rec_t::protected_null_ref(NULL);


#if (LDOM_USE_OWN_MEM_MAN==1)
ldomMemManStorage * pmsREF = NULL;

ldomMemManStorage * block_storages[LOCAL_STORAGE_COUNT] =
{
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
};

inline int blockSizeToStorageIndex( size_t n )
{
    return (n + ((1<<BLOCK_SIZE_GRANULARITY)-1))>>BLOCK_SIZE_GRANULARITY;
}

void * ldomAlloc( size_t n )
{
    n = blockSizeToStorageIndex( n );
    if (n<LOCAL_STORAGE_COUNT)
    {
        if ( block_storages[n] == NULL )
        {
            block_storages[n] = new ldomMemManStorage((n+1)*BLOCK_SIZE_GRANULARITY);
        }
        return block_storages[n]->alloc();
    }
    else
    {
        return malloc( n );
    }
}

void   ldomFree( void * p, size_t n )
{
    n = blockSizeToStorageIndex( n );
    if (n<LOCAL_STORAGE_COUNT)
    {
        if ( block_storages[n] == NULL )
        {
            crFatalError();
        }
        block_storages[n]->free( (ldomMemBlock *)p );
    }
    else
    {
        free( p );
    }
}
#endif
