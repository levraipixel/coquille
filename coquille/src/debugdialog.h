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

#ifndef DEBUGDIALOG_H
#define DEBUGDIALOG_H

/////////////////
//   Headers   //
/////////////////

//C++ Libs

//Qt Libs
#include <QDialog>
class QTreeWidget;
class QTreeWidgetItem;
class QTextEdit;
class QStackedWidget;

//Project Libs

//Project Files
class Coqtop;

//File Classes
class DebugDialog;

//File NameSpace
namespace Ui {
    class DebugDialog;
}

///////////////////
//   DebugPage   //
///////////////////

/*!
\brief A page in Ltac Debug Mode

It is used in the debug dialog.
*/
class DebugPage : public QObject {
    Q_OBJECT

public:
    DebugPage( DebugDialog *debugDialog, QTreeWidget *tree, QStackedWidget *stack, QString label, QString content );
    ~DebugPage();

protected slots:
    void treeItemChanged( QTreeWidgetItem *previous, QTreeWidgetItem *next );
    void stackItemChanged( int next );

protected:
    DebugPage( DebugDialog *debugDialog, QTreeWidget *tree, QStackedWidget *stack );
    void fill( DebugPage *parent, QString label, QString content );

    DebugDialog *m_debugDialog;
    DebugPage *m_parent;
    QTreeWidget *m_tree;
    int m_level;
    QTreeWidgetItem *m_node;
    QStackedWidget *m_stack;
    QTextEdit *m_page;
};

/////////////////////
//   DebugDialog   //
/////////////////////

/*!
\brief A way to display the answers of Coq in Ltac Debug Mode

It gives the user a Treeview to navigate between the answers, displayed on the right on different pages.
*/
class DebugDialog : public QDialog {
    Q_OBJECT

public:
    explicit DebugDialog( Coqtop *parent = 0);
    virtual ~DebugDialog();

    /*! adds a new page to the dialog, with the label on the tree and the content on the page */
    void addDebugPage( QString label, QString content );
    /*! returns the pages */
    QList<DebugPage*>* debugPages();
    /*! returns the result of Debug Mode */
    QString result();

protected slots:
    void next();
    void previous();

protected:
    void endDebug();
    virtual void reject();

    virtual void changeEvent(QEvent *e);
    virtual void closeEvent( QCloseEvent * event );

    Ui::DebugDialog *m_ui;
    Coqtop *m_coqtop;
    QList<DebugPage*> m_debugPages;
    bool m_finished;
    QString m_result;
};

#endif // DEBUGDIALOG_H
