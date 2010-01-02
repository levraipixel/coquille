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
#include <QDebug>
#include <QKeyEvent>

//Project Libs
#include "qdocumentline.h"

//Project Files
#include "texteditor.h"
#include "textstd.h"

////////////////////
//   TextEditor   //
////////////////////

TextEditor::TextEditor(TextStd *p) :
        QEditor(false, p)
{
    textStd = p;
    sentCursors.clear();
    setWindowTitle("[*]");
}

QDocumentCursor TextEditor::lastSentCursor() const {
    return sentCursors.last();
}

QDocumentCursor TextEditor::limitCursor() const {
    if( hasSentCursors() )
        return lastSentCursor().selectionEnd();
    else {
        QDocumentCursor result = cursor();
        result.moveTo(0,0);
        return result;
    }
}

void TextEditor::sendCursor( QDocumentCursor c ) {
    sentCursors.append( c );
}

void TextEditor::restorePreviousCursor() {
    sentCursors.removeLast();
}

bool TextEditor::hasSentCursors() const {
    return !sentCursors.isEmpty();
}

void TextEditor::restart() {
    sentCursors.clear();
}

//search the next '.' and sets the document cursor to the matched sentence. returns true if something is found
QDocumentCursor TextEditor::searchPhraseToSend( bool limited ) {
//    qDebug() << "is limited : " << limited;
    bool onComment = false;
    bool onString = false;
    QDocumentCursor result = limitCursor();
    while( !result.atEnd() && ( !limited || result < cursor() ) && ( onComment || onString || ( result.nextChar() != QChar('.') ) ) ) {
        if( onString ) {
            if( result.nextChar() == QChar('"') )
                onString = false;
        }
        else {
            if( onComment ) {
                if( ( result.previousChar() == QChar('*') ) && ( result.nextChar() == QChar(')') ) )
                    onComment = false;
            }
            else {
                if( result.nextChar() == QChar('"') )
                    onString = true;
                else if( ( result.previousChar() == QChar('(') ) && ( result.nextChar() == QChar('*') ) )
                    onComment = true;
            }
        }
        if( result.atLineEnd() )
           textStd->showLine( result.lineNumber() + 1 );
        result.movePosition(1, QDocumentCursor::NextCharacter, QDocumentCursor::KeepAnchor);
    }
    if( result.atEnd() || ( limited && result >= cursor() ) )
        return QDocumentCursor();
    else {
        result.movePosition(1, QDocumentCursor::NextCharacter, QDocumentCursor::KeepAnchor);
        return result;
    }
}

bool TextEditor::protectedCursor(const QDocumentCursor& c) const {
    //standard verification (inhereted)
    if( QEditor::protectedCursor(c) )
        return true;

    //our verification
    if( hasSentCursors() && ( c.selectionStart() < limitCursor() ) )
        return true;

    return false;
}

bool TextEditor::processCursor(QDocumentCursor& c, QKeyEvent *e, bool& b) {
    //our verification : backspace is forbidden before and at the sent cursor, except at the beginning of the document
    if( ( e->key() == Qt::Key_Backspace ) && hasSentCursors() && (cursor() <= limitCursor()) )
        return false;
    if( ( e->key() == Qt::Key_Backspace ) ) {
//      qDebug() << "baskspace";
    }


    //standard verification (inhereted)
    return QEditor::processCursor(c, e, b);
}

//TODO tester ça
void TextEditor::cut() {
    if( cursor().selectionStart() >= limitCursor() )
        QEditor::cut();
}

void TextEditor::commentSelectedText() {
    if( cursor().hasSelection() ) {
        cursor().selectionStart().insertText( "(*" );
        cursor().selectionEnd().insertText( "*)" );
    }
}

void TextEditor::uncommentSelectedText() {
  qDebug() << "not implemented yet, this is TODO";
    /*
    struct QParenthesis
{
        enum Role
        {
                Open		= 1,
                Close		= 2,
                Indent		= 4,
                Fold		= 8,
                Match		= 16
        };

        inline QParenthesis()
         : id(0), role(0), offset(0), length(0)
        {}

        inline QParenthesis(int i, quint8 r, int pos, int len)
         : id(i), role(r), offset(pos), length(len)
        {}

        int id;
        int role;
        int offset;
        int length;
};*/

/*
    QParenthesis
    int lineIndex = -1 + cursor().selectionStart().lineNumber();

    //the lines before the last line (so they must be full colored)
    while( ++lineIndex < cursor().selectionEnd().lineNumber() )
        document()->
        qDebug() << "line " << lineIndex << " : " << document()->line( lineIndex ).parentheses().count(QParenthesis(i, QParenthesis::Open, ) << " parentheses" << std::endl;*/
}

void TextEditor::commentLine(int lineIndex) {
    QDocumentCursor lineBegin( document(), lineIndex, 0 );
    lineBegin.insertText("(*");
    QDocumentCursor lineEnd( document(), lineIndex, document()->line(lineIndex).length() );
    lineEnd.insertText("*)");

}
void TextEditor::uncommentLine(int lineIndex) {
    QDocumentCursor lineBegin( document(), lineIndex, 0 );
    lineBegin.movePosition( 2, QDocumentCursor::NextCharacter, QDocumentCursor::KeepAnchor );
    QDocumentCursor lineEnd( document(), lineIndex, document()->line(lineIndex).length() );
    lineEnd.movePosition( 2, QDocumentCursor::PreviousCharacter, QDocumentCursor::KeepAnchor );

    if( ( lineBegin.selectedText().compare( "(*" ) == 0 ) && ( lineEnd.selectedText().compare( "*)") == 0 ) ) {
        //end before begin to keep good offsets
        lineEnd.removeSelectedText();
        lineBegin.removeSelectedText();
    }
}
void TextEditor::commentSelectedLines() {
    for( int lineIndex = cursor().selectionStart().lineNumber(); lineIndex <= cursor().selectionEnd().lineNumber(); lineIndex++ )
        commentLine( lineIndex );
}
void TextEditor::uncommentSelectedLines() {
    for( int lineIndex = cursor().selectionStart().lineNumber(); lineIndex <= cursor().selectionEnd().lineNumber(); lineIndex++ )
        uncommentLine( lineIndex );
}
