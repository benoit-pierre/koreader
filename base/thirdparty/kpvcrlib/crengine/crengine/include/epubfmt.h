#ifndef EPUBFMT_H
#define EPUBFMT_H

#include "crsetup.h"

#include "dtddef.h"
#include "lvstream.h"
#include "lvstring.h"

#include <stddef.h>

class CacheLoadingCallback;
class LVDocViewCallback;
class ldomDocument;

bool DetectEpubFormat( LVStreamRef stream );
bool ImportEpubDocument( LVStreamRef stream, ldomDocument * doc, LVDocViewCallback * progressCallback,
    CacheLoadingCallback * formatCallback, int metadataOnly = 0,
    const elem_def_t * node_scheme=NULL, const attr_def_t * attr_scheme=NULL, const ns_def_t * ns_scheme=NULL);
lString32 EpubGetRootFilePath( LVContainerRef m_arc );
LVStreamRef GetEpubCoverpage(LVContainerRef arc);


#endif // EPUBFMT_H
