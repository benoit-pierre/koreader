/*******************************************************

   CoolReader Engine

   lvxml.cpp:  XML parser implementation

   (c) Vadim Lopatin, 2000-2006
   This source code is distributed under the terms of
   GNU General Public License
   See LICENSE file for details

*******************************************************/

#include <stdlib.h>
#include "../include/crgui.h"

//TODO: place to skin file
#define ITEM_MARGIN 8
#define HOTKEY_SIZE 36
#define MENU_NUMBER_FONT_SIZE 24
#define SCROLL_HEIGHT 24
#define DEF_FONT_SIZE 22
#define DEF_TITLE_FONT_SIZE 28


void CRGUIScreenBase::flush( bool full )
{
    if ( _updateRect.isEmpty() && !full )
        return;
    if ( !_front.isNull() && !_updateRect.isEmpty() && !full ) {
        // calculate really changed area
        lvRect rc;
        lvRect lineRect(_updateRect);
        int sz = _width * _canvas->GetBitsPerPixel() / 8;
        for ( int y = _updateRect.top; y < _updateRect.bottom; y++ ) {
            if ( y>=0 && y<_height ) {
                void * line1 = _canvas->GetScanLine( y );
                void * line2 = _front->GetScanLine( y );
                if ( memcmp( line1, line2, sz ) ) {
                    // line content is different
                    lineRect.top = y;
                    lineRect.bottom = y+1;
                    rc.extend( lineRect );
                    // copy line to front buffer
                    memcpy( line2, line1, sz );
                }
            }
        }
        if ( rc.isEmpty() ) {
            // no actual changes
            _updateRect.clear();
            return;
        }
        _updateRect.top = rc.top;
        _updateRect.bottom = rc.bottom;
    }
    if ( full && !_front.isNull() ) {
        // copy full screen to front buffer
        _canvas->DrawTo( _front.get(), 0, 0, 0, NULL );
    }
    if ( full )
        _updateRect = getRect();
    update( _updateRect, full );
    _updateRect.clear();
}


/// returns true if key is processed (by default, let's translate key to command using accelerator table)
bool CRGUIWindowBase::onKeyPressed( int key, int flags )
{
    if ( _acceleratorTable.isNull() )
        return false;
    int cmd, param;
    if ( _acceleratorTable->translate( key, flags, cmd, param ) ) {
        return onCommand( cmd, param );
    }
    return false;
}



void CRMenuItem::Draw( LVDrawBuf & buf, lvRect & rc, CRRectSkinRef skin, bool selected )
{
    lvRect itemBorders = skin->getBorderWidths();
    skin->draw( buf, rc );
    buf.SetTextColor( 0x000000 );
    buf.SetBackgroundColor( 0xFFFFFF );
    int imgWidth = 0;
	int hh = rc.bottom - rc.top - itemBorders.top - itemBorders.bottom;
    if ( !_image.isNull() ) {
        int w = _image->GetWidth();
        int h = _image->GetHeight();
		buf.Draw( _image, rc.left + hh/2-w/2 + itemBorders.left, rc.top + hh/2 - h/2 + itemBorders.top, w, h );
        imgWidth = w + ITEM_MARGIN;
    }
	_defFont->DrawTextString( &buf, rc.left + imgWidth + itemBorders.left, rc.top + hh/2 + itemBorders.top - _defFont->getHeight()/2, _label.c_str(), _label.length(), L'?', NULL, false, 0 );
}

int CRMenu::getPageCount()
{
    return (_items.length() + _pageItems - 1) / _pageItems;
}

void CRMenu::setCurPage( int nPage )
{
    _topItem = _pageItems * nPage;
    if ( _topItem + _pageItems > (int)_items.length() )
        _topItem = (int)_items.length() - _pageItems;
    if ( _topItem < 0 )
        _topItem = 0;
}

int CRMenu::getCurPage( )
{
    return (_topItem + (_pageItems-1)) / _pageItems;
}

int CRMenu::getTopItem()
{
    return _topItem;
}

void CRMenu::Draw( LVDrawBuf & buf, lvRect & rc, CRRectSkinRef skin, bool selected )
{
    CRMenuItem::Draw( buf, rc, skin, selected );
    lString16 s = getSubmenuValue();
    if ( s.empty() )
        return;
    int w = _valueFont->getTextWidth( s.c_str(), s.length() );
    int hh = rc.bottom - rc.top;
    _valueFont->DrawTextString( &buf, rc.right - w - ITEM_MARGIN, rc.top + hh/2 - _valueFont->getHeight()/2, s.c_str(), s.length(), L'?', NULL, false, 0 );
}

