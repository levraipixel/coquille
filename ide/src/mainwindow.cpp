/* Coquille
 * Copyright (C) 2009,2010
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/////////////////
//   Headers   //
/////////////////

//C++ Libs

//Qt Libs

//Project Libs
#ifdef TERMINAL
    #include "qtermwidget.h"
#endif

//Project Files
#include "filelink.h"
#include "mainwindow.h"
#include "qticonloader.h"
#include "settings.h"
#include "tabmanager.h"
#include "ui_mainwindow.h"

////////////////////
//   MainWindow   //
////////////////////

MainWindow::MainWindow( Settings *settings )
    : QMainWindow(0), ui(new Ui::MainWindowClass), m_settings(settings)
{
    ui->setupUi(this);

    m_settings->setMainWindow( this );
    m_tabManager = new TabManager( this, m_settings );
    ui->verticalLayout->insertWidget( 0, m_tabManager );

/***************************/
/*       Left Panel        */
/***************************/
    // FileBrowser
    ui->dockFileBrowser->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QPushButton *up = new QPushButton( "Go up", ui->dockFileBrowser );
    up->setText( "Go up" );
    ui->dockFileBrowser->layout()->addWidget( up );
    connect( up, SIGNAL(clicked()), this, SLOT(goUpInFileBrowser()) );
    m_fileBrowser = new QTreeView(ui->dockFileBrowser);
    ui->dockFileBrowser->setWidget(m_fileBrowser);
    fileBrowserDrawPath( QDir::currentPath() );

    addDockWidget(Qt::LeftDockWidgetArea, ui->dockFileBrowser);
    connect(m_fileBrowser, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(openSpecificFile(const QModelIndex &)));
    
    ui->dockFileBrowser->setVisible(false);    

    // Dock current files
    listOpenedDocuments = new QListWidget();
    ui->dockOpenedDocuments->setWidget(listOpenedDocuments);
    refreshOpenedDocuments();

    connect( m_tabManager, SIGNAL(tabOpen(QString,int)), this, SLOT(refreshOpenedDocuments()));
    connect( m_tabManager, SIGNAL(tabNameChange(QString,int)), this, SLOT(refreshOpenedDocuments()));
    connect( m_tabManager, SIGNAL(tabClose(QString,int)), this, SLOT(refreshOpenedDocuments()));

    connect(listOpenedDocuments, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(changeCurrentDocument(QModelIndex)));

    // all opened documents
    ui->dockOpenedDocuments->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, ui->dockOpenedDocuments);
    ui->dockOpenedDocuments->setVisible(false);   

    tabifyDockWidget( ui->dockFileBrowser, ui->dockOpenedDocuments);
    setTabPosition(Qt::LeftDockWidgetArea, QTabWidget::West);

/***************************/
/*       Central Panel     */
/***************************/

#ifdef TERMINAL
    m_terminalDock = new QDockWidget( ui->centralWidget );
    m_terminalDock->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable);

    restartTerminal();

    ui->verticalLayout->addWidget( m_terminalDock );

    m_actionTerminal = new QAction(ui->menuDisplay);
    m_actionTerminal->setCheckable(true);
    m_actionTerminal->setText( tr( "Terminal" ) );
    ui->menuDisplay->addAction( m_actionTerminal );

    m_actionRestartTerminal = new QAction(ui->menuDisplay);
    m_actionRestartTerminal->setText( tr( "Restart Terminal" ) );
    ui->menuDisplay->addAction( m_actionRestartTerminal );

    connect( m_actionTerminal, SIGNAL(toggled(bool)), this, SLOT(actionTerminal_toggled(bool)) );
    connect( m_actionRestartTerminal, SIGNAL(triggered()), this, SLOT(restartTerminal()) );
    connect( m_terminalDock, SIGNAL(visibilityChanged(bool)), this, SLOT(showTerminal(bool)) );

    actionTerminal_toggled( m_actionTerminal->isChecked() );
