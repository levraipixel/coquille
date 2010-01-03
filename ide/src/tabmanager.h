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

/*!
\brief A better tab manager

It manages the currently opened tabs, ans provides ways to navigate in them.
It is also the interface used by the MainWindow class to trigger the actions of the documents, such as copy, cut, ...
*/
class TabManager : public QWidget {
    Q_OBJECT
public:
    TabManager( QWidget *parent, Settings *settings );
    ~TabManager();

    /*! returns true if there is an opened document */
    bool hasOpenedDocuments();
    /*! returns the list of currently opened documents */
    FilesLinkList *openedDocuments();
    /*! returns the list of names of the tabs */
    QStringList tabNameList();

    /*! returns the word written under the cursor in the current opened document */
    QString wordUnderCursor();

    /*! \brief open a new tab. */
    void addNewTab();

    /*!
        \brief Opens a file

        Creates a new tab and opens a given file in it
        \param fileName path to open
    */
    void openFile(const QString& fileName);
    /*!
        \brief Save a document

        \param forceNameChanging ask for a new file name
        \param index tab index of the document
    */
    void save(bool forceNameChanging = false, int index = -1);
    /*! \brief Save all documents. */
    void saveAll();
    /*!
        \brief Try to close a document

        Returns true if succeeded.

        \param addTab add a new tab if this is the only document
        \param index tab index of the document
    */
    bool close( bool addTab, int index = -1 );
    /*!
        \brief Try to close all documents.

        Returns true if succeeded.
    */
    bool closeAll();
    /*! \brief Select the whole text. */
    void selectAll();
    /*! \brief Copy the selected text, if any. */
    void copy();
    /*! \brief Cut the selected text, if any. */
    void cut();
    /*! \brief Paste text from the clipboard to the current cursor position. */
    void paste();
    /*! \brief Undo the last editing operation, if any on the stack. */
    void undo();
    /*! \brief Redo the last undone editing operation, if any on the stack. */
    void redo();
    /*! \brief Show the search/replace panel. */
    void search();
    /*! \brief Ask the search/replace panel to move to next match. */
    void searchNext();
    /*! \brief Replace the found text by another. */
    void replace();
    /*! \brief Show a dialog to go to a specific line. */
    void goToLine();
    /*! \brief Indent the selection. */
    void indent();
    /*! \brief Unindent the selection. */
    void unindent();
    /*! \brief Comment the selection, if any. */
    void comment();
    /*! \brief Uncomment the selection, if any. */
    void uncomment();
    /*! \brief Comment the selected lines, if any. */
    void commentLines();
    /*! \brief Uncomment the selected lines, if any. */
    void uncommentLines();
    /*! \brief Insert some text at the current cursor position. */
    void insert(QString text);
    /*! \brief Send the next sentence to Coq. */
    void next();
    /*! \brief Unsend the last sentence sent to Coq. */
    void previous();
    /*! \brief Unsend all sentences sent to Coq. */
    void goToStart();
    /*! \brief Send all sentences in the document to Coq. */
    void goToEnd();
    /*! \brief Send all sentences before the current cursor position to Coq. */
    void goToCursor();
    /*! \brief Interrupt the computing of Coq. */
    void interrupt();
    /*! \brief Restart Coq. */
    void restart();
    /*! \brief Set the tab to be shown. */
    void setCurrentIndex(int index);

signals:
    void displayDocName(QString); // name
    void contentModified(bool); // is modified
    void tabOpen(QString, int); // name, index
    void tabClose(QString, int); // name, index
    void tabNameChange(QString, int); //new name, index

protected slots:
    void emitDocName(int index = -1);
    void onDocumentModified(bool b);
    bool closeTab( int index = -1 );

protected:
    void closeWithoutSave( bool addTab, int index = -1 );
    void load(const QString& fileName, int index = -1);

    void changeEvent(QEvent *e);

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
