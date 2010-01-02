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

class TextStd : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(TextStd)

public:
    explicit TextStd( QWidget *parent, Settings *settings );
    virtual ~TextStd();

    TextEditor* textStdIn();
    QTextEdit* textStdOut();
    QTextEdit* textStdErr();

    void expandLine( const int line );
    void showLine( const int line );

    Coqtop *coqtop();
    QString wordUnderCursor();

    void next( bool limited = false );
    void sendNext( bool limited = false );
    void validateNext( bool isValid );
    void previous();
    void sendPrevious( bool undo = true );

    void goToStart();
    void goToEnd();
    void goToCursor();

    void interrupt();
    void restart();

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
public slots:
    void displayStdOut( QString text );
    void displayStdErr( QString text );
    void displayAnswer( Answer *answer );
    void copy();
    void cut();
    void paste();
    void selectAll();
    void undo();
    void redo();
    void search();
    void searchNext();
    void replace();
    void goToLine();
    void indentSelection();
    void unindentSelection();
    void commentSelection();
    void uncommentSelection();
    void commentLines();
    void uncommentLines();
    void insert(QString text);
signals:
    void displayStatusBar( QString text );
    void contentModified(bool y);

protected:
    QString pathToRoot;
    virtual void changeEvent(QEvent *e);
    Coqtop *m_coqtop;

private:
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