#endif

    on_actionOpenedDocuments_toggled( ui->actionOpenedDocuments->isChecked() );
    on_actionFileBrowser_toggled( ui->actionFileBrowser->isChecked() );
    on_actionDocumentationPanel_toggled( ui->actionDocumentationPanel->isChecked() );

    setIcons();

    ui->toolBarCoq->addSeparator();

    tactics = new QComboBox( ui->toolBarCoq );
    qDebug() << "lecture des tactiques";

    tactics->addItems( readLines( ":/SyntaxPrefix/tacticsList" ) );

    QIcon insertTacticIcon;
    insertTacticIcon.addFile(QString::fromUtf8(":/IconPrefix/images/insertText.svg"), QSize(), QIcon::Normal, QIcon::Off);
    QAction* tacticsAction = ui->toolBarCoq->addAction( QtIconLoader::icon("insert-text", insertTacticIcon), "" );
    ui->toolBarCoq->addWidget( tactics );

    connect(tactics, SIGNAL(activated(QString)), this, SLOT(insertText(QString)));
    qDebug() << "lecture des tactiques : done";

    ui->toolBarCoq->addSeparator();

    QComboBox *keywords = new QComboBox( ui->toolBarCoq );
    qDebug() << "lecture des mots clefs";

    keywords->addItems( readLines( ":/SyntaxPrefix/keywordsList" ) );

    QIcon insertKeywordIcon;
    insertKeywordIcon.addFile(QString::fromUtf8(":/IconPrefix/images/insertObject.svg"), QSize(), QIcon::Normal, QIcon::Off);
    ui->toolBarCoq->addAction( QtIconLoader::icon("insert-object", insertKeywordIcon), "" );
    ui->toolBarCoq->addWidget( keywords );

    connect(keywords, SIGNAL(activated(QString)), this, SLOT(insertText(QString)));
    qDebug() << "lecture des mots clefs : done";

    connect( ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()) );
    connect( m_tabManager, SIGNAL(displayDocName(QString)), this, SLOT(displayDocName(QString)) );
    connect( m_tabManager, SIGNAL(displayStatusBar(QString)), ui->statusBar, SLOT(showMessage(QString)) );
    connect( m_tabManager, SIGNAL(contentModified(bool)), this, SLOT(setWindowModified(bool)) );
    connect( ui->dockFileBrowser, SIGNAL(visibilityChanged(bool)), this, SLOT(showFileBrowser(bool)) );
    connect( ui->dockOpenedDocuments, SIGNAL(visibilityChanged(bool)), this, SLOT(showOpenedDocuments(bool)) );
    connect( ui->dockDocumentation, SIGNAL(visibilityChanged(bool)), this, SLOT(showDocumentationPanel(bool)) );
  
    setActions();
    readSettings();
    refreshRecentMenu();

    m_tabManager->addNewTab();
}

MainWindow::~MainWindow() {
    delete ui;
}

QList<QAction*> MainWindow::actions() {
    return m_actions.values();
}

QAction* MainWindow::getAction( QString name ) {
//    qDebug() << "action for " << name;
//    qDebug() << m_actions.value( name );
    return m_actions.value( name );
}

void MainWindow::setActions() {
    m_actions.clear();
    m_actions.reserve(63);
    m_actions.insert( "about", ui->actionAbout );
    m_actions.insert( "close", ui->actionClose );
    m_actions.insert( "closeAll", ui->actionCloseAll );
    m_actions.insert( "comment", ui->actionComment );
    m_actions.insert( "commentLines", ui->actionCommentLines );
    m_actions.insert( "copy", ui->actionCopy );
    m_actions.insert( "cut", ui->actionCut );
    m_actions.insert( "documentation", ui->actionDocumentation );
    m_actions.insert( "documentationPanel", ui->actionDocumentationPanel );
    m_actions.insert( "fileBrowser", ui->actionFileBrowser );
    m_actions.insert( "goToLine", ui->actionGoToLine );
    m_actions.insert( "indent", ui->actionIndent );
    m_actions.insert( "new", ui->actionNew );
    m_actions.insert( "open", ui->actionOpen );
    m_actions.insert( "openedDocuments", ui->actionOpenedDocuments );
    m_actions.insert( "paste", ui->actionPaste );
    m_actions.insert( "quit", ui->actionQuit );
    m_actions.insert( "redo", ui->actionRedo );
    m_actions.insert( "replace", ui->actionReplace );
    m_actions.insert( "save", ui->actionSave );
    m_actions.insert( "saveAll", ui->actionSaveAll );
    m_actions.insert( "saveAs", ui->actionSaveAs );
    m_actions.insert( "search", ui->actionSearch );
    m_actions.insert( "searchNext", ui->actionSearchNext );
    m_actions.insert( "selectAll", ui->actionSelectAll );
    m_actions.insert( "send", ui->actionSend );
    m_actions.insert( "sendAll", ui->actionSendAll );
    m_actions.insert( "sendToCursor", ui->actionSendToCursor );
    m_actions.insert( "settings", ui->actionSettings );
#ifdef TERMINAL
    m_actions.insert( "terminal", m_actionTerminal );
#endif
    m_actions.insert( "uncomment", ui->actionUncomment );
    m_actions.insert( "uncommentLines", ui->actionUncommentLines );
    m_actions.insert( "undo", ui->actionUndo );
    m_actions.insert( "unindent", ui->actionUnindent );
    m_actions.insert( "unsend", ui->actionUnsend );
    m_actions.insert( "unsendAll", ui->actionUnsendAll );
}

