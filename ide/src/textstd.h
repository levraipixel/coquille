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

#ifndef TEXTSTD_H
#define TEXTSTD_H

/////////////////
//   Headers   //
/////////////////

//C++ Libs

//Qt Libs
#include <QWidget>
class QTextEdit;

//Project Libs
class QCodeEdit;
class QDocumentCursor;
class QFormatScheme;
class QGotoLinePanel;
class QLanguageFactory;
class QPanel;

//Project Files
class Answer;
class Coqtop;
class Settings;
class TextEditor;

//File Classes
class TextStd;

//File NameSpace
namespace Ui {
    class TextStd;
}

/////////////////
//   TextStd   //
/////////////////

/*!
\brief a Coq Editor Widget

A TextStd is a widget containing :
<ul>
    <li>1 Coqtop
    <li>1 TextEditor for the input</li>
    <li>2 QTextEdit for the output</li>
    <li>
</ul>
*/
class TextStd : public QWidget {
    Q_OBJECT

public:
    explicit TextStd( QWidget *parent, Settings *settings );
    virtual ~TextStd();

    /*! Return the input TextEditor */
    TextEditor* textStdIn();

    /*!
    \brief Expand all the necessary for a line to be seen

    A line can be in a folded block, which can be itself in another block, ...

    \param line The line to be seen
    */
    void showLine( const int line );
    /*! Return the word written at the current cursor position */
    QString wordUnderCursor();

    /*! \brief Create a DocumentSendCommand to execute a "Send" action, and executes it. */
    void next( bool limited = false );
    /*!
    \brief Send the next valid sentence to Coq.

    Search a valid sentence, send it to Coq, evaluate and display the answer.

    \param limited If true, search only before the current cursor position
    */
    void sendNext( bool limited = false );
    /*! \brief Create a DocumentSendCommand to execute an "Unsend" action, and executes it. */
    void previous();
    /*!
    \brief Unsend the previous sentence sent to Coq.

    Update the current document.

    \param undo If true, really "Undo" in Coq
    */
    void sendPrevious( bool undo = true );
    /*! \brief Unsend everything. */
    void goToStart();
    /*! \brief Send everything. */
    void goToEnd();
    /*! \brief Send everything before the current cursor position */
    void goToCursor();

    /*! \brief Interrupt the computing of Coq. */
    void interrupt();
    /*! \brief Restart Coq and the input TextEditor. */
    void restart();

    /*! \brief Copy the selected text, if any. */
    void copy();
    /*! \brief Cut the selected text, if any. */
    void cut();
    /*! \brief Paste text from the clipboard to the current cursor position. */
    void paste();
    /*! \brief Select the whole text. */
    void selectAll();
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
    void indentSelection();
    /*! \brief Unindent the selection. */
    void unindentSelection();
    /*! \brief Comment the selection, if any. */
    void commentSelection();
    /*! \brief Uncomment the selection, if any. */
    void uncommentSelection();
    /*! \brief Comment the selected lines, if any. */
    void commentLines();
    /*! \brief Uncomment the selected lines, if any. */
    void uncommentLines();
    /*! \brief Insert some text at the current cursor position. */
    void insert(QString text);

signals:
    void contentModified(bool y);

protected:
    void expandLine( const int line );

    void addFormatOverlay( const int formatId, const int lineIndex, const int columnStart = 0, const int columnEnd = -1 );
    void addFormatOverlay( const int formatId, QDocumentCursor start, QDocumentCursor end );
    void addSentOverlay( const int lineIndex, const int columnStart = 0, const int columnEnd = -1 );
    void addSentOverlay( QDocumentCursor start, QDocumentCursor end );
    void addValidOverlay( const int lineIndex, const int columnStart = 0, const int columnEnd = -1 );
    void addValidOverlay( QDocumentCursor start, QDocumentCursor end );
    void removeFormatOverlay( const int formatId, const int lineIndex, const int columnStart = 0 );
    void removeFormatOverlay( const int formatId, QDocumentCursor start );
    void removeFormatOverlay( const int formatId, QDocumentCursor start, QDocumentCursor end );
    void removeSentOverlay( const int lineIndex, const int columnStart = 0 );
    void removeSentOverlay( QDocumentCursor start );
    void removeSentOverlay( QDocumentCursor start, QDocumentCursor end );
    void removeValidOverlay( const int lineIndex, const int columnStart = 0 );
    void removeValidOverlay( QDocumentCursor start );
    void removeValidOverlay( QDocumentCursor start, QDocumentCursor end );
    void removeAllOverlays();

    void fillStdIn( QString text );

    QString displayUnicode( const QString &text );
    QString removePromptsFrom( QString text );

    void displayStdOut( QString text );
    void displayStdErr( QString text );
    void displayAnswer( Answer *answer );

    virtual void changeEvent(QEvent *e);

    QString pathToRoot;
    Coqtop *m_coqtop;
    Ui::TextStd *m_ui;
    Settings *m_settings;
    QCodeEdit *m_editControl;
    QPanel *m_lineNumberPanel;
    QPanel *m_foldPanel;
    QPanel *m_lineChangePanel;
    QPanel *m_statusPanel;
    QPanel *m_searchReplacePanel;
    QGotoLinePanel *m_gotoLinePanel;
    QFormatScheme *m_formats;
    QLanguageFactory *m_languages;
};

#endif // TEXTSTD_H
