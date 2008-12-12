/***************************************************************************
 *   Copyright (C) 2007 by Vadim Lopatin   *
 *   vadim.lopatin@coolreader.org   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef CR_GUI_INCLUDED
#define CR_GUI_INCLUDED

#include "lvtypes.h"
#include "lvptrvec.h"
#include "lvdrawbuf.h"
#include "lvdocview.h"

#ifdef CR_WX_SUPPORT
#include <wx/wx.h>
#endif

class CRGUIWindowManager;


/// Accelerator table entry: to convert keypress to command
class CRGUIAccelerator
{
    public:
        int keyCode;
        int keyFlags;
        int commandId;
        int commandParam;
};

/// accelerator table
class CRGUIAcceleratorTable
{
protected:
    LVPtrVector<CRGUIAccelerator> _items;
    int indexOf( int keyCode, int keyFlags )
    {
        for ( int i=0; i<_items.length(); i++ )
            if ( _items[i]->keyCode==keyCode && _items[i]->keyFlags==keyFlags )
                return i;
        return -1;
    }
public:
    /// remove accelerator from table
    bool remove( int keyCode, int keyFlags )
    {
        int index = indexOf( keyCode, keyFlags );
        if ( index >= 0 ) {
            _items.erase( index, 1 );
            return true;
        }
        return false;
    }
    /// add accelerator to table or change existing
    bool add( int keyCode, int keyFlags, int commandId, int commandParam )
    {
        int index = indexOf( keyCode, keyFlags );
        if ( index >= 0 ) {
            // just update
            CRGUIAccelerator * item = _items[index];
            item->commandId = commandId;
            item->commandParam = commandParam;
            return false;
        }
        CRGUIAccelerator * item = new CRGUIAccelerator();
        item->keyCode = keyCode;
        item->keyFlags = keyFlags;
        item->commandId = commandId;
        item->commandParam = commandParam;
        _items.add(item);
        return true;
    }
    /// translate keycode to command, returns true if translated
    bool translate( int keyCode, int keyFlags, int & commandId, int & commandParam )
    {
        int index = indexOf( keyCode, keyFlags );
        if ( index<0 )
            return false;
        commandId = _items[index]->commandId;
        commandParam = _items[index]->commandParam;
        return true;
    }
    /// empty table constructor
    CRGUIAcceleratorTable() { }
    /// constructor from int array: 4 ints per entry (keyCode, keyFlags, commandId, commandParam), keyCode==0 indicates end of list 
    CRGUIAcceleratorTable( const int * tableQuadsArray )
    {
        while( *tableQuadsArray ) {
            CRGUIAccelerator * item = new CRGUIAccelerator();
            item->keyCode = *tableQuadsArray++;
            item->keyFlags = *tableQuadsArray++;
            item->commandId = *tableQuadsArray++;
            item->commandParam = *tableQuadsArray++;
            _items.add(item);
        }
    }
};

/// i18n support interface
class CRGUIStringTranslator
{
public:
    /// translate string by key, return default value if not found
    virtual lString16 translateString( const char * key, const char * defValue )
    {
        return Utf8ToUnicode( lString8(defValue) );
    }
};

/// accelerator table reference
typedef LVRef<CRGUIAcceleratorTable> CRGUIAcceleratorTableRef;


/// Screen object - provides canvas and interface to device screen
class CRGUIScreen
{
    public:
        /// creates compatible canvas of specified size
        virtual LVDrawBuf * createCanvas( int dx, int dy ) = 0;
        /// sets new screen size, returns true if size is changed
        virtual bool setSize( int dx, int dy ) { return false; }
        /// returns screen width
        virtual int getWidth() = 0;
        /// returns screen height
        virtual int getHeight() = 0;
        /// returns screen dimension
        virtual lvRect getRect() { return lvRect(0, 0, getWidth(), getHeight() ); }
        /// return pointer to screen canvas
        virtual LVRef<LVDrawBuf> getCanvas() = 0;
        /// draw image on screen canvas
        virtual void draw( LVDrawBuf * img, int x = 0, int y = 0) = 0;
        /// transfers contents of buffer to device, if full==true, redraws whole screen, otherwise only changed area
        virtual void flush( bool full ) { }
        /// invalidates rectangle: add it to bounding box of next partial update
        virtual void invalidateRect( const lvRect & rc ) { }
        virtual ~CRGUIScreen() { }
};

/// Window interface
class CRGUIWindow
{
    public:
        /// set accelerator table for window
        virtual void setAccelerators( CRGUIAcceleratorTableRef table ) { }
        /// get window accelerator table
        virtual CRGUIAcceleratorTableRef getAccelerators() { return CRGUIAcceleratorTableRef(); }
        /// returns true if key is processed
        virtual bool onKeyPressed( int key, int flags = 0 ) { return false; }
        /// returns true if command is processed
        virtual bool onCommand( int command, int params = 0 ) { return false; }
        /// returns true if window is visible
        virtual bool isVisible() const = 0;
        /// returns true if window is fullscreen
        virtual bool isFullscreen() = 0;
        /// returns true if window is changed but now drawn
        virtual bool isDirty() = 0;
        /// shows or hides window
        virtual void setVisible( bool visible ) = 0;
        /// returns window rectangle
        virtual const lvRect & getRect() = 0;
        /// sets window rectangle
        virtual void setRect( const lvRect & rc ) = 0;
        /// draws content of window to screen
        virtual void flush() = 0;
        /// called if window gets focus
        virtual void activated() { }
        /// called if window loss focus
        virtual void covered() { }
        /// returns window manager
        virtual CRGUIWindowManager * getWindowManager() = 0;
        /// destroys window
        virtual ~CRGUIWindow() { }
};

/// Window manager
class CRGUIWindowManager : public CRGUIStringTranslator
{
    protected:
        LVPtrVector<CRGUIWindow, true> _windows;
        CRGUIScreen * _screen;
        /// if true, we should delete screen in destructor
        bool _ownScreen;
        LVRef<CRGUIStringTranslator> _i18n;
        int _postedCommand;
        int _postedCommandParam;
    public:
        /// sets another i18n translator
        virtual void setTranslator( LVRef<CRGUIStringTranslator> i18n )
        {
            _i18n = i18n;
        }
        /// translate string by key, return default value if not found
        virtual lString16 translateString( const char * key, const char * defValue )
        {
            if ( _i18n.isNull() )
                return Utf8ToUnicode( lString8(defValue) );
            return _i18n->translateString( key, defValue );
        }
        /// returns count of windows
        virtual int getWindowCount() { return _windows.length(); }
        /// sets new screen size
        virtual void setSize( int dx, int dy )
        {
            if ( _screen->setSize( dx, dy ) ) {
                lvRect fullRect = _screen->getRect();
                for ( int i=_windows.length()-1; i>=0; i-- ) {
                    lvRect rc = _windows[i]->getRect();
                    if ( _windows[i]->isFullscreen() )
                        _windows[i]->setRect( fullRect );
                    else {
                        if ( rc.right > dx ) {
                            rc.left -= rc.right - dx;
                            rc.right = dx;
                            if ( rc.left < 0 )
                                rc.left = 0;
                        }
                        if ( rc.right > dx ) {
                            rc.left -= rc.right - dx;
                            rc.right = dx;
                            if ( rc.left < 0 )
                                rc.left = 0;
                        }
                        _windows[i]->setRect( rc );
                    }
                }
                update( true );
            }
        }
        /// adds command to message queue
        virtual void postCommand( int command, int params = 0 )
        {
            _postedCommand = command;
            _postedCommandParam = params;
        }
        /// runs posted events (commands)
        virtual bool processPostedEvents()
        {
            // TODO: support posted event queue
            bool res = false;
            if ( !_postedCommand )
                return res;
            res = onCommand( _postedCommand, _postedCommandParam );
            _postedCommand = 0;
            _postedCommandParam = 0;
            return res;
        }
        /// returns true if command is processed
        virtual bool onCommand( int command, int params = 0 )
        {
            for ( int i=_windows.length()-1; i>=0; i-- ) {
                if ( _windows[i]->isVisible() && _windows[i]->onCommand( command, params ) )
                    return true;
            }
            return false;
        }
        /// returns true if key is processed
        virtual bool onKeyPressed( int key, int flags = 0 )
        {
            for ( int i=_windows.length()-1; i>=0; i-- ) {
                if ( _windows[i]->isVisible() && _windows[i]->onKeyPressed( key, flags ) )
                    return true;
            }
            return false;
        }
        /// returns top visible window
        CRGUIWindow * getTopVisibleWindow()
        {
            for ( int i=_windows.length()-1; i>=0; i-- ) {
                if ( !_windows[i]->isVisible() )
                    continue;
                return _windows[i];
            }
            return NULL;
        }
        /// shows or hides window
        void showWindow( CRGUIWindow * window, bool visible )
        {
            int index = _windows.indexOf( window );
            if ( index >= 0  ) { //&& window->isVisible()!=visible
                window->setVisible( visible );
                if ( !visible ) {
                    window->covered();
                    CRGUIWindow * wnd = getTopVisibleWindow();
                    if ( wnd )
                        activateWindow( wnd );
                } else
                    activateWindow( window );
            }
        }
        /// activates window, brings it on top; add to stack if not added
        void activateWindow( CRGUIWindow * window )
        {
            int index = _windows.indexOf( window );
            CRGUIWindow * lostFocus = getTopVisibleWindow();
            window->setVisible( true );
            if ( index < 0 ) {
                _windows.push( window );
            } else if ( index < _windows.length() - 1 ) {
                _windows.push( _windows.remove( index ) );
            }
            if ( window != lostFocus )
            {
                if ( lostFocus )
                    lostFocus->covered();
                window->activated();
            }
        }
        /// closes window, removes from stack, destroys object
        void closeWindow( CRGUIWindow * window )
        {
            int index = _windows.indexOf( window );
            if ( index >= 0 ) {
                if ( window == _windows.peek() )
                    window->covered(); // send cover before close
                _windows.remove( index );
            }
            delete window;
        }
        /// redraw one window
        void updateWindow( CRGUIWindow * window )
        {
            int index = _windows.indexOf( window );
            if ( index < 0 )
                return;
            lvRect coverBox;
            if  ( _windows.empty() )
                return;
            LVPtrVector<CRGUIWindow, false> drawList;
            for ( int i=_windows.length()-1; i>=index; i-- ) {
                if ( !_windows[i]->isVisible() )
                    continue;
                lvRect rc = _windows[i]->getRect();
                if ( coverBox.isRectInside( rc ) )
                    continue; // fully covered by top window
                if ( !rc.isEmpty() )
                    drawList.add( _windows[i] );
                if ( !rc.isRectInside( coverBox ) )
                    coverBox = rc;
            }
            while ( !drawList.empty()  ) {
                CRGUIWindow * w = drawList.pop();
                if ( w->isDirty() ) {
                    if ( w->isVisible() )
                        w->flush();
                    _screen->invalidateRect( w->getRect() );
                }
            }
        /// invalidates rectangle: add it to bounding box of next partial update
            _screen->flush( false );
        }
        /// full redraw of all windows
        void update( bool fullScreenUpdate )
        {
            lvRect coverBox;
            if  ( _windows.empty() )
                return;
            LVPtrVector<CRGUIWindow, false> drawList;
            for ( int i=_windows.length()-1; i>=0; i-- ) {
                if ( !_windows[i]->isVisible() )
                    continue;
                lvRect rc = _windows[i]->getRect();
                if ( coverBox.isRectInside( rc ) )
                    continue; // fully covered by top window
                if ( !rc.isEmpty() )
                    drawList.add( _windows[i] );
                if ( !rc.isRectInside( coverBox ) )
                    coverBox = rc;
            }
            while ( !drawList.empty()  ) {
                CRGUIWindow * w = drawList.pop();
                if ( w->isDirty() || fullScreenUpdate ) {
                    if ( w->isVisible() )
                        w->flush();
                    _screen->invalidateRect( w->getRect() );
                }
            }
            _screen->flush( fullScreenUpdate );
        }
        /// returns screen associated with window manager
        CRGUIScreen * getScreen()
        {
            return _screen;
        }
        /// runs event loop
        virtual int runEventLoop() { return 0; }
        /// constructor
        CRGUIWindowManager(CRGUIScreen * screen)
        : _screen( screen ), _ownScreen(false)
        , _postedCommand(0)
        , _postedCommandParam(0)
        {
        }
        virtual void closeAllWindows()
        {
            for ( int i=_windows.length()-1; i>=0; i-- ) {
                closeWindow(_windows[i]);
            }
        }
        /// destroy all windows on close
        virtual ~CRGUIWindowManager()
        {
            closeAllWindows();
            if ( _ownScreen )
                delete _screen;
        }
};

/// Window base implementation
class CRGUIWindowBase : public CRGUIWindow
{
    protected:
        CRGUIWindowManager * _wm;
        lvRect _rect;
        bool _visible;
        bool _fullscreen;
        bool _dirty;
        CRGUIAcceleratorTableRef _acceleratorTable;
        virtual void draw() = 0;
    public:
        /// returns true if key is processed (by default, let's translate key to command using accelerator table)
        virtual bool onKeyPressed( int key, int flags = 0 );
        /// set accelerator table for window
        virtual void setAccelerators( CRGUIAcceleratorTableRef table ) { _acceleratorTable = table; }
        /// get window accelerator table
        virtual CRGUIAcceleratorTableRef getAccelerators() { return _acceleratorTable; }
        /// returns window width
        inline int getWidth() { return getRect().width(); }
        /// returns window height
        inline int getHeight() { return getRect().height(); }
        /// sets dirty flag
        virtual void setDirty() { _dirty = true; }
        /// returns true if window is changed but now drawn
        virtual bool isDirty() { return _dirty; }
        /// shows or hides window
        virtual void setVisible( bool visible ) { _visible = visible; setDirty(); }
        virtual bool isVisible() const { return _visible; }
        virtual const lvRect & getRect() { return _rect; }
        virtual void setRect( const lvRect & rc ) { _rect = rc; setDirty(); }
        virtual void flush() { draw(); _dirty = false; }
        /// returns true if window is fullscreen
        virtual bool isFullscreen() { return _fullscreen; }
        /// set fullscreen state for window
        virtual void setFullscreen( bool fullscreen ) { _fullscreen = fullscreen; }
        virtual CRGUIWindowManager * getWindowManager() { return _wm; }
        CRGUIWindowBase( CRGUIWindowManager * wm )
        : _wm(wm), _visible(true), _fullscreen(true), _dirty(true)
        {
            // fullscreen visible by default
            _rect = _wm->getScreen()->getRect();
        }
        virtual ~CRGUIWindowBase() { }
};

/// Base Screen class implementation
class CRGUIScreenBase : public CRGUIScreen
{
    protected:
        int _width;
        int _height;
        lvRect _updateRect;
        LVRef<LVDrawBuf> _canvas;
        LVRef<LVDrawBuf> _front;
        /// override in ancessor to transfer image to device
        virtual void update( const lvRect & rc, bool full ) = 0;
    public:
        /// creates compatible canvas of specified size
        virtual LVDrawBuf * createCanvas( int dx, int dy )
        {
#if (COLOR_BACKBUFFER==1)
            LVDrawBuf * buf = new LVColorDrawBuf( dx, dy );
#else
            LVDrawBuf * buf = new LVGrayDrawBuf( dx, dy, GRAY_BACKBUFFER_BITS );
#endif
            return buf;
        }
        /// sets new screen size
        virtual bool setSize( int dx, int dy )
        {
            if ( _width!=dx || _height != dy ) {
                _width = dx;
                _height = dy;
                _canvas = LVRef<LVDrawBuf>( createCanvas( dx, dy ) );
                if ( !_front.isNull() )
                    _front = LVRef<LVDrawBuf>( createCanvas( dx, dy ) );
                return true;
            }
            return false;
        }

        /// returns screen width
        virtual int getWidth() { return _width; }
        /// returns screen height
        virtual int getHeight() { return _height; }
        /// return pointer to screen canvas
        virtual LVRef<LVDrawBuf> getCanvas() { return _canvas; }
        /// draw image on screen canvas
        virtual void draw( LVDrawBuf * img, int x = 0, int y = 0)
        {
            img->DrawTo( _canvas.get(), x, y, 0, NULL );
        }
        /// transfers contents of buffer to device, if full==true, redraws whole screen, otherwise only changed area
        virtual void flush( bool full );
        /// invalidates rectangle: add it to bounding box of next partial update
        virtual void invalidateRect( const lvRect & rc )
        {
            _updateRect.extend( rc );
        }
        CRGUIScreenBase( int width, int height, bool doublebuffer  )
        : _width( width ), _height( height ), _canvas(NULL), _front(NULL)
        {
            if ( width && height ) {
                _canvas = LVRef<LVDrawBuf>( createCanvas( width, height ) );
                if ( doublebuffer )
                    _front = LVRef<LVDrawBuf>( createCanvas( width, height ) );
            }
        }
        virtual ~CRGUIScreenBase()
        {
        }
};

#ifdef CR_WX_SUPPORT
/// WXWidget support: draw to wxImage
class CRWxScreen : public CRGUIScreenBase
{
    protected:
        wxBitmap _wxbitmap;
        virtual void update( const lvRect & rc, bool full )
        {
            wxImage img;
            int dyy = _canvas->GetHeight();
            int dxx = _canvas->GetWidth();
            int dx = dxx;
            int dy = dyy;
            img.Create(dx, dy, true);
            unsigned char * bits = img.GetData();
            for ( int y=0; y<dy && y<dyy; y++ ) {
                int bpp = _canvas->GetBitsPerPixel();
                if ( bpp==32 ) {
                    const lUInt32* src = (const lUInt32*) _canvas->GetScanLine( y );
                    unsigned char * dst = bits + y*dx*3;
                    for ( int x=0; x<dx && x<dxx; x++ )
                    {
                        lUInt32 c = *src++;
                        *dst++ = (c>>16) & 255;
                        *dst++ = (c>>8) & 255;
                        *dst++ = (c>>0) & 255;
                    }
                } else if ( bpp==2 ) {
                    //
                    static const unsigned char palette[4][3] = {
                        { 0xff, 0xff, 0xff },
                        { 0xaa, 0xaa, 0xaa },
                        { 0x55, 0x55, 0x55 },
                        { 0x00, 0x00, 0x00 },
                    };
                    const lUInt8* src = (const lUInt8*) _canvas->GetScanLine( y );
                    unsigned char * dst = bits + y*dx*3;
                    for ( int x=0; x<dx && x<dxx; x++ )
                    {
                        lUInt32 c = (( src[x>>2] >> ((3-(x&3))<<1) ))&3;
                        *dst++ = palette[c][0];
                        *dst++ = palette[c][1];
                        *dst++ = palette[c][2];
                    }
                } else if ( bpp==1 ) {
                    //
                    static const unsigned char palette[2][3] = {
                        { 0xff, 0xff, 0xff },
                        { 0x00, 0x00, 0x00 },
                    };
                    const lUInt8* src = (const lUInt8*) _canvas->GetScanLine( y );
                    unsigned char * dst = bits + y*dx*3;
                    for ( int x=0; x<dx && x<dxx; x++ )
                    {
                        lUInt32 c = (( src[x>>3] >> ((7-(x&7))) ))&1;
                        *dst++ = palette[c][0];
                        *dst++ = palette[c][1];
                        *dst++ = palette[c][2];
                    }
                }
            }

            // copy to bitmap
            wxBitmap bmp( img );
            _wxbitmap = bmp;
        }
    public:
        CRWxScreen( int width, int height )
        :  CRGUIScreenBase( width, height, true ) { }
        wxBitmap getWxBitmap() { return _wxbitmap; }
};
#endif

/// Window to show LVDocView contents
class CRDocViewWindow : public CRGUIWindowBase
{
    protected:
        LVDocView * _docview;
        virtual void draw()
        {
            LVDocImageRef pageImage = _docview->getPageImage(0);
            LVDrawBuf * drawbuf = pageImage->getDrawBuf();
            _wm->getScreen()->draw( drawbuf, _rect.left, _rect.top );
        }
    public:
        LVDocView * getDocView()
        {
            return _docview;
        }
        CRDocViewWindow( CRGUIWindowManager * wm )
        : CRGUIWindowBase( wm )
        {
            _docview = new LVDocView();
            _docview->Resize( getWidth(), getHeight() );
        }
        virtual ~CRDocViewWindow()
        {
            delete _docview;
        }
        virtual void setRect( const lvRect & rc )
        {
            if ( rc == _rect )
                return;
            _rect = rc;
            _docview->Resize( getWidth(), getHeight() );
            setDirty();
        }
        /// returns true if command is processed
        virtual bool onCommand( int command, int params )
        {
            if ( command >= LVDOCVIEW_COMMANDS_START && command <= LVDOCVIEW_COMMANDS_END ) {
                _docview->doCommand( (LVDocCmd)command, params );
                _dirty = true;
                return true;
            }
            return false;
        }
        /// returns true if window is changed but now drawn
        virtual bool isDirty()
        {
            return _dirty || !_docview->IsRendered() || !_docview->IsDrawed();
        }
};





//===========================================================================================
// MENU SUPPORT

enum CRMenuControlCmd {
    MCMD_CANCEL=500,
    MCMD_OK,
    MCMD_SCROLL_FORWARD,
    MCMD_SCROLL_BACK,
    MCMD_SELECT_1,
    MCMD_SELECT_2,
    MCMD_SELECT_3,
    MCMD_SELECT_4,
    MCMD_SELECT_5,
    MCMD_SELECT_6,
    MCMD_SELECT_7,
    MCMD_SELECT_8,
    MCMD_SELECT_9,
};

class CRMenu;

/// CRGUI menu item base class
class CRMenuItem
{
    protected:
        CRMenu * _menu;
        int _id;
        lString16 _label;
        LVImageSourceRef _image;
        LVFontRef _defFont;
        lString16 _propValue;
    public:
        /// id of item
        int getId() { return _id; }
        /// item label
        lString16 getLabel() { return _label; }
        /// item icon
        LVImageSourceRef getImage() { return _image; }
        /// item label font
        LVFontRef getFont() { return _defFont; }
        /// constructor
        CRMenuItem( CRMenu * menu, int id, lString16 label, LVImageSourceRef image, LVFontRef defFont, const lChar16 * propValue=NULL  )
    : _menu(menu), _id(id), _label(label), _image(image), _defFont(defFont), _propValue(propValue) { }
        /// measures item size
        virtual lvPoint getItemSize();
        /// draws item
        virtual void Draw( LVDrawBuf & buf, lvRect & rc, bool selected );
        /// returns true if submenu
        virtual bool isSubmenu() { return false; }
        /// called on item selection
        virtual int onSelect() { return 0; }
        virtual ~CRMenuItem() { }
        /// submenu for options dialog support
        virtual lString16 getSubmenuValue() { return lString16(); }
        /// property value, for options editor support
        virtual lString16 getPropValue() { return _propValue; }
};

/// CRGUI menu base class
class CRMenu : public CRMenuItem, public CRGUIWindowBase {
    protected:
        LVPtrVector<CRMenuItem> _items;
        CRPropRef _props;
        lString16 _propName;
        LVFontRef _valueFont;
        int _topItem;
        int _pageItems;
        // override for CRGUIWindow method
        virtual void draw();
        virtual void Draw( LVDrawBuf & buf, lvRect & rc, bool selected );
        virtual void Draw( LVDrawBuf & buf, int x, int y );
    public:
        CRMenu( CRGUIWindowManager * wm, CRMenu * parentMenu, int id, lString16 label, LVImageSourceRef image, LVFontRef defFont, LVFontRef valueFont, CRPropRef props=CRPropRef(), const char * propName=NULL )
        : CRGUIWindowBase( wm ), CRMenuItem( parentMenu, id, label, image, defFont ), _props(props), _propName(Utf8ToUnicode(lString8(propName))), _valueFont(valueFont), _topItem(0), _pageItems(8) { }
        virtual bool isSubmenu() { return true; }
        LVPtrVector<CRMenuItem> & getItems() { return _items; }
        CRPropRef getProps() { return _props; }
        lString16 getPropName() { return _propName; }
        void addItem( CRMenuItem * item ) { _items.add( item ); }
        CRMenuItem * findItem( int id ) {
            for ( int i=0; i<_items.length(); i++ )
                if ( _items[i]->getId()==id )
                    return _items[i];
            return NULL;
        }
        CRMenu * findSubmenu( int id ) {
            for ( int i=0; i<_items.length(); i++ )
                if ( _items[i]->getId()==id && _items[i]->isSubmenu() )
                    return (CRMenu*)_items[i];
            return NULL;
        }
        virtual int getPageCount();
        virtual void setCurPage( int nPage );
        virtual int getCurPage( );
        virtual int getTopItem();
        virtual lString16 getSubmenuValue();
        virtual void toggleSubmenuValue();
        virtual int getItemHeight();
        virtual lvPoint getMaxItemSize();
        virtual lvPoint getItemSize();
        virtual lvPoint getSize();
        virtual ~CRMenu() { }
        // CRGUIWindow
        virtual const lvRect & getRect();
        /// returns true if command is processed
        virtual bool onCommand( int command, int params = 0 );
        /// closes menu and its submenus, posts command
        virtual void closeMenu( int command, int params = 0 );
        /// closes top level menu and its submenus, posts command
        virtual void closeAllMenu( int command, int params = 0 );
        /// closes menu and its submenus
        virtual void destroyMenu();
};


#endif// CR_GUI_INCLUDED