lvPoint CRMenuItem::getItemSize()
{
    int h = _defFont->getHeight() * 5/4;
    int w = _defFont->getTextWidth( _label.c_str(), _label.length() );
    w += ITEM_MARGIN * 2;
    if ( !_image.isNull() ) {
        if ( _image->GetHeight()>h )
            h = _image->GetHeight() * 8 / 7;
        w += h;
    }
	if ( h < 48 )
		h = 48;
    return lvPoint( w, h );
}

lvPoint CRMenu::getItemSize()
{
    lString16 path = lString16(L"/CR3Skin/") + getSkinName();
    CRMenuSkinRef skin = _wm->getSkin()->getMenuSkin( path.c_str() );
    CRRectSkinRef itemSkin = skin->getItemSkin();
    lvRect itemBorders = itemSkin->getBorderWidths();
    lvPoint sz = CRMenuItem::getItemSize();
    if ( !isSubmenu() || _propName.empty() || _props.isNull() )
        return sz;
    int maxw = 0;
    for ( int i=0; i<_items.length(); i++ ) {
        lString16 s = _items[i]->getLabel();
        int w = _valueFont->getTextWidth( s.c_str(), s.length() );
        if ( w > maxw )
            maxw = w;
    }
    if ( maxw>0 )
        sz.x = sz.x + itemBorders.left + itemBorders.right + maxw;
    return sz;
}

int CRMenu::getItemHeight()
{
    int h = _defFont->getHeight() * 5/4;
	if ( h < 48 )
		h = 48;
	return h;
}

lvPoint CRMenu::getMaxItemSize()
{
    lvPoint mySize = getItemSize();
    int itemHeight = getItemHeight();
    int maxx = 0;
    int maxy = 0;
    for ( int i=0; i<_items.length(); i++ ) {
        lvPoint sz = _items[i]->getItemSize();
        if ( maxx < sz.x )
            maxx = sz.x;
        if ( maxy < sz.y )
            maxy = sz.y;
    }
    if ( maxx < mySize.x )
        maxx = mySize.x;
    if ( maxy < mySize.y )
        maxy = mySize.y;
    return lvPoint( maxx, maxy );
}

lvPoint CRMenu::getSize()
{
    lvPoint itemSize = getMaxItemSize();
    int nItems = _items.length();
    int scrollHeight = 0;
    if ( nItems > _pageItems ) {
        nItems = _pageItems;
        scrollHeight = SCROLL_HEIGHT;
    }
    int h = nItems * (itemSize.y) + scrollHeight;
    int w = itemSize.x + 3 * ITEM_MARGIN + HOTKEY_SIZE;
    if ( w>600 )
        w = 600;
    lString16 path = lString16(L"/CR3Skin/") + getSkinName();
    CRMenuSkinRef skin = _wm->getSkin()->getMenuSkin( path.c_str() );
    return skin->getWindowSize( lvPoint( w, h ) );
}

lString16 CRMenu::getSubmenuValue()
{
    if ( !isSubmenu() || _propName.empty() || _props.isNull() )
        return lString16();
    lString16 value = getProps()->getStringDef(
                               UnicodeToUtf8(getPropName()).c_str(), "");
    for ( int i=0; i<_items.length(); i++ ) {
        if ( !_items[i]->getPropValue().empty() &&
                value==(_items[i]->getPropValue()) )
            return _items[i]->getLabel();
    }
    return lString16();
}

void CRMenu::toggleSubmenuValue()
{
    if ( !isSubmenu() || _propName.empty() || _props.isNull() )
        return;
    lString16 value = getProps()->getStringDef(
                               UnicodeToUtf8(getPropName()).c_str(), "");
    for ( int i=0; i<_items.length(); i++ ) {
        if ( !_items[i]->getPropValue().empty() &&
              value==(_items[i]->getPropValue()) ) {
            int n = (i + 1) % _items.length();
            getProps()->setString(UnicodeToUtf8(getPropName()).c_str(), _items[n]->getPropValue() );
            //return _items[i]->getLabel();
            return;
        }
    }
}

static void DrawArrow( LVDrawBuf & buf, int x, int y, int dx, int dy, lvColor cl, int direction )
{
    int x0 = x + dx/2;
    int y0 = y + dy/2;
    dx -= 4;
    dy -= 4;
    dx /= 2;
    dy /= 2;
    int deltax = direction?-1:1;
    x0 -= deltax * dx/2;
    for ( int k=0; k<dx; k++ ) {
        buf.FillRect( x0+k*deltax, y0-k, x0+k*deltax+1, y0+k+1, cl );
    }
}

