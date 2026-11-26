#ifndef ODTFMT_H
#define ODTFMT_H

#include "crsetup.h"

#include "lvstream.h"

class CacheLoadingCallback;
class LVDocViewCallback;
class ldomDocument;

bool DetectOpenDocumentFormat( LVStreamRef stream );
bool ImportOpenDocument( LVStreamRef stream, ldomDocument * doc, LVDocViewCallback * progressCallback, CacheLoadingCallback * formatCallback );

#endif // DOCXFMT_H
