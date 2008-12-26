#include <cstdlib>
#include "dictdlg.h"
#include "lvstring.h"
#include "lvref.h"
//#include "selector.h"
#include "crgui.h"
#include "crtrace.h"



#include "lvarray.h"
#include "lvstring.h"
#include "lvtinydom.h"
#include "lvdocview.h"
#include "crtrace.h"
#include <stdexcept>
#include "mainwnd.h"
#include "t9encoding.h"


// to exclude short words from search (requested by LVD)
#define DICT_MIN_WORD_LENGTH 3


#ifdef _WIN32
//#define USE_LIBDICTD
template <typename T>
class Dictionary {
    typedef T string_type;
    T config_;
//    static Dictionary<T> * self_;
public:
    Dictionary(const T& config,const T& progname) : config_(config) {
#ifdef USE_LIBDICTD
        libdict_init(progname.c_str(), config_.c_str());
#endif
    }
    virtual ~Dictionary() { 
#ifdef USE_LIBDICTD
        libdict_uninit();
#endif
    }

    T translate(const T& word) {
#ifdef USE_LIBDICTD
        char * ptr;
        ptr = libdict_define(word.c_str(),"*");
        if(!ptr)
            return T();
        T tmp(ptr);
        libdict_free(ptr);
        return tmp;
#else
		return T("Temporary stub for dictionary.\nFake article for ") + word;

#endif
    }
};

#else
#include "libdictd.h"
#endif


#ifdef CR_USE_JINKE
#define DICTD_CONF "/root/crengine/dict/dictd.conf"
#else
#define DICTD_CONF "/etc/dictd/dictd.conf"
#endif


class WordWithRanges
{
    lString16 word;
    lString16 wordLower;
    lString8  encoded;
    LVArray<ldomWord> ranges;
public:
    const lString16 & getWord() { return word; }
    const lString8 & getEncoded() { return encoded; }
    LVArray<ldomWord> & getRanges() { return ranges; }
    bool matchEncoded( const lString8 & prefix )
    {
        if ( prefix.empty() )
            return false;
        return encoded.startsWith( prefix );
    }
    bool matchWord( const lString16 & prefix )
    {
        if ( prefix.empty() )
            return false;
        return word.startsWithNoCase( prefix );
    }
    bool equals( const lString16 & w )
    {
        lString16 w1( w );
        w1.lowercase();
        return w1 == wordLower;
    }
    void add( const ldomWord & range )
    {
        ranges.add( range );
    }
    WordWithRanges( const lString16 & w, const lString8 & enc, const ldomWord & range )
    : word( w ), encoded( enc )
    {
        wordLower = w;
        wordLower.lowercase();
        ranges.add( range );
    }
};

class wordlist {
    LVDocView& docview_;
    LVPtrVector<WordWithRanges> _words;
public:

    wordlist(LVDocView& docview, const TEncoding& encoding) :
        docview_(docview) 
    {
        //ldomDocument * doc = docview.getDocument();
        int pageIndex = -1; //docview.getCurPage();
        LVRef<ldomXRange> range = docview.getPageDocumentRange( pageIndex );
        crtrace trace;
        if( !range.isNull() ) {
            LVArray<ldomWord> words;
            range->getRangeWords(words);
            for ( int i=0; i<words.length(); i++ ) {
                lString16 w = words[i].getText();
                lString8 encoded = encoding.encode_string( w );
                if ( w.length() < DICT_MIN_WORD_LENGTH )
                    continue;
                /*
                trace << "string " << w <<
                    " encoded as " << encoded << "\n";
                */
                int index = -1;
                for ( int j=0; j<_words.length(); j++ )
                    if ( _words[j]->equals(w) ) {
                        index = j;
                        break;
                    }
                if ( index>=0 )
                    _words[index]->add( words[i] );  // add range to existing word
                else
                    _words.add( new WordWithRanges( w, encoded, words[i] ) ); // add new word
            }
        }
    }

    void match( const lString8& prefix, LVArray<WordWithRanges *> & result )
    {
        crtrace dumpstr;
        dumpstr << "match with " << prefix;
        for( int i=0; i<_words.length(); i++ ) {
            if( _words[i]->matchEncoded( prefix ) ) {
                result.add( _words[i] );
                //dumpstr << " " << i << " " << encoded_words_[i];
            };
        };
    }

    void highlight( WordWithRanges * p )
    {
        crtrace trace;
        trace << "Select word " << p->getWord() << " (" << p->getRanges().length() << " occurences) : ";
        for ( int i=0; i<p->getRanges().length(); i++ ) {
            trace << "[" << p->getRanges()[i].getStart() << "," << p->getRanges()[i].getEnd() << "] ";
        }
        docview_.selectWords( p->getRanges() );
    }
};