void CRMenu::Draw( LVDrawBuf & buf, int x, int y )
{
    lString16 path = lString16(L"/CR3Skin/") + getSkinName();
    CRMenuSkinRef skin = _wm->getSkin()->getMenuSkin( path.c_str() );
    CRRectSkinRef itemSkin = skin->getItemSkin();
    CRRectSkinRef titleSkin = skin->getTitleSkin();
    CRRectSkinRef itemShortcutSkin = skin->getItemShortcutSkin();
    lvRect itemBorders = itemSkin->getBorderWidths();
    lvRect headerRc = skin->getTitleRect(_rect);

    buf.SetTextColor( 0x000000 );
    buf.SetBackgroundColor( 0xFFFFFF );

    skin->draw( buf, _rect );
    titleSkin->draw( buf, headerRc );
    titleSkin->drawText( buf, headerRc, _label );

    lvPoint itemSize = getMaxItemSize();
    int hdrHeight = itemSize.y; // + ITEM_MARGIN + ITEM_MARGIN;
    lvPoint sz = getSize();

    int nItems = _items.length();
    int scrollHeight = 0;
    if ( nItems > _pageItems ) {
        nItems = _pageItems;
        scrollHeight = SCROLL_HEIGHT;
    }

	lvRect itemsRc( skin->getClientRect(_rect) );
	itemsRc.bottom -= scrollHeight;
	//lvRect headerRc( x + itemBorders.left, y + itemBorders.top, x + sz.x - itemBorders.right, itemsRc.top+1 );
    lvRect scrollRc( skin->getClientRect(_rect) );
	scrollRc.top = scrollRc.bottom - scrollHeight;
    //buf.FillRect( x, y, x+sz.x, y+sz.y, buf.GetBackgroundColor() );
    //buf.Rect( headerRc, buf.GetTextColor() );
    //buf.Rect( itemsRc, buf.GetTextColor() );
    // draw scrollbar
    if ( scrollHeight ) {
        int totalCount = _items.length();
        int visibleCount = _pageItems;
        buf.Rect( scrollRc, buf.GetTextColor() );
        scrollRc.shrink( 2 );
        buf.Rect( scrollRc.left, scrollRc.top, scrollRc.left+SCROLL_HEIGHT - 4, scrollRc.bottom, buf.GetTextColor() );
        buf.Rect( scrollRc.right - SCROLL_HEIGHT + 4, scrollRc.top, scrollRc.right, scrollRc.bottom, buf.GetTextColor() );
        DrawArrow( buf, scrollRc.left, scrollRc.top, SCROLL_HEIGHT-4, SCROLL_HEIGHT-4, _topItem>0?buf.GetTextColor() : 0xAAAAAA, 0 );
        DrawArrow( buf, scrollRc.right-SCROLL_HEIGHT+4, scrollRc.top, SCROLL_HEIGHT-4, SCROLL_HEIGHT-4, _topItem < totalCount - visibleCount ? buf.GetTextColor() : 0xAAAAAA, 1 );
        scrollRc.left += SCROLL_HEIGHT - 3;
        scrollRc.right -= SCROLL_HEIGHT - 3;
        int x = scrollRc.width() * _topItem / totalCount;
        int endx = scrollRc.width() * (_topItem + visibleCount) / totalCount;
        CRLog::trace("scrollBar: x=%d, dx=%d, _topItem=%d, visibleCount=%d, totalCount=%d", x, endx, _topItem, visibleCount, totalCount );
        scrollRc.right = scrollRc.left + endx;
        scrollRc.left += x;
        buf.Rect( scrollRc, buf.GetTextColor() );
        scrollRc.shrink( 2 );
        buf.FillRect( scrollRc, 0xAAAAAA );
    }
    //headerRc.shrink( 2 );
    //buf.FillRect( headerRc, 0xA0A0A0 );
    //headerRc.shrink( ITEM_MARGIN );
    //CRMenuItem::Draw( buf, headerRc, itemSkin, false );
    lvRect rc( itemsRc );
    rc.top += 0; //ITEM_MARGIN;
    //rc.left += ITEM_MARGIN;
    //rc.right -= ITEM_MARGIN;
    LVFontRef numberFont( fontMan->GetFont( MENU_NUMBER_FONT_SIZE, 600, true, css_ff_sans_serif, lString8("Arial")) );
    for ( int index=0; index<_pageItems; index++ ) {
        int i = _topItem + index;
        if ( i >= _items.length() )
            break;
        bool selected = false; //TODO
        if ( !getProps().isNull() && !_items[i]->getPropValue().empty() &&
              getProps()->getStringDef(
                       UnicodeToUtf8(getPropName()).c_str()
                       , "")==(_items[i]->getPropValue()) )
            selected = true;

        rc.bottom = rc.top + itemSize.y;
        if ( selected ) {
            lvRect sel = rc;
            sel.extend( ITEM_MARGIN/3 );
            buf.Rect(sel, 0xAAAAAA );
            sel.extend( 1 );
            buf.Rect(sel, 0x555555 );
        }
        // number
        lvRect numberRc( rc );
        //numberRc.extend(ITEM_MARGIN/4); //ITEM_MARGIN/8-2);
        numberRc.right = numberRc.left + HOTKEY_SIZE;

        itemShortcutSkin->draw( buf, numberRc );
        lString16 number = index<9 ? lString16::itoa( index+1 ) : L"0";
        itemShortcutSkin->drawText( buf, numberRc, number );
        // item
        lvRect itemRc( rc );
        itemRc.left = numberRc.right;
        _items[i]->Draw( buf, itemRc, itemSkin, selected );
        rc.top += itemSize.y;
    }
}

