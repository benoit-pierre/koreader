#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui/QFileDialog>
#include <QtGui/QStyleFactory>
#include <QClipboard>
#include <QTranslator>
#include <QLibraryInfo>
#include "settings.h"
#include "tocdlg.h"
#include "recentdlg.h"
#include "aboutdlg.h"
#include "filepropsdlg.h"
#include "bookmarklistdlg.h"
#include "addbookmarkdlg.h"
#include "crqtutil.h"
#include "../crengine/include/lvtinydom.h"

#define DOC_CACHE_SIZE 128 * 0x100000

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    ui->setupUi(this);
    ui->view->setScrollBar( ui->scroll );

    addAction(ui->actionOpen);
    addAction(ui->actionRecentBooks);
    addAction(ui->actionTOC);
    addAction(ui->actionToggle_Full_Screen);
    addAction(ui->actionSettings);
    addAction(ui->actionClose);
    addAction(ui->actionToggle_Pages_Scroll);
    addAction(ui->actionMinimize);
    addAction(ui->actionNextPage);
    addAction(ui->actionPrevPage);
    addAction(ui->actionNextLine);
    addAction(ui->actionPrevLine);
    addAction(ui->actionFirstPage);
    addAction(ui->actionLastPage);
    addAction(ui->actionBack);
    addAction(ui->actionForward);
    addAction(ui->actionNextChapter);
    addAction(ui->actionPrevChapter);
    addAction(ui->actionZoom_In);
    addAction(ui->actionZoom_Out);
    addAction(ui->actionCopy);
    addAction(ui->actionCopy2); // alternative shortcut
    addAction(ui->actionAddBookmark);
    addAction(ui->actionShowBookmarksList);
    addAction(ui->actionCopy);

#ifdef _LINUX
    QString homeDir = QDir::toNativeSeparators(QDir::homePath() + "/.cr3/");
#else
    QString homeDir = QDir::toNativeSeparators(QDir::homePath() + "/cr3/");
#endif
    QString exeDir = QDir::toNativeSeparators(qApp->applicationDirPath() + "/"); //QDir::separator();
    QString cacheDir = homeDir + "cache";
    QString histFile = homeDir + "cr3hist.bmk";
    QString iniFile = homeDir + "cr3.ini";
    QString cssFile = homeDir + "fb2.css";
    QString cssFile2 = exeDir + "fb2.css";
    QString translations = exeDir + "i18n";
    QString hyphDir = exeDir + "hyph" + QDir::separator();
    ldomDocCache::init( qt2cr( cacheDir ), DOC_CACHE_SIZE );
    ui->view->setPropsChangeCallback( this );
    ui->view->loadSettings( iniFile );
    ui->view->loadHistory( histFile );
    ui->view->setHyphDir( hyphDir );
    if ( !ui->view->loadCSS( cssFile ) )
        ui->view->loadCSS( cssFile2 );

//     QTranslator qtTranslator;
//     if (qtTranslator.load("qt_" + QLocale::system().name(),
//             QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
//        QApplication::installTranslator(&qtTranslator);
//
//     QTranslator myappTranslator;
//     QString trname = "cr3_" + QLocale::system().name();
//     CRLog::info("Using translation file %s from dir %s", UnicodeToUtf8(qt2cr(trname)).c_str(), UnicodeToUtf8(qt2cr(translations)).c_str() );
//     if ( myappTranslator.load(trname, translations) )
//         QApplication::installTranslator(&myappTranslator);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_view_destroyed()
{
    //
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open book file"),
         "",
         tr("All supported formats (*.fb2 *.txt *.tcr *.rtf *.epub *.html *.htm *.zip);;FB2 books (*.fb2 *.fb2.zip);;Text files (*.txt);;Rich text (*.rtf);;HTML files (*.htm *.html);;EPUB files (*.epub);;ZIP archives (*.zip)"));
    if ( fileName.length()==0 )
        return;
    if ( !ui->view->loadDocument( fileName ) ) {
        // error
    } else {
        update();
    }
}

void MainWindow::on_actionMinimize_triggered()
{
    showMinimized();
}

void MainWindow::on_actionClose_triggered()
{
    close();
}

void MainWindow::on_actionNextPage_triggered()
{
    ui->view->nextPage();
}

void MainWindow::on_actionPrevPage_triggered()
{
    ui->view->prevPage();
}

void MainWindow::on_actionNextLine_triggered()
{
    ui->view->nextLine();
}

void MainWindow::on_actionPrevLine_triggered()
{
    ui->view->prevLine();
}

void MainWindow::on_actionFirstPage_triggered()
{
    ui->view->firstPage();
}

void MainWindow::on_actionLastPage_triggered()
{
    ui->view->lastPage();
}

void MainWindow::on_actionBack_triggered()
{
    ui->view->historyBack();
}

void MainWindow::on_actionForward_triggered()
{
    ui->view->historyForward();
}

void MainWindow::on_actionNextChapter_triggered()
{
    ui->view->nextChapter();
}

void MainWindow::on_actionPrevChapter_triggered()
{
    ui->view->prevChapter();
}

