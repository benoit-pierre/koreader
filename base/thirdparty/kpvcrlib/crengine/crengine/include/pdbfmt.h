#ifndef PDBFMT_H
#define PDBFMT_H

#include "crsetup.h"

#include "bookformats.h"
#include "lvstream.h"

class CacheLoadingCallback;
class LVDocViewCallback;
class ldomDocument;

// creates PDB decoder stream for stream
//LVStreamRef LVOpenPDBStream( LVStreamRef srcstream, int &format );

bool DetectPDBFormat( LVStreamRef stream, doc_format_t & contentFormat );
bool ImportPDBDocument( LVStreamRef & stream, ldomDocument * doc, LVDocViewCallback * progressCallback, CacheLoadingCallback * formatCallback, doc_format_t & contentFormat );
LVStreamRef GetPDBCoverpage(LVStreamRef stream);


#endif // PDBFMT_H