/// closes menu and its submenus
void CRMenu::destroyMenu()
{
    for ( int i=_items.length()-1; i>=0; i-- )
        if ( _items[i]->isSubmenu() ) {
            ((CRMenu*)_items[i])->destroyMenu();
            _items.remove( i );
        }
    _wm->closeWindow( this ); // close, for root menu
}

/// closes menu and its submenus
void CRMenu::closeMenu( int command, int params )
{
    for ( int i=0; i<_items.length(); i++ )
        if ( _items[i]->isSubmenu() )
            ((CRMenu*)_items[i])->closeMenu( 0, 0 );
    if ( _menu != NULL )
        _wm->showWindow( this, false ); // just hide, for submenus
    else {
        if ( command )
            _wm->postCommand( command, params );
        destroyMenu();
    }
}

/// closes top level menu and its submenus, posts command
void CRMenu::closeAllMenu( int command, int params )
{
    CRMenu* p = this;
    while ( p->_menu )
        p = p->_menu;
    if ( command )
        _wm->postCommand( command, params );
    p->destroyMenu();
}

/// returns true if command is processed
bool CRMenu::onCommand( int command, int params )
{
    if ( command==MCMD_CANCEL ) {
        closeMenu( 0 );
        return true;
    }
    if ( command==MCMD_OK ) {
        int command = getId();
        if ( _menu != NULL )
            closeMenu( 0 );
        else
            closeMenu( command ); // close, for root menu
        return true;
    }
    if ( command==MCMD_SCROLL_FORWARD ) {
        setCurPage( getCurPage()+1 );
        return true;
    }
    if ( command==MCMD_SCROLL_BACK ) {
        setCurPage( getCurPage()-1 );
        return true;
    }
    int option = -1;
    if ( command>=MCMD_SELECT_1 && command<=MCMD_SELECT_9 )
        option = command - MCMD_SELECT_1;
    if ( option < 0 )
        return true;
    option += getTopItem();
    if ( option >= getItems().length() )
        return true;
    CRMenuItem * item = getItems()[option];
    if ( item->onSelect()>0 )
        return true;
    if ( item->isSubmenu() ) {
        // TODO: two-values submenu - toggle
        _wm->activateWindow( (CRMenu*) item );
        return true;
    } else {
        CRGUIWindowManager * wm = _wm;
        // command menu item
        if ( !item->getPropValue().empty() ) {
                // set property
            CRLog::trace("Setting property value");
            _props->setString( UnicodeToUtf8(getPropName()).c_str(), item->getPropValue() );
            int command = getId();
            if ( _menu != NULL )
                closeMenu( 0 );
            else
                closeMenu( command ); // close, for root menu
            return true;
        }
        int command = item->getId();
        if ( _menu != NULL )
            closeMenu( 0 );
        else
            closeMenu( command ); // close, for root menu
        return true;
    }
    return false;
}

const lvRect & CRMenu::getRect()
{
    lvPoint sz = getSize();
    lvRect rc = _wm->getScreen()->getRect();
    _rect = rc;
    _rect.top = _rect.bottom - sz.y;
    _rect.right = _rect.left + sz.x;
    return _rect;
}

void CRMenu::draw()
{
    Draw( *_wm->getScreen()->getCanvas(), _rect.left, _rect.top );
    //_wm->getScreen()->getCanvas()->Rect( _rect, 0xAAAAAA );
}
