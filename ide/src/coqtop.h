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

#ifndef COQTOP_H
#define COQTOP_H

/////////////////
//   Headers   //
/////////////////

//C++ Libs

//Qt Libs
#include <QWidget>
#include <QString>
class QProcess;
class QDebug;

//Project Libs

//Project Files
class TextStd;
class DebugDialog;

//File Classes
class Answer;
class DebugAnswer;
class Except;
class Coqtop;

//File NameSpace
using namespace std;

////////////////
//   Answer   //
////////////////

class Answer {
public:
    Answer( bool _accepted, QString _stdOut = QString(), QString _stdErr = QString() );
    bool accepted;
    QString stdOut;
    QString stdErr;
};

/////////////////////
//   DebugAnswer   //
/////////////////////

class DebugAnswer {
public:
    DebugAnswer( bool _isDebug, QString _label, QString _content );
    bool isDebug;
    QString label;
    QString content;
};

////////////////
//   Except   //
////////////////

class Except {};

////////////////
//   Coqtop   //
////////////////

class Coqtop : public QObject {
    Q_OBJECT
public:
    Coqtop(TextStd* parent = 0);
    Answer *sendStdIn( const QString &text );
    void undoQed();
    Answer* undo( const QString &text );
    Answer* LtacDebug( QString label, QString content );
    DebugAnswer* LtacDebugNext();
    void kill();
    void restart();
    QString startMessage();

signals:
    void signalDebug( QString label, QString content );
    void signalStartDebug( QString label, QString content );
    void signalEndDebug( Answer *answer );

private:
    bool modePreuve();
    TextStd* m_textStd;
    QProcess* m_processus;
    QString m_startMessage;
    DebugDialog *m_debugDialog;

private slots:
    Answer* traiteSortie( QString value = "" );
    void dontStart();
};

#endif // COQTOP_H
