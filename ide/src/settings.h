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

#ifndef SETTINGS_H
#define SETTINGS_H

/////////////////
//   Headers   //
/////////////////

//C++ Libs

//Qt Libs

//Project Libs

//Project Files
class FilesLinkList;
class MainWindow;
class TextStd;

//File Classes
class Settings;

//File NameSpace

//////////////////
//   Settings   //
//////////////////

class Settings {
public:
    Settings();
    ~Settings();
    void setMainWindow( MainWindow *mainWindow );
    QStringList mainWindowModifiableShortcuts();
    QString getActionText( QString actionName );

    void setShortcut( QString actionName, QKeySequence shortcut = QKeySequence(), bool modifiable = false );
    void resetToDefaultValues();
    void resetSettings();
    void getSettingsFrom( MainWindow *mainWindow );
    void getSettingsFrom( TextStd *textStd );
    void setSettingsTo( MainWindow *mainWindow );
    void setSettingsTo( TextStd *textStd );

    void readSettingsFile();
    void writeSettingsFile();

    void showDialog();

    //MainWindow
    MainWindow *m_mainWindow;

    QSize mainWindowSize;
    QPoint mainWindowPosition;
    bool mainWindowMaximized;
    QFont mainWindowFont;
#ifdef TERMINAL
    bool mainWindowShowTerminal;
#endif
    bool mainWindowShowOpenedDocuments;
    bool mainWindowShowFileBrowser;
    bool mainWindowShowDocumentationPanel;
    int mainWindowRecentFilesSize;
    FilesLinkList *mainWindowRecentFiles;
    QHash<QString, bool> mainWindowShortcutedActions;
    QHash<QString, QKeySequence> mainWindowShortcuts;

    //TextStd
    bool textStdScrollCursorOnSend;
    bool textStdShowLineNumberPanel;
    bool textStdShowLineChangePanel;
    bool textStdShowSearchReplacePanel;
    bool textStdShowFoldPanel;
    bool textStdShowStatusPanel;
    bool textStdLatexMode;
};

#endif // SETTINGS_H