void
init_keytable(LVArray<lString16>& keytable){
#define DEFKEY(x)  keytable.add(lString16(x))
DEFKEY(L" "); // 0 and 9 are STUBs
DEFKEY(L"abc");
DEFKEY(L"def");
DEFKEY(L"ghi");
DEFKEY(L"jkl");
DEFKEY(L"mno");
DEFKEY(L"pqrs");
DEFKEY(L"tuv");
DEFKEY(L"wxyz");
DEFKEY(L".,"); // 9 STUB
#undef DEFKEY
};

class selector {
    wordlist words_;
    int current_;
    int level_;
    LVArray<WordWithRanges *> candidates_;
    LVArray<lString16> encoded_;
    lString8 prefix_;
//    LVArray<lString16> keytable_;
    int repeat_;
    int last_;
public:
	lString8 getPrefix() { return prefix_; }
    selector(LVDocView& docview, const TEncoding& encoding) : 
        words_(docview, encoding), 
        current_(0), 
        level_(0),
        candidates_(),
        prefix_(),
        repeat_(0)
        {
//            init_keytable(keytable_);
            update_candidates();
        };

    void moveBy( int delta )
    {
        current_ += delta;
        if ( current_ < 0 )
            current_ = candidates_.length()-1;
        if ( current_ >= candidates_.length() )
            current_ = 0;
        if ( current_ < 0 )
            current_ = 0;
        if ( candidates_.length() )
            words_.highlight(candidates_[current_]);
    }

    void up() { moveBy( -1 ); }

    void down() { moveBy( 1 ); }

    bool update_candidates()
    {
        CRLog::info("update_candidates() enter\n");
        LVArray<WordWithRanges *> new_candidates;
        words_.match( prefix_, new_candidates );
        CRLog::info("update_candidates() mid\n");
        current_=0;
        CRLog::info("update_candidates() mid2\n");
        if( new_candidates.length() == 0 ) {
            CRLog::info("nothig to highlight");
            return false;
        };
        candidates_ = new_candidates;
        words_.highlight( candidates_[current_] );
        CRLog::info("update_candidates() leave\n");
        return true;
    }

    bool push_button(int key) {
        crtrace trace("selector::push(): ");
        lString8 old_prefix = prefix_;
        prefix_.append(1,static_cast<char>(key));
        if(update_candidates()){
            level_++;
            return true;
        }
        prefix_ = old_prefix;
        return false;
    }


    bool pop()
    {
        if (level_==0) {
            return true;
        }
        prefix_ = prefix_.substr(0,prefix_.length()-1);
        level_--;
        update_candidates();
        return false;
    }

    const lString16 & get()
    {
        return candidates_[current_]->getWord();
    }
};



class DictWindow;

class Article : public  CRDocViewWindow {
    LVStreamRef stream_;
    DictWindow * parent_;
public:
    Article(CRGUIWindowManager * wm, lString16 title, lString8 text, DictWindow * parent) :
        CRDocViewWindow(wm),
        parent_(parent) {
            if ( text.empty() )
                text = lString8(L"Test article.\nA little text only.\n");
            stream_ = LVCreateStringStream( text );
            _skin = _wm->getSkin()->getWindowSkin(L"#dialog");
			_title = title;
			lvRect rc = _wm->getScreen()->getRect();
			int dx = rc.width() / 10;
			int dy = rc.height() / 10;
			rc.left += dx;
			rc.top += dy;
			rc.right -= dx;
			rc.bottom -= dy;
			_fullscreen = false;
			setRect( rc );
            getDocView()->setBackgroundColor(0xFFFFFF);
            getDocView()->setTextColor(0x000000);
            getDocView()->setFontSize( 20 );
            getDocView()->setShowCover( false );
            getDocView()->setPageHeaderInfo( 0 ); // hide title bar
            getDocView()->setPageMargins( lvRect(8,8,8,8) );
            getDocView()->LoadDocument(stream_);

        }

    virtual bool onCommand( int command, int params = 0 );
};

typedef Dictionary<lString8> dict_type;
LVRef<dict_type> dict_;

inline lUInt16 minBits( lUInt16 n1, lUInt16 n2, lUInt16 mask )
{
	if ( n1 == n2 )
		return n1 & mask;
	if ( (n1 & mask) < (n2 & mask) )
		return n1 & mask;
	else
		return n2 & mask;
}

