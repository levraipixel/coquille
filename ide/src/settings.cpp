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
#include <QMessageBox>
#include <QSettings>

//Project Libs

//Project Files
#include "filelink.h"
#include "mainwindow.h"
#include "settings.h"
#include "settingsdialog.h"
#include "ui_mainwindow.h"

//////////////////
//   Settings   //
//////////////////

Settings::Settings() {
    resetToDefaultValues();
    readSettingsFile();
}

Settings::~Settings() {
    writeSettingsFile();
}

void Settings::setMainWindow( MainWindow *mainWindow ) {
    m_mainWindow = mainWindow;
}

QStringList Settings::mainWindowModifiableShortcuts() {
    return mainWindowShortcutedActions.keys( true );
}

QString Settings::getActionText( QString actionName ) {
    return m_mainWindow->getAction( actionName )->text();
}

void Settings::resetSettings() {
    QSettings settings;
    switch( QMessageBox::question(0, QObject::tr( "Reset settings" ), QObject::tr( "Are you sure you want to reset all settings ?" ), QMessageBox::No | QMessageBox::Yes ) ) {
        case QMessageBox::Yes:
            settings.clear();
            resetToDefaultValues();
            writeSettingsFile();
            setSettingsTo( m_mainWindow );
            break;
        default:
            break;
    }
}

void Settings::resetToDefaultValues() {
    mainWindowSize = QSize(1000, 600);
    mainWindowPosition = QPoint(200, 200);
    mainWindowMaximized = false;
    mainWindowFont = QFont();
#ifdef TERMINAL
    mainWindowShowTerminal = false;
#endif
    mainWindowShowOpenedDocuments = false;
    mainWindowShowFileBrowser = false;
    mainWindowShowDocumentationPanel = false;
    mainWindowRecentFilesSize = 10;
    mainWindowRecentFiles = new FilesLinkList( mainWindowRecentFilesSize );
    mainWindowShortcutedActions.clear();
    mainWindowShortcuts.clear();
    setShortcut( "about" );
    setShortcut( "close", QKeySequence( "Ctrl+W" ) );
    setShortcut( "closeAll", QKeySequence( "Ctrl+Alt+W" ) );
    setShortcut( "comment", QKeySequence( "Ctrl+*" ) );
    setShortcut( "commentLines" );
    setShortcut( "copy", QKeySequence( "Ctrl+C" ) );
    setShortcut( "cut", QKeySequence( "Ctrl+X" ) );
    setShortcut( "documentation", QKeySequence( "F1" ) );
    setShortcut( "documentationPanel", QKeySequence( "Meta+Ctrl+F1" ) );
    setShortcut( "fileBrowser", QKeySequence( "Meta+Ctrl+F10" ) );
    setShortcut( "goToLine", QKeySequence( "Ctrl+L" ) );
    setShortcut( "indent", QKeySequence( "Ctrl+I" ) );
    setShortcut( "new", QKeySequence( "Ctrl+N" ) );
    setShortcut( "open", QKeySequence( "Ctrl+O" ) );
    setShortcut( "openedDocuments", QKeySequence( "Meta+Ctrl+F11" ) );
    setShortcut( "paste", QKeySequence( "Ctrl+V" ) );
    setShortcut( "quit", QKeySequence( "Ctrl+Q" ) );
    setShortcut( "redo", QKeySequence( "Ctrl+Shift+Z" ), true );
    setShortcut( "replace", QKeySequence( "Ctrl+H" ) );
#ifdef TERMINAL
    setShortcut( "restartTerminal", QKeySequence( "Meta+F12" ) );
#endif
    setShortcut( "save", QKeySequence( "Ctrl+S" ) );
    setShortcut( "saveAll", QKeySequence( "Ctrl+Alt+S" ) );
    setShortcut( "saveAs", QKeySequence( "Ctrl+Shift+S" ) );
    setShortcut( "search", QKeySequence( "Ctrl+F" ) );
    setShortcut( "searchNext", QKeySequence( "F3" ) );
    setShortcut( "selectAll", QKeySequence( "Ctrl+A" ) );
    setShortcut( "send", QKeySequence( "Meta+Ctrl+Down" ), true );
    setShortcut( "sendAll", QKeySequence( "Meta+Ctrl+PgDown" ), true );
    setShortcut( "sendToCursor", QKeySequence( "Meta+Ctrl+Space" ), true );
#ifdef TERMINAL
    setShortcut( "terminal", QKeySequence( "Meta+Ctrl+F12" ) );
#endif
    setShortcut( "uncomment", QKeySequence( "Ctrl+Shift+*" ) );
    setShortcut( "uncommentLines" );
    setShortcut( "undo", QKeySequence( "Ctrl+Z" ), true );
    setShortcut( "unindent", QKeySequence( "Ctrl+Shift+I" ) );
    setShortcut( "unsend", QKeySequence( "Meta+Ctrl+Up" ), true );
    setShortcut( "unsendAll", QKeySequence( "Meta+Ctrl+PgUp" ), true );

    textStdScrollCursorOnSend = true;
    textStdLatexMode = true;
}