void MainWindow::on_actionToggle_Pages_Scroll_triggered()
{
    ui->view->togglePageScrollView();
}

void MainWindow::on_actionToggle_Full_Screen_triggered()
{
    toggleProperty( PROP_WINDOW_FULLSCREEN );
}

void MainWindow::on_actionZoom_In_triggered()
{
    ui->view->zoomIn();
}

void MainWindow::on_actionZoom_Out_triggered()
{
    ui->view->zoomOut();
}

void MainWindow::on_actionTOC_triggered()
{
    TocDlg::showDlg( this, ui->view );
}

void MainWindow::on_actionRecentBooks_triggered()
{
    RecentBooksDlg::showDlg( this, ui->view );
}

void MainWindow::on_actionSettings_triggered()
{
    SettingsDlg::showDlg( this, ui->view );
}

void MainWindow::toggleProperty( const char * name )
{
    ui->view->toggleProperty( name );
}

void MainWindow::onPropsChange( PropsRef props )
{
    for ( int i=0; i<props->count(); i++ ) {
        QString name = props->name( i );
        QString value = props->value( i );
        int v = (value != "0");
        CRLog::debug("MainWindow::onPropsChange [%d] '%s'=%s ", i, props->name(i), props->value(i).toUtf8().data() );
        if ( name == PROP_WINDOW_FULLSCREEN ) {
            bool state = windowState().testFlag(Qt::WindowFullScreen);
            bool vv = v ? true : false;
            if ( state != vv )
                setWindowState( windowState() ^ Qt::WindowFullScreen );
        }
        if ( name == PROP_WINDOW_SHOW_MENU ) {
            ui->menuBar->setVisible( v );
        }
        if ( name == PROP_WINDOW_SHOW_SCROLLBAR ) {
            ui->scroll->setVisible( v );
        }
        if ( name == PROP_WINDOW_TOOLBAR_SIZE ) {
            ui->mainToolBar->setVisible( v );
        }
        if ( name == PROP_WINDOW_SHOW_STATUSBAR ) {
            ui->statusBar->setVisible( v );
        }
        if ( name == PROP_WINDOW_STYLE ) {
            QApplication::setStyle( value );
        }
    }
}

void MainWindow::contextMenu( QPoint pos )
{
    QMenu *menu = new QMenu;
    menu->addAction(ui->actionOpen);
    menu->addAction(ui->actionRecentBooks);
    menu->addAction(ui->actionTOC);
    menu->addAction(ui->actionToggle_Full_Screen);
    menu->addAction(ui->actionSettings);
    if ( ui->view->isPointInsideSelection( pos ) )
        menu->addAction(ui->actionCopy);
    menu->addAction(ui->actionAddBookmark);
    menu->addAction(ui->actionClose);
    menu->exec(ui->view->mapToGlobal(pos));
}


void MainWindow::on_actionCopy_triggered()
{
    QString txt = ui->view->getSelectionText();
    if ( txt.length()>0 ) {
         QClipboard * clipboard = QApplication::clipboard();
         clipboard->setText(txt);
    }
}

void MainWindow::on_actionCopy2_triggered()
{
    on_actionCopy_triggered();
}

static bool firstShow = true;

void MainWindow::showEvent ( QShowEvent * event )
{
    if ( !firstShow )
        return;
    CRLog::debug("first showEvent()");
    firstShow = false;
    int n = ui->view->getOptions()->getIntDef( PROP_APP_START_ACTION, 0 );
    if ( n==0 ) {
        // open recent book
        CRLog::info("Startup Action: Open recent book");
        ui->view->loadLastDocument();
    } else if ( n==1 ) {
        // show recent books dialog
        CRLog::info("Startup Action: Show recent books dialog");
        //hide();
        RecentBooksDlg::showDlg( this, ui->view );
        //show();
    } else if ( n==2 ) {
        // show file open dialog
        CRLog::info("Startup Action: Show file open dialog");
        //hide();
        on_actionOpen_triggered();
        //RecentBooksDlg::showDlg( ui->view );
        //show();
    }
}

static bool firstFocus = true;

void MainWindow::focusInEvent ( QFocusEvent * event )
{
    if ( !firstFocus )
        return;
    CRLog::debug("first focusInEvent()");
//    int n = ui->view->getOptions()->getIntDef( PROP_APP_START_ACTION, 0 );
//    if ( n==1 ) {
//        // show recent books dialog
//        CRLog::info("Startup Action: Show recent books dialog");
//        RecentBooksDlg::showDlg( ui->view );
//    }

    firstFocus = false;
}

void MainWindow::on_actionAboutQT_triggered()
{
    QApplication::aboutQt();
}

void MainWindow::on_actionAboutCoolReader_triggered()
{
    AboutDialog::showDlg( this );
}

void MainWindow::on_actionAddBookmark_triggered()
{
    AddBookmarkDialog::showDlg( this, ui->view );
}

void MainWindow::on_actionShowBookmarksList_triggered()
{
    BookmarkListDialog::showDlg( this, ui->view );
}

void MainWindow::on_actionFileProperties_triggered()
{
    FilePropsDialog::showDlg( this, ui->view );
}
