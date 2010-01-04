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

//Project Libs

//Project Files
#include "documentsendcommand.h"
#include "textstd.h"

/////////////////////////////
//   DocumentSendCommand   //
/////////////////////////////

DocumentSendCommand::DocumentSendCommand( TextStd* t, QDocument* doc, DocumentSendCommand::SendFlag f, bool limited )
    : QDocumentCommand(QDocumentCommand::Custom, doc), textStd(t), m_send(f), m_limited(limited)
{
}

DocumentSendCommand::~DocumentSendCommand()
{
}

bool DocumentSendCommand::mergeWith(const QUndoCommand *command) {
    Q_UNUSED(command)
    return false;
}

void DocumentSendCommand::redo() {
    if( m_send == DocumentSendCommand::Next )
        textStd->sendNext( m_limited );
    else
        textStd->sendPrevious();
}

void DocumentSendCommand::undo() {
    if( m_send == DocumentSendCommand::Next )
        textStd->sendPrevious();
    else
        textStd->sendNext( m_limited );
}
