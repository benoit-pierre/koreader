#ifndef EPUBFMT_H
#define EPUBFMT_H

#include "../include/crsetup.h"
#include "../include/lvtinydom.h"

bool DetectEpubFormat( LVStreamRef stream );
bool ImportEpubDocument( LVStreamRef stream, ldomDocument * doc, LVDocViewCallback * progressCallback, CacheLoadingCallback * formatCallback, int metadataOnly = 0 );
lString32 EpubGetRootFilePath( LVContainerRef m_arc );
LVStreamRef GetEpubCoverpage(LVContainerRef arc);


#endif // EPUBFMT_H
