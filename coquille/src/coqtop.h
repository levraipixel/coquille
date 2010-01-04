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

/*! \brief A Coq answer */
class Answer {
public:
    Answer( bool _accepted, QString _stdOut = QString(), QString _stdErr = QString() );
    /*! if the answer is : accepted */
    bool accepted;
    /*! content of StdOut */
    QString stdOut;
    /*! content of StdErr */
    QString stdErr;
};

/////////////////////
//   DebugAnswer   //
/////////////////////

/*! \brief A debug step return */
class DebugAnswer {
public:
    DebugAnswer( bool _isDebug, QString _label, QString _content );
    /*! if the answer is still in debug mode */
    bool isDebug;
    /*! the title of the answer */
    QString label;
    /*! the content of the answer */
    QString content;
};

////////////////
//   Except   //
////////////////

/*! \brief A custom exception */
class Except {};

////////////////
//   Coqtop   //
////////////////

/*!
\brief An interface with Coq

It allows the user to send sentences and receive answers.
*/
class Coqtop : public QObject {
    Q_OBJECT

public:
    Coqtop(TextStd* parent = 0);

    /*! returns the Start Message of the process used to dialog with */
    QString startMessage();
    /*! send to Coq */
    Answer *sendStdIn( const QString &text );
    /*! undo last operation */
    Answer* undo( const QString &text );
    /*! asks for next step in debug mode */
    DebugAnswer* LtacDebugNext();
    /*! kills the process */
    void kill();
    /*! restarts the process */
    void restart();

protected slots:
    void dontStart();

protected:
    Answer* traiteSortie( QString value = "" );

    void undoQed();
    Answer* LtacDebug( QString label, QString content );

    bool modePreuve();
    TextStd* m_textStd;
    QProcess* m_processus;
    QString m_startMessage;
    DebugDialog *m_debugDialog;
};

#endif // COQTOP_H
