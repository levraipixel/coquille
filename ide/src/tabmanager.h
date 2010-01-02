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

#ifndef TABMANAGER_H
#define TABMANAGER_H

/////////////////
//   Headers   //
/////////////////

//C++ Libs

//Qt Libs
#include <QWidget>
class QEditor;
class QTabWidget;

//Project Libs

//Project Files
class FilesLinkList;
class Settings;
class TextStd;

//File Classes
class TabManager;

//File NameSpace
namespace Ui {
    class TabManager;
}

////////////////////
//   TabManager   //
////////////////////

class TabManager : public QWidget {
    Q_OBJECT
public:
    TabManager( QWidget *parent, Settings *settings );
    ~TabManager();

    bool hasOpenedDocuments();
    FilesLinkList *openedDocuments();
    QStringList tabNameList();
    QString wordUnderCursor();

public slots:
    void addNewTab();
    void openFile(const QString& fileName);
    void load(const QString& fileName, int index = -1);
    void save(bool forceNameChanging = false, int index = -1);
    void saveAll();
    void closeWithoutSave( bool addTab, int index = -1 );
    bool closeTab( int index = -1 );
    bool close( bool addTab, int index = -1 );
    bool closeAll();
    void focusOn( const QString& title );

    void selectAll();
    void copy();
    void cut();
    void paste();
    void undo();
    void redo();
    void search();
    void searchNext();
    void replace();
    void goToLine();
    void indent();
    void unindent();
    void comment();
    void uncomment();
    void commentLines();
    void uncommentLines();
    void insert(QString text);

    void next();
    void previous();
    void goToStart();
    void goToEnd();
    void goToCursor();
    void interrupt();
    void restart();

    void setCurrentIndex(int index);

private slots:
    void emitDocName(int index = -1);
    void onDocumentModified(bool b);

signals:
    void displayStatusBar(QString);
    void displayDocName(QString);
    void contentModified(bool b);
    void signalStartDebug( QString label, QString content );
    void tabOpen(QString, int); // name, index
    void tabClose(QString, int); // name, index
    void tabNameChange(QString, int); //new name, index

protected:
    void changeEvent(QEvent *e);

private:
    int addTab(const QString& name = "");
    int trueIndex(int index = -1);
    bool rename(QEditor* editor);
    TextStd* textStdOf(int index = -1);
    TextStd* currentTextStd();
    void setTabText(int index, const QString& text);

    Ui::TabManager *m_ui;
    Settings *m_settings;
    QTabWidget* tabWidget;
};

#endif // TABMANAGER_H
