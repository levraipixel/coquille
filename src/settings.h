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

/*! \brief An object for configuration

A Settings object is accessible from everywhere.

It is convenient to configure several objects.
*/
class Settings {
public:
    /*! Erase the configuration avec reads the configuration file */
    Settings();
    ~Settings();
    /*! associates this with a MainWindow */
    void setMainWindow( MainWindow *mainWindow );
    /*! returns the list names of actions whose shortcuts are modifiable by user */
    QStringList mainWindowModifiableShortcuts();
    /*! returns the text of the action */
    QString getActionText( QString actionName );

    /*! reads the configuration file */
    void readSettingsFile();
    /*! writes the configuration file */
    void writeSettingsFile();
    /*! show the dialog box to modify settings */
    void showDialog();
    /*! reset to default configuration */
    void resetSettings();
    /*! reads the configuration of mainWindow */
    void getSettingsFrom( MainWindow *mainWindow );
    /*! configures mainWindow */
    void setSettingsTo( MainWindow *mainWindow );

    //MainWindow
    /*! The MainWindow associated with this Settings object */
    MainWindow *m_mainWindow;

    /*! The size of the mainWindow */
    QSize mainWindowSize;
    /*! The position of the mainWindow */
    QPoint mainWindowPosition;
    /*! If the mainWindow has to be seen maximized */
    bool mainWindowMaximized;
    /*! The font of the mainWindow */
    QFont mainWindowFont;
#ifdef TERMINAL
    bool mainWindowShowTerminal;
#endif
    /*! If the "Opened documents" pannel has to be shown */
    bool mainWindowShowOpenedDocuments;
    /*! If the "File Browser" pannel has to be shown */
    bool mainWindowShowFileBrowser;
    /*! If the "Documentation" pannel has to be shown */
    bool mainWindowShowDocumentationPanel;
    /*! The size of the "Recent Files" history */
    int mainWindowRecentFilesSize;
    /*! The history */
    FilesLinkList *mainWindowRecentFiles;
    /*! All the shortcuted actions of MainWindow, containing for each the name and if it is modifiable by the user */
    QHash<QString, bool> mainWindowShortcutedActions;
    /*! All the shortcuts currently used for MainWindow */
    QHash<QString, QKeySequence> mainWindowShortcuts;

    //TextStd
    /*! If the cursor has to follow the "Send" action */
    bool textStdScrollCursorOnSend;
    /*! If the "Line Number" pannel has to be shown */
    bool textStdShowLineNumberPanel;
    /*! If the "Line Change" pannel has to be shown */
    bool textStdShowLineChangePanel;
    /*! If the "Search / Replace" pannel has to be shown */
    bool textStdShowSearchReplacePanel;
    /*! If the "Code Folding" pannel has to be shown */
    bool textStdShowFoldPanel;
    /*! If the "Status" pannel has to be shown */
    bool textStdShowStatusPanel;
    /*! If the answers of Coq have to be seen in LaTeX mode */
    bool textStdLatexMode;

protected:
    void setShortcut( QString actionName, QKeySequence shortcut = QKeySequence(), bool modifiable = false );
    void resetToDefaultValues();
    void getSettingsFrom( TextStd *textStd );
    void setSettingsTo( TextStd *textStd );
};

#endif // SETTINGS_H
