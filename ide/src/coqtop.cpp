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
#include <QProcess>

//Project Libs

//Project Files
#include "coqtop.h"
#include "debugdialog.h"

////////////////
//   Coqtop   //
////////////////

Coqtop::Coqtop(TextStd* parent) :
        m_textStd(parent)
{
    m_processus = new QProcess(this);
//    m_processus->setReadChannel(QProcess::StandardError);
    m_processus->setReadChannelMode(QProcess::MergedChannels);
    connect(m_processus, SIGNAL(error(QProcess::ProcessError)), this, SLOT(dontStart()));
    QStringList arguments;
//    arguments.append( "-emacs" );
    m_processus->start("coqtop", arguments);
    m_processus->waitForStarted();
    m_processus->waitForReadyRead(-1);
    m_startMessage = QString( m_processus->readAllStandardOutput() );
}

QString Coqtop::startMessage() {
    return m_startMessage;
}

Answer* Coqtop::sendStdIn( const QString & text ) {
    QString envoi(text.trimmed());
    //unsupported commands
    if( !envoi.left(4).compare( "Undo" ) )
        return new Answer( false, "", "Undo : "+tr("This command is not supported") );
    if( !envoi.left(11).compare( "Write State" ) )
        return new Answer( false, "", "Write State : "+tr("This command is not supported") );
    if( !envoi.left(13).compare( "Restore State" ) )
        return new Answer( false, "", "Restore State : "+tr("This command is not supported") );
    //useless commands
    if( !envoi.left(5).compare( "Proof" ) ) {
        return sendStdIn( "Show." );
    }

    //for better paths, TODO ?
    //envoi.replace( "LoadPath \"", "LoadPath \""+QFileInfo( m_textStd->textStdIn()->fileName() ).canonicalPath()+"/" );

    if(!envoi.endsWith('\n'))
        envoi.append('\n');

    qDebug()<< "coqtop << " << envoi;
    m_processus->write(envoi.toAscii());

    qDebug() << "waiting for answer";
    m_processus->waitForReadyRead(-1);
    qDebug() << "answer is there : treat it";
    return traiteSortie();
}

bool Coqtop::modePreuve() {
    bool res;

    m_processus->write("Show.\n");
    m_processus->waitForReadyRead(-1);
    QString chaine(m_processus->readAllStandardOutput());
    res = (chaine.indexOf("Error: No focused proof (No proof-editing in progress).") == -1);

//    QString prompt( m_processus->readAllStandardError() );
//    res = prompt.left(3).compare( "Coq" );

    return res;
}

void Coqtop::undoQed() {
    m_processus->write( "Inspect 1.\n" );
    qDebug() << "coqtop << Inspect 1.";
    m_processus->waitForReadyRead(-1);
    QString chaine( m_processus->readAllStandardOutput() );
    qDebug() << "coqtop >> " << chaine;
    QRegExp reg( "(\\w*) :" );
    reg.indexIn( chaine );
    QString lastObjectName( reg.cap(1) );
    qDebug() << "object name : " << lastObjectName;
    m_processus->write( QString( "Reset "+lastObjectName+".\n" ).toAscii() );
}

Answer* Coqtop::undo( const QString &text ) {
    QString content(text.trimmed());
    //useless commands
    if( !content.left(5).compare( "Proof" ) )
        return sendStdIn( "Show." );
    if( !content.left(4).compare( "Show" ) )
        return sendStdIn( "Show." );
    //impossible-to-cancel commands
    if( !content.left(3).compare( "Qed" ) )
        return new Answer( false, "", "Qed : "+tr("This command cannot be cancelled") );
    if( !content.left(7).compare( "Defined" ) )
        return new Answer( false, "", "Defined : "+tr("This command cannot be cancelled") );
    if( !content.left(4).compare( "Save" ) )
        return new Answer( false, "", "Save : "+tr("This command cannot be cancelled") );
    if( !content.left(8).compare( "Admitted" ) )
        return new Answer( false, "", "Admitted : "+tr("This command cannot be cancelled") );

    if(modePreuve()) {
        m_processus->write("Undo.\n");
        qDebug() << "coqtop << Undo.";

        m_processus->waitForReadyRead(-1);
        QString chaine(m_processus->readAllStandardOutput());
        if(chaine.indexOf("Error: Undo stack exhausted") != -1) {
            m_processus->write("Abort.\n");
            qDebug() << "coqtop << Abort.";

            m_processus->waitForReadyRead(-1);
            chaine = m_processus->readAllStandardOutput();
        }
        return new Answer( true, chaine );
    }
    else {
        qDebug() << "coqtop << Back";
        m_processus->write("Back.\n");
        return new Answer( true );
    }
}

Answer* Coqtop::traiteSortie( QString value ) {
    QString chaine(value);
    if( chaine.isEmpty() )
        chaine = QString(m_processus->readAllStandardOutput());

    int debugPos = chaine.indexOf("TcDebug", 0, Qt::CaseInsensitive );
    if( debugPos > -1 ) {
        return LtacDebug( chaine.mid(debugPos), chaine.left(debugPos) );
//        emit signalStartDebug( chaine.mid(debugPos), chaine.left(debugPos) );
    }
    else {
        if( chaine.indexOf("error", 0, Qt::CaseInsensitive ) > -1 ) {
            qDebug() << "coqtop : error";
            qDebug() << chaine;
            return new Answer( false, QString(), chaine );
        }
        else {
            return new Answer( true, chaine );
        }
    }
}

void Coqtop::dontStart() {
  Except a;
  throw a;
}

Answer* Coqtop::LtacDebug( QString label, QString content ) {
    m_debugDialog = new DebugDialog( this );
    m_debugDialog->addDebugPage( label, content );
    m_debugDialog->exec();
    QString result( m_debugDialog->result() );
    delete m_debugDialog;
    m_debugDialog = 0;
    return traiteSortie( result );
}

DebugAnswer* Coqtop::LtacDebugNext() {
//    qDebug() << "coqtop << send next step";
    m_processus->write( "\n" );

    m_processus->waitForReadyRead(-1);
    QString content( m_processus->readAllStandardOutput() );
//    qDebug() << "result of step : " << content;
    int debugPos = content.indexOf("TcDebug", 0, Qt::CaseInsensitive );
    if( debugPos < 0 ) {
//        qDebug() << "coqtop << end of debug";
        return new DebugAnswer( false, "", QString( content ) );
    }
    else {
        return new DebugAnswer( true, QString( content.mid(debugPos) ), QString( content.left(debugPos) ) );
    }
}

void Coqtop::kill() {
    m_processus->kill();
}

void Coqtop::restart() {
    kill();
    QStringList arguments;
    m_processus->start("coqtop",arguments);
}

////////////////
//   Answer   //
////////////////

Answer::Answer( bool _accepted, QString _stdOut, QString _stdErr ) {
    accepted = _accepted;
    stdErr = QString( _stdErr );
    stdOut = QString( _stdOut );
}

/////////////////////
//   DebugAnswer   //
/////////////////////

DebugAnswer::DebugAnswer( bool _isDebug, QString _label, QString _content ) {
    isDebug = _isDebug;
    label = QString( _label );
    content = QString( _content );
}
