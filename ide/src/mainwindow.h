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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/////////////////
//   Headers   //
/////////////////

//C++ Libs

//Qt Libs
#include <QHash>
#include <QMainWindow>
class QAbstractItemView;
class QDebug;
class QFileInfo;
class QFileSystemModel;
class QListWidget;
class QModelIndex;
class QTreeView;
class QComboBox;

//Project Libs

//Project Files
class FileLink;
class Settings;
class TabManager;

//File Classes
class MainWindow;

//File NameSpace
namespace Ui {
    class MainWindowClass;
}

////////////////////
//   MainWindow   //
////////////////////


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(Settings *settings);
    ~MainWindow();
    void setActions();
    void setIcons();

    void closeEvent(QCloseEvent *event);
    void writeSettings();
    void readSettings();

    void addRecentFile( QString fileName );
    FileLink* lastRecentFile();
    void refreshRecentMenu();
    QWidget* tabManager();
    QAction* getAction( QString name );
    QList<QAction*> actions();

public slots:
#ifdef TERMINAL
    void showTerminal( bool visible );
    void restartTerminal();
#endif
    void showFileBrowser( bool visible );
    void showOpenedDocuments( bool visible );
    void showDocumentationPanel( bool visible );
    void displayDocName(const QString &c);
    void openFileByPath();
    void refreshOpenedDocuments();
    void insertText( QString text );

    /***************************/
    /*          Menus          */
    /***************************/

    /* File */
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionClose_triggered();
    void actionClose_triggered(int index = -1);
    void on_actionCloseAll_triggered();
    void on_actionSave_triggered();
    void actionSave_triggered(int index = -1);
    void on_actionSaveAs_triggered();
    void on_actionSaveAll_triggered();
    void clearRecentMenu();

    /* Edit */
    void on_actionSelectAll_triggered();
    void on_actionCopy_triggered();
    void on_actionCut_triggered();
    void on_actionPaste_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionSearch_triggered();
    void on_actionSearchNext_triggered();
    void on_actionReplace_triggered();
    void on_actionGoToLine_triggered();
    void on_actionIndent_triggered();
    void on_actionUnindent_triggered();
    void on_actionComment_triggered();
    void on_actionUncomment_triggered();
    void on_actionCommentLines_triggered();
    void on_actionUncommentLines_triggered();
    void on_actionSettings_triggered();
    void on_actionResetSettings_triggered();

    /* Navigate */
    void on_actionSend_triggered();
    void on_actionSendAll_triggered();
    void on_actionSendToCursor_triggered();
    void on_actionUnsend_triggered();
    void on_actionUnsendAll_triggered();
    void on_actionInterrupt_triggered();
    void on_actionRestart_triggered();

    /* Display */
#ifdef TERMINAL
    void actionTerminal_toggled(bool checked);
#endif
    void on_actionOpenedDocuments_toggled(bool checked);
    void on_actionFileBrowser_toggled(bool checked);
    void on_actionDocumentationPanel_toggled(bool checked);

    /* ? */
    void on_actionAbout_triggered();
    void on_actionDocumentation_triggered();

private slots:
    void openSpecificFile(const QModelIndex &index);
    void changeCurrentDocument(const QModelIndex &index);
    void goUpInFileBrowser();
    void fileBrowserDrawPath( QString path );

private:
    QStringList readLines( QString fileName ) const;


    Ui::MainWindowClass *ui;
    TabManager *m_tabManager;
    Settings *m_settings;
    QHash<QString, QAction*> m_actions;
    QTreeView *m_fileBrowser;
    QListWidget *listOpenedDocuments;
    QComboBox *tactics;
#ifdef TERMINAL
    QDockWidget *m_terminalDock;
    QAction *m_actionTerminal;
    QAction *m_actionRestartTerminal;
#endif
};

#endif // MAINWINDOW_H
