#ifndef CHMFMT_H
#define CHMFMT_H

#include "crsetup.h"

#if CHM_SUPPORT_ENABLED==1

#include "lvstream.h"

class CacheLoadingCallback;
class LVDocViewCallback;
class ldomDocument;

bool DetectCHMFormat( LVStreamRef stream );
bool ImportCHMDocument( LVStreamRef stream, ldomDocument * doc, LVDocViewCallback * progressCallback, CacheLoadingCallback * formatCallback );

/// opens CHM container
LVContainerRef LVOpenCHMContainer( LVStreamRef stream );

#endif

#endif // CHMFMT_H