void Settings::setShortcut( QString actionName, QKeySequence shortcut, bool modifiable ) {
    mainWindowShortcutedActions.insert( actionName, modifiable );
    mainWindowShortcuts.insert( actionName, shortcut );
}

void Settings::getSettingsFrom( MainWindow *mainWindow ) {
    mainWindowSize = mainWindow->size();
    mainWindowPosition = mainWindow->pos();
    mainWindowMaximized = mainWindow->isMaximized();
    mainWindowFont.setFamily( mainWindow->fontInfo().family() );
    mainWindowFont.setPixelSize( mainWindow->fontInfo().pixelSize() );
    mainWindowFont.setBold( mainWindow->fontInfo().bold() );
    mainWindowFont.setItalic( mainWindow->fontInfo().italic() );
#ifdef TERMINAL
    mainWindowShowTerminal = mainWindow->getAction( "terminal" )->isChecked();
#endif
    mainWindowShowOpenedDocuments = mainWindow->getAction( "openedDocuments" )->isChecked();
    mainWindowShowFileBrowser = mainWindow->getAction( "fileBrowser" )->isChecked();
    mainWindowShowDocumentationPanel = mainWindow->getAction( "documentationPanel" )->isChecked();

    mainWindowShortcuts.clear();
    QAction *action;
    for( QHash<QString, bool>::iterator shortcutedAction = mainWindowShortcutedActions.begin(); shortcutedAction != mainWindowShortcutedActions.end(); shortcutedAction++ ) {
        if( action = mainWindow->getAction( shortcutedAction.key() ) )
            mainWindowShortcuts.insert( shortcutedAction.key(), action->shortcut() );
    }
}

void Settings::setSettingsTo( MainWindow *mainWindow ) {
    mainWindow->resize( mainWindowSize );
    mainWindow->move( mainWindowPosition );
    if( mainWindowMaximized )
        mainWindow->showMaximized();
    mainWindow->setFont( mainWindowFont );
#ifdef TERMINAL
    mainWindow->showTerminal( mainWindowShowTerminal );
#endif
    mainWindow->showOpenedDocuments( mainWindowShowOpenedDocuments );
    mainWindow->showFileBrowser( mainWindowShowFileBrowser );

    mainWindow->showDocumentationPanel( mainWindowShowDocumentationPanel );
    QAction *action;
    for( QHash<QString, bool>::iterator shortcutedAction = mainWindowShortcutedActions.begin(); shortcutedAction != mainWindowShortcutedActions.end(); shortcutedAction++ ) {
        if( action = mainWindow->getAction( shortcutedAction.key() ) )
            action->setShortcut( mainWindowShortcuts.value( shortcutedAction.key() ) );
    }
}

void Settings::getSettingsFrom( TextStd *textStd ) {
}

void Settings::setSettingsTo( TextStd *textStd ) {
}

