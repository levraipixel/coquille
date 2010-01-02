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

#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

/////////////////
//   Headers   //
/////////////////

//C++ Libs

//Qt Libs

//Project Libs
#include "qeditor.h"
class QFormat;

//Project Files
class TextStd;

//File Classes
class TextEditor;

//File NameSpace

////////////////////
//   TextEditor   //
////////////////////

class TextEditor : public QEditor
{
public:
    TextEditor(TextStd* p);

    QDocumentCursor lastSentCursor() const;
    QDocumentCursor limitCursor() const;
    void sendCursor( QDocumentCursor c );
    void restorePreviousCursor();
    bool hasSentCursors() const;
    void restart();
    QDocumentCursor searchPhraseToSend( bool limited = false );

    virtual bool protectedCursor( const QDocumentCursor& c ) const;
    virtual bool processCursor(QDocumentCursor& c, QKeyEvent *e, bool& b);
    virtual void cut();

    void commentSelectedText();
    void uncommentSelectedText();
    void commentLine(int lineIndex);
    void uncommentLine(int lineIndex);
    void commentSelectedLines();
    void uncommentSelectedLines();

private:
    TextStd* textStd;
    QList<QDocumentCursor> sentCursors;
};

#endif // TEXTEDITOR_H
