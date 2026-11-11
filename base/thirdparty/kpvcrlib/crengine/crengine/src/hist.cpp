/** \file hist.cpp
    \brief file history and bookmarks container

    CoolReader Engine

    (c) Vadim Lopatin, 2000-2007
    This source code is distributed under the terms of
    GNU General Public License
    See LICENSE file for details
*/

#include "crsetup.h"
#include "hist.h"

#include "lvstream.h"
#include "lvtinydom.h"
#include "lvxml.h"

#include <stdio.h>
#include <time.h>

void CRFileHist::clear()
{
    _records.clear();
}

static void splitFName( lString32 pathname, lString32 & path, lString32 & name )
{
    //
    int spos = -1;
    for ( spos=pathname.length()-1; spos>=0; spos-- ) {
        lChar32 ch = pathname[spos];
        if ( ch=='\\' || ch=='/' ) {
            break;
        }
    }
    if ( spos>=0 ) {
        path = pathname.substr( 0, spos+1 );
        name = pathname.substr( spos+1, pathname.length()-spos-1 );
    } else {
        path.clear();
        name = pathname;
    }
}

int CRFileHist::findEntry( const lString32 & fname, const lString32 & fpath, lvsize_t sz )
{
    CR_UNUSED(fpath);
    for ( int i=0; i<_records.length(); i++ ) {
        CRFileHistRecord * rec = _records[i];
        if ( rec->getFileName().compare(fname) )
            continue;
        if ( rec->getFileSize()!=sz ) {
            CRLog::warn("CRFileHist::findEntry() Filename matched %s but sizes are different %d!=%d", LCSTR(fname), sz, rec->getFileSize() );
            continue;
        }
        return i;
    }
    return -1;
}

void CRFileHist::makeTop( int index )
{
    if ( index<=0 || index>=_records.length() )
        return;
    CRFileHistRecord * rec = _records[index];
    for ( int i=index; i>0; i-- )
        _records[i] = _records[i-1];
    _records[0] = rec;
}

void CRFileHistRecord::setLastPos( CRBookmark * bmk )
{
    _lastpos = *bmk;
}

lString32 CRBookmark::getChapterName( ldomXPointer ptr )
{
    //CRLog::trace("CRBookmark::getChapterName()");
	lString32 chapter;
	int lastLevel = -1;
	bool foundAnySection = false;
    lUInt16 section_id = ptr.getNode()->getDocument()->getElementNameIndex( U"section" );
	if ( !ptr.isNull() )
	{
		ldomXPointerEx p( ptr );
		p.nextText();
		while ( !p && !p.isNull() ) {
			if ( !p.prevElement() )
				break;
            bool foundSection = p.findElementInPath( section_id ) > 0;
            //(p.toString().pos("section") >=0 );
            foundAnySection = foundAnySection || foundSection;
            if ( !foundSection && foundAnySection )
                continue;
			lString32 nname = p.getNode()->getNodeName();
            if ( !nname.compare("title") || !nname.compare("h1") || !nname.compare("h2")  || !nname.compare("h3") ) {
				if ( lastLevel!=-1 && p.getLevel()>=lastLevel )
					continue;
				lastLevel = p.getLevel();
				if ( !chapter.empty() )
                    chapter = " / " + chapter;
				chapter = p.getText(' ') + chapter;
				if ( !p.parent() )
					break;
			}
		}
	}
	return chapter;
}

CRFileHistRecord * CRFileHist::savePosition( lString32 fpathname, size_t sz,
                            const lString32 & title,
                            const lString32 & author,
                            const lString32 & series,
                            ldomXPointer ptr )
{
    //CRLog::trace("CRFileHist::savePosition");
    lString32 name;
	lString32 path;
    splitFName( fpathname, path, name );
    CRBookmark bmk( ptr );
    //CRLog::trace("Bookmark created");
    int index = findEntry( name, path, (lvsize_t)sz );
    //CRLog::trace("findEntry exited");
    if ( index>=0 ) {
        makeTop( index );
        _records[0]->setLastPos( &bmk );
        _records[0]->setLastTime( (time_t)time(0) );
        return _records[0];
    }
    CRFileHistRecord * rec = new CRFileHistRecord();
    rec->setTitle( title );
    rec->setAuthor( author );
    rec->setSeries( series );
    rec->setFileName( name );
    rec->setFilePath( path );
    rec->setFileSize( (lvsize_t)sz );
    rec->setLastPos( &bmk );
    rec->setLastTime( (time_t)time(0) );

    _records.insert( 0, rec );
    //CRLog::trace("CRFileHist::savePosition - exit");
    return rec;
}

CRBookmark::CRBookmark (ldomXPointer ptr )
: _startpos(lString32::empty_str)
, _endpos(lString32::empty_str)
, _percent(0)
, _type(0)
, _shortcut(0)
, _postext(lString32::empty_str)
, _titletext(lString32::empty_str)
, _commenttext(lString32::empty_str)
, _timestamp(time_t(0))
, _page(0)
{
    //
    if ( ptr.isNull() )
        return;

    //CRLog::trace("CRBookmark::CRBookmark() started");
    lString32 path;

    //CRLog::trace("CRBookmark::CRBookmark() calling ptr.toPoint");
    lvPoint pt = ptr.toPoint();
    //CRLog::trace("CRBookmark::CRBookmark() calculating percent");
    ldomDocument * doc = ptr.getNode()->getDocument();
    int h = doc->getFullHeight();
    if ( pt.y > 0 && h > 0 ) {
        if ( pt.y < h ) {
            _percent = (int)((lInt64)pt.y * 10000 / h);
        } else {
            _percent = 10000;
        }
    }
    //CRLog::trace("CRBookmark::CRBookmark() calling getChaptername");
	setTitleText( CRBookmark::getChapterName( ptr ) );
    _startpos = ptr.toString();
    _timestamp = (time_t)time(0);
    lvPoint endpt = pt;
    endpt.y += 100;
    //CRLog::trace("CRBookmark::CRBookmark() creating xpointer for endp");
    ldomXPointer endptr = doc->createXPointer( endpt );
    //CRLog::trace("CRBookmark::CRBookmark() finished");
}


lString32 CRFileHistRecord::getLastTimeString( bool longFormat )
{

    time_t t = getLastTime();
    tm * bt = localtime(&t);
    char str[33];
    if ( !longFormat )
        snprintf(str, sizeof (str), "%02d.%02d.%04d", bt->tm_mday, 1+bt->tm_mon, 1900+bt->tm_year );
    else
        snprintf(str, sizeof (str), "%02d.%02d.%04d %02d:%02d", bt->tm_mday, 1+bt->tm_mon, 1900+bt->tm_year, bt->tm_hour, bt->tm_min);
    return Utf8ToUnicode( lString8( str ) );
}
