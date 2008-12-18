//
// C++ Interface: settings
//
// Description: 
//
//
// Author: Vadim Lopatin <vadim.lopatin@coolreader.org>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MAINWND_H_INCLUDED
#define MAINWND_H_INCLUDED

#include <crengine.h>
#include <crgui.h>
#include <crtrace.h>
#include "settings.h"

#define CR_USE_XCB
#define WITH_DICT

#ifdef _WIN32
#define XK_Return   0xFF01
#define XK_Up       0xFF02
#define XK_Down     0xFF03
#define XK_Escape   0xFF04
#else

#ifdef CR_USE_XCB
#define XK_MISCELLANY
#include <X11/keysymdef.h>
#endif
#endif

#define MAIN_MENU_COMMANDS_START 200
enum CRMainMenuCmd
{
    MCMD_BEGIN = MAIN_MENU_COMMANDS_START,
    MCMD_QUIT,
    MCMD_MAIN_MENU,
    MCMD_GO_PAGE,
    MCMD_GO_PAGE_APPLY,
    MCMD_SETTINGS,
    MCMD_SETTINGS_APPLY,
#ifdef WITH_DICT
    MCMD_DICT,
#endif
};

class V3DocViewWin : public CRDocViewWindow
{
protected:
    CRPropRef _props;
    CRPropRef _newProps;
    CRGUIAcceleratorTableRef _menuAccelerators;
    CRGUIAcceleratorTableRef _dialogAccelerators;
	lString16 _dataDir;
public:
	CRGUIAcceleratorTableRef getMenuAccelerators() { return _menuAccelerators; }
	CRGUIAcceleratorTableRef getDialogAccelerators() { return _dialogAccelerators; }

    /// returns current properties
    CRPropRef getProps() { return _props; }

    /// sets new properties
    void setProps( CRPropRef props )
    {
        _props = props;
        _docview->propsUpdateDefaults( _props );
    }

    V3DocViewWin( CRGUIWindowManager * wm, lString16 dataDir );

    void applySettings();

    void showSettingsMenu();

    void showMainMenu();

    void showGoToPageDialog();

    virtual bool onCommand( int command, int params );
};


#endif
