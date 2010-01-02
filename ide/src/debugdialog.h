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

class DebugPage : public QObject {
    Q_OBJECT
    public:
        DebugPage( DebugDialog *debugDialog, QTreeWidget *tree, QStackedWidget *stack );
        DebugPage( DebugDialog *debugDialog, QTreeWidget *tree, QStackedWidget *stack, QString label, QString content );
        ~DebugPage();
        void fill( DebugPage *parent, QString label, QString content );
    public slots:
        void treeItemChanged( QTreeWidgetItem *previous, QTreeWidgetItem *next );
        void stackItemChanged( int next );

    public:
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

class DebugDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(DebugDialog)
public:
    explicit DebugDialog( Coqtop *parent = 0);
    virtual ~DebugDialog();
    QString result();

public slots:
    virtual void reject();
    void addDebugPage( QString label, QString content );
    void next();
    void previous();
    void endDebug();

signals:
    void nextDebugStep();

protected:
    virtual void changeEvent(QEvent *e);
    virtual void closeEvent( QCloseEvent * event );

public:
    Ui::DebugDialog *m_ui;
    Coqtop *m_coqtop;
    QList<DebugPage*> m_debugPages;
    bool m_finished;
    QString m_result;
};

#endif // DEBUGDIALOG_H
