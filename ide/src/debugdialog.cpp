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
#include <QCloseEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QTextEdit>

//Project Libs

//Project Files
#include "coqtop.h"
#include "debugdialog.h"
#include "ui_debugdialog.h"

///////////////////
//   DebugPage   //
///////////////////

void DebugPage::fill( DebugPage *parent, QString label, QString content ) {
//  qDebug() << "label : " << label;
//  qDebug() << "content : " << content;
    m_parent = parent;

    m_node = new QTreeWidgetItem();
    m_node->setText( 0, label );

    if( m_parent ) {
//      qDebug() << "parent : " << m_parent->m_node->text(0);
        m_parent->m_node->setExpanded(true);
        m_parent->m_node->addChild( m_node );
    }
    else
        m_tree->addTopLevelItem( m_node );

    m_page = new QTextEdit( m_stack );
    m_page->setText( content );
    m_stack->addWidget( m_page );

    connect( m_tree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(treeItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect( m_stack, SIGNAL(currentChanged(int)), this, SLOT(stackItemChanged(int)));

    m_stack->setCurrentWidget( m_page );
}

DebugPage::DebugPage( DebugDialog *debugDialog, QTreeWidget *tree, QStackedWidget *stack )
    : m_debugDialog(debugDialog), m_tree(tree), m_stack(stack)
{
//  qDebug() << "Root";
    m_level = -1;
    fill( 0, tr( "Stack" ), tr( "Debug Start" ) );
}

DebugPage::DebugPage( DebugDialog *debugDialog, QTreeWidget *tree, QStackedWidget *stack, QString label, QString content )
    : m_debugDialog(debugDialog), m_tree(tree), m_stack(stack)
{
    if( m_debugDialog->debugPages()->size() <= 0 )
        m_debugDialog->debugPages()->append( new DebugPage( m_debugDialog, m_tree, m_stack ) );

    QRegExp reg( "TcDebug \\((\\d+)\\)" );
    reg.indexIn( label );
    m_level = reg.cap(1).toInt();

//    qDebug() << "other element";
    DebugPage *parentPage = m_debugDialog->debugPages()->last();
    while( parentPage && ( m_level <= parentPage->m_level ) )
        parentPage = parentPage->m_parent;
    fill( parentPage, label, content );
}

DebugPage::~DebugPage() {
    delete m_page;
    delete m_node;
}

void DebugPage::treeItemChanged( QTreeWidgetItem *previous, QTreeWidgetItem *next ) {
    Q_UNUSED(next)
    if( previous == m_node )
        m_stack->setCurrentWidget( m_page );
}

void DebugPage::stackItemChanged( int next ) {
    if( m_stack->indexOf( m_page ) == next )
        m_tree->setCurrentItem( m_node );
}

/////////////////////
//   DebugDialog   //
/////////////////////

DebugDialog::DebugDialog(Coqtop *parent) :
    QDialog(0),
    m_ui(new Ui::DebugDialog),
    m_coqtop(parent)
{
    m_ui->setupUi(this);

    m_finished = false;

    QIcon prevIcon, nextIcon;
    prevIcon.addFile(QString::fromUtf8(":/IconPrefix/images/left_blue.png"), QSize(), QIcon::Normal, QIcon::Off);
    nextIcon.addFile(QString::fromUtf8(":/IconPrefix/images/right_blue.png"), QSize(), QIcon::Normal, QIcon::Off);

    QPushButton *prev = new QPushButton( prevIcon, tr("Previous"), m_ui->buttonBox );
    QPushButton *next = new QPushButton( nextIcon, tr("Next"), m_ui->buttonBox );
    m_ui->buttonBox->addButton( next, QDialogButtonBox::ApplyRole );
    m_ui->buttonBox->addButton( prev, QDialogButtonBox::ApplyRole );
    m_ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);

    connect(prev, SIGNAL(clicked()), this, SLOT(previous()));
    connect(next, SIGNAL(clicked()), this, SLOT(next()));
}

DebugDialog::~DebugDialog()
{
    delete m_ui;
}

void DebugDialog::addDebugPage( QString label, QString content ) {
    m_debugPages.append( new DebugPage( this, m_ui->pageTree, m_ui->pageStack, label, content ) );
}

QList<DebugPage*>* DebugDialog::debugPages() {
    return &m_debugPages;
}

void DebugDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DebugDialog::previous() {
//  qDebug() << "previous";
    QTreeWidgetItem *prevItem = m_ui->pageTree->itemAbove( m_ui->pageTree->currentItem() );
    if( prevItem )
        m_ui->pageTree->setCurrentItem( prevItem );
}
void DebugDialog::next() {
//    qDebug() << "next";
    QTreeWidgetItem *nextItem = m_ui->pageTree->itemBelow( m_ui->pageTree->currentItem() );
//    qDebug() << "nextItem : " << nextItem;
    if( nextItem )
        m_ui->pageTree->setCurrentItem( nextItem );
    else if( !m_finished ) {
//        qDebug() << "debugdialog << send next step command";
        DebugAnswer *answer = m_coqtop->LtacDebugNext();
        if( answer->isDebug ) {
            addDebugPage( answer->label, answer->content );
        }
        else {
            m_result = QString( answer->content );
            endDebug();
        }
    }
}

QString DebugDialog::result() {
    return m_result;
}

void DebugDialog::endDebug() {
//    qDebug() << "debugdialog << debug finished";
    m_finished = true;
    m_ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(false);
}

void DebugDialog::reject() {
//    qDebug() << "Annuler";
    m_result = QString( "error : Cancelled by user during Debug Mode" );
    QDialog::reject();
}

void DebugDialog::closeEvent( QCloseEvent * event ) {
//  qDebug() << "debugdialog << closeevent";
    if( m_finished )
        event->accept();
    else {
        event->ignore();
        switch( QMessageBox::question(this, tr( "The debug is not finished yet" ), tr( "Are you sure you want to exit ?" ), QMessageBox::Cancel | QMessageBox::Close ) ) {
            case QMessageBox::Close:
                reject();
                break;
            default:
                break;
        }
    }
}
