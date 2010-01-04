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

/*!
\brief A better document editor

Contained by a TextStd.
*/
class TextEditor : public QEditor
{
public:
    TextEditor(TextStd* p);

    /*! Returns last sent cursor. */
    QDocumentCursor lastSentCursor() const;
    /*! Returns the cursor representing the read-only limit. */
    QDocumentCursor limitCursor() const;
    /*! \brief Add a cursor to the list of sent cursors. */
    void sendCursor( QDocumentCursor c );
    /*! \brief Forget the last sent cursor */
    void restorePreviousCursor();
    /*! \brief Check if there are some sent cursors */
    bool hasSentCursors() const;
    /*! \brief Restart the editor */
    void restart();
    /*!
    Search a sentence available for Coq.

    \param limited if true, search before the current cursor position
    */
    QDocumentCursor searchPhraseToSend( bool limited = false );

    /*! \brief Cut the selected text, if any. */
    virtual void cut();

    /*! \brief Comment the selected text, if any. */
    void commentSelectedText();
    /*! \brief Uncomment the selected text, if any. */
    void uncommentSelectedText();
    /*! \brief Comment a given line. */
    void commentLine(int lineIndex);
    /*! \brief Uncomment a given line. */
    void uncommentLine(int lineIndex);
    /*! \brief Comment the selected lines, if any. */
    void commentSelectedLines();
    /*! \brief Uncomment the selected lines, if any. */
    void uncommentSelectedLines();

protected:
    virtual bool protectedCursor( const QDocumentCursor& c ) const;
    virtual bool processCursor(QDocumentCursor& c, QKeyEvent *e, bool& b);

    TextStd* textStd;
    QList<QDocumentCursor> sentCursors;
};

#endif // TEXTEDITOR_H