void Settings::readSettingsFile() {
    QSettings settings;

    settings.beginGroup( "MainWindow" );

    mainWindowSize = settings.value( "size", mainWindowSize ).toSize();
    mainWindowPosition = settings.value( "pos", mainWindowPosition ).toPoint();
    mainWindowMaximized = settings.value( "maximized", mainWindowMaximized ).toBool();
    QFontInfo fontInfo( mainWindowFont );
    mainWindowFont.setFamily( settings.value( "fontFamily", fontInfo.family() ).toString() );
    mainWindowFont.setPixelSize( settings.value( "fontSize", fontInfo.pixelSize() ).toInt() );
    mainWindowFont.setBold( settings.value( "fontBold", fontInfo.bold() ).toBool() );
    mainWindowFont.setItalic( settings.value( "fontItalic", fontInfo.italic() ).toBool() );

#ifdef TERMINAL
    mainWindowShowTerminal = settings.value( "showTerminal", mainWindowShowTerminal ).toBool();
#endif
    mainWindowShowOpenedDocuments = settings.value( "showOpenedDocuments", mainWindowShowOpenedDocuments ).toBool();
    mainWindowShowFileBrowser = settings.value( "showFileBrowser", mainWindowShowFileBrowser ).toBool();
    mainWindowShowDocumentationPanel = settings.value( "showDocumentationPanel", mainWindowShowDocumentationPanel ).toBool();

    mainWindowRecentFilesSize = settings.value( "recentFilesSize", mainWindowRecentFilesSize ).toInt();
    mainWindowRecentFiles = new FilesLinkList( mainWindowRecentFilesSize );
    QStringList list( settings.value( "recentFiles" ).toStringList() );
    for( int i = list.size()-1; i >= 0; i-- )
        mainWindowRecentFiles->addFile( list.at( i ) );

    mainWindowShortcuts.reserve( mainWindowShortcutedActions.size() );
    for( QHash<QString, bool>::iterator shortcutedAction = mainWindowShortcutedActions.begin(); shortcutedAction != mainWindowShortcutedActions.end(); shortcutedAction++ ) {
        QKeySequence before( mainWindowShortcuts.value( shortcutedAction.key() ) );
        mainWindowShortcuts.remove( shortcutedAction.key() );
        mainWindowShortcuts.insert(
            shortcutedAction.key(),
            settings.value( shortcutedAction.key(), before ).toString()
        );
    }

    settings.endGroup();

    settings.beginGroup( "TextStd" );

    textStdScrollCursorOnSend = settings.value( "scrollCursorOnSend", textStdScrollCursorOnSend ).toBool();
    textStdLatexMode = settings.value( "latexMode", textStdLatexMode ).toBool();

    settings.endGroup();
}

void Settings::writeSettingsFile() {
    QSettings settings;

    settings.beginGroup( "MainWindow" );

    settings.setValue( "size", mainWindowSize );
    settings.setValue( "pos", mainWindowPosition );
    settings.setValue( "maximized", mainWindowMaximized );
    QFontInfo fontInfo( mainWindowFont );
    settings.setValue( "fontFamily", fontInfo.family() );
    settings.setValue( "fontSize", fontInfo.pixelSize() );
    settings.setValue( "fontBold", fontInfo.bold() );
    settings.setValue( "fontItalic", fontInfo.italic() );

#ifdef TERMINAL
    settings.setValue( "showTerminal", mainWindowShowTerminal );
#endif
    settings.setValue( "showOpenedDocuments", mainWindowShowOpenedDocuments );
    settings.setValue( "showFileBrowser", mainWindowShowFileBrowser );
    settings.setValue( "showDocumentationPanel", mainWindowShowDocumentationPanel );

    settings.setValue( "recentFilesSize", mainWindowRecentFilesSize );
    settings.setValue( "recentFiles", QVariant( mainWindowRecentFiles->toPathList() ) );
    for( int i = 0; i < mainWindowShortcuts.keys().size(); i++ )
        settings.setValue( mainWindowShortcuts.keys().at(i), mainWindowShortcuts.value( mainWindowShortcuts.keys().at(i) ) );

    settings.endGroup();

    settings.beginGroup( "TextStd" );

    settings.setValue( "scrollCursorOnSend", textStdScrollCursorOnSend );
    settings.setValue( "latexMode", textStdLatexMode );

    settings.endGroup();
}

void Settings::showDialog() {
    SettingsDialog dialog( this );
    dialog.exec();
    setSettingsTo( m_mainWindow );
}
