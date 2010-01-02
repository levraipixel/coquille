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

#ifndef DOCUMENTSENDCOMMAND_H
#define DOCUMENTSENDCOMMAND_H

/////////////////
//   Headers   //
/////////////////

//C++ Libs

//Qt Libs
#include <QWidget>

//Project Libs
#include "qdocumentcommand.h"

//Project Files
class TextStd;

//File Classes
class DocumentSendCommand;

//File NameSpace

/////////////////////////////
//   DocumentSendCommand   //
/////////////////////////////

class DocumentSendCommand : public QDocumentCommand
{
    public:
        enum SendFlag {
            Next = 0,
            Previous = 1
        };
        DocumentSendCommand( TextStd* t, QDocument* doc, SendFlag f, bool limited = false );
        virtual ~DocumentSendCommand();

        virtual bool mergeWith(const QUndoCommand *command);

        virtual void redo();
        virtual void undo();
    protected:
        TextStd* textStd;
        SendFlag m_send;
        bool m_limited;
};

#endif // DOCUMENTSENDCOMMAND_H