inline lUInt16 maxBits( lUInt16 n1, lUInt16 n2, lUInt16 mask )
{
	if ( n1 == n2 )
		return n1 & mask;
	if ( (n1 & mask) > (n2 & mask) )
		return n1 & mask;
	else
		return n2 & mask;
}

/// window to show on top of DocView window, shifting/stretching DOCView content to fit
class BackgroundFitWindow : public CRGUIWindowBase
{
protected:
    V3DocViewWin * mainwin_;
    LVDocView& docview_;
    virtual void draw()
    {
        mainwin_->flush();
        lvRect fullRect = _wm->getScreen()->getRect();
        LVDrawBuf * buf = _wm->getScreen()->getCanvas().get();
#if 0
		buf->FillRect(0, 0, 600, 60, 0xFFFFFF );
		for ( int k=0; k<60; k++ ) {
			buf->FillRect(k*10, k, k*10+10, k+1, 0 );
			buf->FillRect(k*10+100, k, k*10+10+100, k+1, 0 );
		}
#endif
        int src_y0 = fullRect.top;
        int src_y1 = fullRect.bottom;
        // TODO: support top position of window too
        int dst_y0 = fullRect.top;
        int dst_y1 = _rect.top;
        int linesz = buf->GetBitsPerPixel() * buf->GetWidth() / 8;
		int lastline = -3;
		//int delta = (src_y1 - src_y0) - (dst_y1 - dst_y0);
        for ( int y = dst_y0; y<dst_y1; y++ ) {
            int srcy = ((src_y1 - src_y0) * y) / (dst_y1 - dst_y0) + 1;
			lUInt8 * src = buf->GetScanLine( srcy );
			lUInt8 * dst = buf->GetScanLine( y );
			memcpy( dst, src, linesz );
			if ( srcy == lastline+2 ) {
				if ( buf->GetBitsPerPixel()==2 ) {
					src = buf->GetScanLine( srcy-1 );
					dst = buf->GetScanLine( y-1 );
					for ( int i=0; i<linesz; i++ ) {
						lUInt16 n1 = src[i];
						lUInt16 n2 = dst[i];
						if ( n1 != n2 )
							n1 = n1+ 0;
						if ( true ) {
							// min
#if GRAY_INVERSE==1
							n1 = maxBits(n1,n2,0x03) | maxBits(n1,n2,0x0C) | maxBits(n1,n2,0x30) | maxBits(n1,n2,0xC0);
#else
							n1 = minBits(n1,n2,0x03) | minBits(n1,n2,0x0C) | minBits(n1,n2,0x30) | minBits(n1,n2,0xC0);
#endif
						} else {
							// blend
							n1 = ( (n1 & 0x33) << 8) | ((n1 & 0xCC)>>2);
							n2 = ( (n2 & 0x33) << 8) | ((n2 & 0xCC)>>2);
							n1 = (n1 + n2) >> 1;
							n1 = ((n1 & 0x3300)>>8) | ((n1 & 0x33)<<2);
						}
						dst[i] = (lUInt8)n1;
					}
				}
				// TODO: support other color formats
			}
			lastline = srcy;
        }
    }
public:
    BackgroundFitWindow(  CRGUIWindowManager * wm, V3DocViewWin * mainwin )
    : CRGUIWindowBase(wm),
        mainwin_(mainwin),
        docview_(*mainwin->getDocView())
    {
        _fullscreen = true;
    }
};

class DictWindow : public BackgroundFitWindow
{
    selector selector_;
    lString8 progname_;
    lString8 dict_conf_;
    const TEncoding& encoding_;
protected:
    virtual void draw()
    {
        BackgroundFitWindow::draw();
        CRRectSkinRef skin = _wm->getSkin()->getWindowSkin( L"#dialog" )->getClientSkin();
        LVDrawBuf * buf = _wm->getScreen()->getCanvas().get();
        skin->draw( *buf, _rect );
        lString16 prompt = Utf8ToUnicode(selector_.getPrefix());
        prompt << L"_";
        buf->FillRect( _rect, 0xAAAAAA );
        lvRect keyRect = _rect;
        lvRect borders = skin->getBorderWidths();
        LVFontRef font = fontMan->GetFont( 20, 600, false, css_ff_sans_serif, lString8("Arial")); //skin->getFont();
        int margin = 4;
        for ( int i=0; i<encoding_.length(); i++ ) {
            lString16 txtN = lString16::itoa(i);
            lString16 txt = encoding_[i];
            if ( txt.empty() )
                continue;
            // label 0..9
            int wN = font->getTextWidth( txtN.c_str(), txtN.length() );
            keyRect.right = keyRect.left + wN + margin + margin; //borders.left + borders.right;
            ((CRSkinnedItem*)skin.get())->drawText( *_wm->getScreen()->getCanvas(), keyRect, txtN, font, 0x000000, 0xAAAAAA, SKIN_HALIGN_CENTER|SKIN_VALIGN_CENTER  );
            keyRect.left = keyRect.right;
            // chars (abc)
            int w = font->getTextWidth( txt.c_str(), txt.length() );
            keyRect.right = keyRect.left + w + margin + margin; //borders.left + borders.right;
            skin->draw( *_wm->getScreen()->getCanvas(), keyRect );
            //skin->drawText( *_wm->getScreen()->getCanvas(), keyRect, txt );
            ((CRSkinnedItem*)skin.get())->drawText( *_wm->getScreen()->getCanvas(), keyRect, txt, font, 0x000000, 0xFFFFFF, SKIN_HALIGN_CENTER|SKIN_VALIGN_CENTER  );
            keyRect.left = keyRect.right + margin; //borders.left;
        }
        keyRect.right = _rect.right;
        if ( !keyRect.isEmpty() ) {
            skin->draw( *_wm->getScreen()->getCanvas(), keyRect );
            skin->drawText( *_wm->getScreen()->getCanvas(), keyRect, prompt );
        }
    }

public:

	DictWindow( CRGUIWindowManager * wm, V3DocViewWin * mainwin, const TEncoding& encoding ) :
		BackgroundFitWindow(wm, mainwin),
		selector_(*mainwin->getDocView(), encoding),
		progname_("lbook-fb2-plugin"),
		dict_conf_(DICTD_CONF),
        encoding_(encoding) {

		this->setAccelerators( mainwin->getDialogAccelerators() );

		if (dict_.isNull()){
			dict_ = LVRef<dict_type>(new dict_type(dict_conf_,progname_));
		};
		_rect = _wm->getScreen()->getRect();
		//_rect.bottom = _rect.top;
		_rect.top = _rect.bottom - 40;
	}

	bool onCommand( int command, int params )
	{
		switch ( command ) {
			case MCMD_SELECT_0:
			case MCMD_SELECT_1:
			case MCMD_SELECT_2:
			case MCMD_SELECT_3:
			case MCMD_SELECT_4:
			case MCMD_SELECT_5:
			case MCMD_SELECT_6:
			case MCMD_SELECT_7:
			case MCMD_SELECT_8:
			case MCMD_SELECT_9:
				selector_.push_button( command - MCMD_SELECT_0 + '0' );
				break;
			case MCMD_SCROLL_FORWARD:
				selector_.down();
				break;
			case MCMD_SCROLL_BACK:
				selector_.up();
				break;
			case MCMD_OK:
				{
					lString8 translated;
					lString8 output;
					lString16 src = selector_.get();
					translated = dict_->translate( UnicodeToUtf8(src) );
					if(translated.length() == 0) {
						output = lString8("No article for this word");
					} else {
						output = lString8("<?xml version=\"1.0\" encoding=\"UTF-8\">");
						output << "<FictionBook><body><code style=\"text-align: left; text-indent: 0; font-size: 20\">";
						output << translated;
						output << "</code></body></FictionBook>";
					};
					crtrace crt("article: ");
					crt << output;
					Article * article = new Article(_wm, src, output, this);
					article->setAccelerators( mainwin_->getDialogAccelerators() );
					_wm->activateWindow( article );
				};
				break;
			case MCMD_CANCEL:
				if ( selector_.pop() ) {
					close();
					return true ;
				}
				break;
		}
		return true;
	}

    void close() {
        CRLog::info("Closing dict");
        docview_.clearSelection();
        _wm->closeWindow(this);
    };


protected:
    DictWindow(const DictWindow&); //non-copyable
};



bool Article::onCommand( int command, int params )
{
        switch ( command ) {
			case MCMD_CANCEL:
			case MCMD_OK:
                parent_->close();
                _wm->closeWindow(this);
                return true;
			case MCMD_SCROLL_FORWARD:
                return CRDocViewWindow::onCommand( DCMD_PAGEDOWN, 1 );
			case MCMD_SCROLL_BACK:
                return CRDocViewWindow::onCommand( DCMD_PAGEUP, 1 );
                //_wm->postCommand(command, 0);
                //_wm->closeWindow(this);
                return true;
            default:
                return true; //CRDocViewWindow::onKeyPressed(key,flags);
        }
}

void activate_dict( CRGUIWindowManager *wm, V3DocViewWin * mainwin, const TEncoding& encoding )
{
    CRLog::info("Entering dict mode\n");
    wm->activateWindow(new DictWindow(wm, mainwin, encoding));
}