void MainWindow::setIcons() {
    QIcon windowIcon;
    windowIcon.addFile(QString::fromUtf8(":/IconPrefix/images/poussin.png"), QSize(), QIcon::Normal, QIcon::Off);
    setWindowIcon( windowIcon );

    QIcon new_file_icon;
    new_file_icon.addFile(QString::fromUtf8(":/IconPrefix/images/new_file.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionNew->setIcon(QtIconLoader::icon("document-new", new_file_icon));

    QIcon open_file_icon;
    open_file_icon.addFile(QString::fromUtf8(":/IconPrefix/images/open_file.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionOpen->setIcon(QtIconLoader::icon("document-open", open_file_icon));

    QIcon close_icon;
    close_icon.addFile(QString::fromUtf8(":/IconPrefix/images/close.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionClose->setIcon(QtIconLoader::icon("window-close", close_icon));

    QIcon save_file_icon;
    save_file_icon.addFile(QString::fromUtf8(":/IconPrefix/images/save_file.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionSave->setIcon(QtIconLoader::icon("document-save", save_file_icon));

    QIcon save_as_icon;
    save_as_icon.addFile(QString::fromUtf8(":/IconPrefix/images/save_file.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionSaveAs->setIcon(QtIconLoader::icon("document-save-as", save_file_icon));

    QIcon save_all_icon;
    save_all_icon.addFile(QString::fromUtf8(":/IconPrefix/images/save_all.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionSaveAll->setIcon(QtIconLoader::icon("document-save-all", save_all_icon));

    QIcon quit_icon;
    quit_icon.addFile(QString::fromUtf8(":/IconPrefix/images/quit.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionQuit->setIcon(QtIconLoader::icon("application-exit", quit_icon));

    QIcon select_all_icon;
    select_all_icon.addFile(QString::fromUtf8(":/IconPrefix/images/selectAll.svg"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionSelectAll->setIcon(QtIconLoader::icon("edit-select-all", select_all_icon));

    QIcon copy_icon;
    copy_icon.addFile(QString::fromUtf8(":/IconPrefix/images/copy.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionCopy->setIcon(QtIconLoader::icon("edit-copy", copy_icon));

    QIcon cut_icon;
    cut_icon.addFile(QString::fromUtf8(":/IconPrefix/images/cut.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionCut->setIcon(QtIconLoader::icon("edit-cut", cut_icon));

    QIcon paste_icon;
    paste_icon.addFile(QString::fromUtf8(":/IconPrefix/images/paste.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionPaste->setIcon(QtIconLoader::icon("edit-paste", paste_icon));

    QIcon undo_icon;
    undo_icon.addFile(QString::fromUtf8(":/IconPrefix/images/undo.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionUndo->setIcon(QtIconLoader::icon("edit-undo", undo_icon));

    QIcon redo_icon;
    redo_icon.addFile(QString::fromUtf8(":/IconPrefix/images/redo.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionRedo->setIcon(QtIconLoader::icon("edit-redo", redo_icon));

    QIcon find_icon;
    find_icon.addFile(QString::fromUtf8(":/IconPrefix/images/find.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionSearch->setIcon(QtIconLoader::icon("edit-find", find_icon));

    QIcon replace_icon;
    replace_icon.addFile(QString::fromUtf8(":/IconPrefix/images/replace.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionReplace->setIcon(QtIconLoader::icon("edit-find-replace", replace_icon));

    QIcon right_icon;
    right_icon.addFile(QString::fromUtf8(":/IconPrefix/images/right_blue.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionIndent->setIcon(QtIconLoader::icon("go-next", right_icon));

    QIcon left_icon;
    left_icon.addFile(QString::fromUtf8(":/IconPrefix/images/left_blue.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionUnindent->setIcon(QtIconLoader::icon("go-previous", left_icon));

    QIcon down_icon;
    down_icon.addFile(QString::fromUtf8(":/IconPrefix/images/down_blue.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionSend->setIcon(QtIconLoader::icon("go-down", down_icon));

    QIcon go_cursor_icon;
    go_cursor_icon.addFile(QString::fromUtf8(":/IconPrefix/images/go_cursor.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionSendToCursor->setIcon(QtIconLoader::icon("go-jump", go_cursor_icon));

    QIcon go_bottom_icon;
    go_bottom_icon.addFile(QString::fromUtf8(":/IconPrefix/images/go_bottom.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionSendAll->setIcon(QtIconLoader::icon("go-bottom", go_bottom_icon));

    QIcon up_icon;
    up_icon.addFile(QString::fromUtf8(":/IconPrefix/images/up_blue.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionUnsend->setIcon(QtIconLoader::icon("go-up", up_icon));

    QIcon go_top_icon;
    go_top_icon.addFile(QString::fromUtf8(":/IconPrefix/images/go_top.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionUnsendAll->setIcon(QtIconLoader::icon("go-top", go_top_icon));

    QIcon about_icon;
    about_icon.addFile(QString::fromUtf8(":/IconPrefix/images/info.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionAbout->setIcon(QtIconLoader::icon("help-about", about_icon));

    QIcon settings_icon;
    settings_icon.addFile(QString::fromUtf8(":/IconPrefix/images/settings.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionSettings->setIcon(QtIconLoader::icon("configure", settings_icon));

    QIcon indent_more_icon;
    indent_more_icon.addFile(QString::fromUtf8(":/IconPrefix/images/right_blue.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionIndent->setIcon(QtIconLoader::icon("format-indent-more", indent_more_icon));

    QIcon indent_less_icon;
    indent_less_icon.addFile(QString::fromUtf8(":/IconPrefix/images/right_blue.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionUnindent->setIcon(QtIconLoader::icon("format-indent-less", indent_less_icon));

#ifdef TERMINAL
    QIcon terminalIcon;
    terminalIcon.addFile(QString::fromUtf8(":/IconPrefix/images/terminal.svg"), QSize(), QIcon::Normal, QIcon::Off);
    m_actionRestartTerminal->setIcon(QtIconLoader::icon("utilities-terminal", terminalIcon));
#endif

    QIcon recentIcon;
    recentIcon.addFile(QString::fromUtf8(":/IconPrefix/images/recent.svg"), QSize(), QIcon::Normal, QIcon::Off);
    ui->menuRecent->setIcon(QtIconLoader::icon("document-open-recent", recentIcon));

    QIcon restartIcon;
    restartIcon.addFile(QString::fromUtf8(":/IconPrefix/images/refresh.svg"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionRestart->setIcon(QtIconLoader::icon("view-refresh", restartIcon));

    QIcon interruptIcon;
    interruptIcon.addFile(QString::fromUtf8(":/IconPrefix/images/stop.svg"), QSize(), QIcon::Normal, QIcon::Off);
    ui->actionInterrupt->setIcon(QtIconLoader::icon("process-stop", interruptIcon));
}

void MainWindow::refreshOpenedDocuments() {
    listOpenedDocuments->clear();

    listOpenedDocuments->addItems( m_tabManager->tabNameList() );
}

void MainWindow::insertText( QString text ) {
    m_tabManager->insert( text );
}

void MainWindow::addRecentFile( QString filePath ) {
    m_settings->mainWindowRecentFiles->addFile( filePath );
}
FileLink* MainWindow::lastRecentFile() {
    if( m_settings->mainWindowRecentFiles->isEmpty() )
        return new FileLink();
    else
        return m_settings->mainWindowRecentFiles->first();
}

void MainWindow::clearRecentMenu() {
  m_settings->mainWindowRecentFiles->clear();
  refreshRecentMenu();
}

void MainWindow::refreshRecentMenu() {
    QAction *actionClear;
    ui->menuRecent->clear();
    if( m_settings->mainWindowRecentFiles->size() == 0 )
      ui->menuRecent->setEnabled(false);
    else {
        ui->menuRecent->setEnabled(true);
        for( int i = 0; i < m_settings->mainWindowRecentFiles->size(); i++ ) {
            FileLink *f = m_settings->mainWindowRecentFiles->at(i);
            QAction *action = new QAction( this );
            action->setVisible( true );
            action->setText( f->name() );
            action->setData( f->absoluteFilePath() );
            action->setVisible(true);
            connect( action, SIGNAL(triggered()), this, SLOT(openFileByPath()) );
            ui->menuRecent->addAction( action );
        }
        ui->menuRecent->addSeparator();
        actionClear = ui->menuRecent->addAction(tr("Clear list"));
        connect( actionClear, SIGNAL(triggered()), this, SLOT(clearRecentMenu()));
    }
}

QWidget* MainWindow::tabManager() {
    return m_tabManager;
}

void MainWindow::displayDocName(const QString &c) {
    setWindowTitle("[*]" + c);
}

/***************************/
/*          Menus          */
/***************************/

/* File */
void MainWindow::on_actionNew_triggered() {
    m_tabManager->addNewTab();
}

void MainWindow::on_actionOpen_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open..."), lastRecentFile()->absolutePath(), tr("Coq Files (*.v *.V *.g);;All Files (*)"));
    if (fileName.isEmpty()) {
        //QMessageBox::critical(this, tr("Open file"), tr("Select a valid path"));
        return;
    }
    else {
        if (!QFile::exists(fileName)) {
            QMessageBox::critical(this, tr("Open file"), tr("This file doesn't exists"));
            return;
        }
        addRecentFile( fileName );
        m_tabManager->openFile(fileName);
        refreshRecentMenu();
    }
}

void MainWindow::on_actionClose_triggered() {
    actionClose_triggered();
}
void MainWindow::actionClose_triggered(int index) {
    m_tabManager->close(index);
}
void MainWindow::on_actionCloseAll_triggered() {
    m_tabManager->closeAll();
}
void MainWindow::on_actionSave_triggered() {
    m_tabManager->save(false);
}
void MainWindow::actionSave_triggered(int index) {
    m_tabManager->save(false, index);
}
void MainWindow::on_actionSaveAs_triggered() {
    m_tabManager->save(true);
}
void MainWindow::on_actionSaveAll_triggered() {
    m_tabManager->saveAll();
}

/* Edit */
void MainWindow::on_actionSelectAll_triggered() {
    m_tabManager->selectAll();
}
void MainWindow::on_actionCopy_triggered() {
    m_tabManager->copy();
}
void MainWindow::on_actionCut_triggered() {
    m_tabManager->cut();
}
void MainWindow::on_actionPaste_triggered() {
    m_tabManager->paste();
}
void MainWindow::on_actionUndo_triggered() {
    m_tabManager->undo();
}
void MainWindow::on_actionRedo_triggered() {
    m_tabManager->redo();
}
void MainWindow::on_actionSearch_triggered() {
    m_tabManager->search();
}
void MainWindow::on_actionSearchNext_triggered() {
    m_tabManager->searchNext();
}
void MainWindow::on_actionReplace_triggered() {
    m_tabManager->replace();
}
void MainWindow::on_actionGoToLine_triggered() {
    m_tabManager->goToLine();
}
void MainWindow::on_actionIndent_triggered() {
    m_tabManager->indent();
}
void MainWindow::on_actionUnindent_triggered() {
    m_tabManager->unindent();
}
void MainWindow::on_actionComment_triggered() {
    m_tabManager->comment();
}
void MainWindow::on_actionUncomment_triggered() {
    m_tabManager->uncomment();
}
void MainWindow::on_actionCommentLines_triggered() {
    m_tabManager->commentLines();
}
void MainWindow::on_actionUncommentLines_triggered() {
    m_tabManager->uncommentLines();
}
void MainWindow::on_actionSettings_triggered() {
    m_settings->showDialog();
}
void MainWindow::on_actionResetSettings_triggered() {
    m_settings->resetSettings();
}

/* Navigate */
void MainWindow::on_actionSend_triggered() {
    m_tabManager->next();
}
void MainWindow::on_actionUnsend_triggered() {
    m_tabManager->previous();
}
void MainWindow::on_actionUnsendAll_triggered() {
    m_tabManager->goToStart();
}
void MainWindow::on_actionSendAll_triggered() {
    m_tabManager->goToEnd();
}
void MainWindow::on_actionSendToCursor_triggered() {
    m_tabManager->goToCursor();
}
void MainWindow::on_actionInterrupt_triggered() {
    m_tabManager->interrupt();
}
void MainWindow::on_actionRestart_triggered() {
    m_tabManager->restart();
}

/* Display */
#ifdef TERMINAL
void MainWindow::actionTerminal_toggled(bool checked) {
    m_terminalDock->setHidden(!checked);
}
#endif

void MainWindow::on_actionOpenedDocuments_toggled(bool checked) {
    ui->dockOpenedDocuments->setHidden(!checked);
}
void MainWindow::on_actionFileBrowser_toggled(bool checked) {
    ui->dockFileBrowser->setHidden(!checked);
}
void MainWindow::on_actionDocumentationPanel_toggled(bool checked) {
    ui->dockDocumentation->setHidden(!checked);
}

/* Help */
void MainWindow::on_actionAbout_triggered() {
    QMessageBox::about(this, tr("About"), tr("<b>Coquille for Coq coding and learning</b><br />Made by the M1IF 2009-2010 of the ENS de Lyon"));
}

void MainWindow::on_actionDocumentation_triggered() {
    on_actionDocumentationPanel_toggled(true);
    ui->documentationPanel->showDocumentationAbout( m_tabManager->wordUnderCursor() );
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if( m_tabManager->closeAll() ) {
        writeSettings();
        event->accept();
    }
    else {
        event->ignore();
    }
}

void MainWindow::writeSettings() {
    m_settings->getSettingsFrom( this );
}

void MainWindow::readSettings() {
    m_settings->setSettingsTo( this );
}

#ifdef TERMINAL
void MainWindow::showTerminal( bool visible ) {
    m_actionTerminal->setChecked(visible);
}
void MainWindow::restartTerminal() {
    delete m_terminalDock->widget();

    QTermWidget *terminal = new QTermWidget();

//    QFont font = QApplication::font();
//    font.setFamily("Terminus");
//    font.setPointSize(11);
//
//    terminal->setTerminalFont(font);

    //console->setColorScheme(COLOR_SCHEME_BLACK_ON_LIGHT_YELLOW);

    terminal->setScrollBarPosition(QTermWidget::ScrollBarRight);

    m_terminalDock->setWidget( terminal );
}
#endif

void MainWindow::showFileBrowser( bool visible ) {
    ui->actionFileBrowser->setChecked(visible);
}
void MainWindow::showOpenedDocuments( bool visible ) {
    ui->actionOpenedDocuments->setChecked(visible);
}
void MainWindow::showDocumentationPanel( bool visible ) {
    qDebug() << "documentationpanel is visible : " << visible;
    ui->actionDocumentationPanel->setChecked( visible );
}

void MainWindow::openSpecificFile(const QModelIndex &index){

    QString path = dynamic_cast<QFileSystemModel*>( m_fileBrowser->model() )->filePath( index );
    if( dynamic_cast<QFileSystemModel*>( m_fileBrowser->model() )->isDir(index) )
        fileBrowserDrawPath( path );
    else
        m_tabManager->openFile(path);
}

void MainWindow::openFileByPath() {
    QAction *action = qobject_cast<QAction *>(sender());
     if( action )
         m_tabManager->openFile( action->data().toString() );
}

void MainWindow::changeCurrentDocument(const QModelIndex &index) {
    m_tabManager->setCurrentIndex(index.row());
}

void MainWindow::fileBrowserDrawPath( QString path ) {
    m_fileBrowser->setModel(0);

    QFileSystemModel *fileSystemModel = new QFileSystemModel();
    QStringList fileTypes;
    fileTypes << "*.v" ;
    fileSystemModel->setNameFilters(fileTypes);
    //fileSystemModel->setFilter(QDir::AllEntries);
    fileSystemModel->setRootPath( path );

    m_fileBrowser->setModel( fileSystemModel );
    m_fileBrowser->setRootIndex( fileSystemModel->index( path ) );

    connect( fileSystemModel, SIGNAL(rootPathChanged(QString)), this, SLOT(fileBrowserDrawPath(QString)) );
}

void MainWindow::goUpInFileBrowser() {
    QDir currentDir = QDir( dynamic_cast<QFileSystemModel*>( m_fileBrowser->model() )->rootPath() );
    if( currentDir.cdUp() )
        dynamic_cast<QFileSystemModel*>( m_fileBrowser->model() )->setRootPath( currentDir.absolutePath() );
}

QStringList MainWindow::readLines( QString fileName ) const {
    QFile file( fileName );

    if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
        qDebug() << tr( "Error in opening tactic list file" );
        return QStringList();
    }

    QString result = file.readAll();

    return result.split("\n");
}
