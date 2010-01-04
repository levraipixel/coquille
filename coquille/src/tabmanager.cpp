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
#include <QFileDialog>
#include <QMessageBox>

//Project Libs

//Project Files
#include "filelink.h"
#include "tabmanager.h"
#include "texteditor.h"
#include "textstd.h"
#include "ui_tabmanager.h"

////////////////////
//   TabManager   //
////////////////////

TabManager::TabManager( QWidget *parent, Settings *settings ) :
        QWidget(parent),
        m_ui(new Ui::TabManager),
        m_settings(settings)
{
    m_ui->setupUi(this);

    tabWidget = new QTabWidget(this);
    tabWidget->setTabsClosable(true);

    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(emitDocName(int)));
    connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

    m_ui->verticalLayout->addWidget(tabWidget);
}

TabManager::~TabManager() {
    delete m_ui;
}

bool TabManager::hasOpenedDocuments() {
    return ( tabWidget->count() > 0 );
}
void TabManager::addNewTab() {
    QString tabName = tr("New file");
    int index = addTab(tabName);
    tabWidget->setCurrentIndex(index);

    emit tabOpen(tabName, index);
}

void TabManager::openFile(const QString& fileName) {
    bool found = false;
    int indexFound = -1;
    for( int tabIndex = 0; !found && (tabIndex < tabWidget->count()); tabIndex++ ) {
        if( textStdOf( tabIndex )->textStdIn()->fileName().compare( fileName ) == 0 ) {
            found = true;
            indexFound = tabIndex;
        }
    }
    if( found )
        tabWidget->setCurrentIndex( indexFound );
    else {
        int index = addTab();
        load(fileName, index);

        emit tabOpen(tabWidget->tabText(index), index);
    }
}

void TabManager::load(const QString& fileName, int index) {
    int _index = trueIndex( index );

    TextEditor* editor = textStdOf( _index )->textStdIn();
    editor->load( fileName );
    tabWidget->setTabText( _index, editor->name() );
    tabWidget->setCurrentIndex( _index );
}

void TabManager::save(bool forceNameChanging, int index) {
    int _index = trueIndex( index );

    QEditor* editor = textStdOf( _index )->textStdIn();

    //if it's a new file, with no name, or if we force the name changing
    if( editor->fileName().isEmpty() || forceNameChanging ) {
        if( !rename( editor ) )
            return;
        else {
            setTabText( _index, editor->name() );

            if( _index == tabWidget->currentIndex() )
                emitDocName();
        }
    }

    //in case the rename functions didn't operate well
    if( editor->fileName().isEmpty() )
        QMessageBox::warning(this, tr("Problem with name setting"), tr("Unable to set the file name"));
    else
        editor->save();
}

void TabManager::saveAll() {
    for(int i = 0; i < tabWidget->count(); ++i) {
        save(false, i);
    }
}

void TabManager::closeWithoutSave( bool addTab, int index ) {
    int _index = trueIndex( index );

    if( addTab && ( tabWidget->count() == 1 ) )
        addNewTab();

    tabWidget->removeTab( _index );

    emit tabClose(tabWidget->tabText(_index), _index);
}

bool TabManager::closeTab( int index ) {
    return close( true, index );
}

bool TabManager::close( bool addTab, int index ) {
    int _index = trueIndex( index );

    //qDebug() << "close action for " << _index;

    if( !textStdOf( _index )->textStdIn()->isContentModified() ) {
        closeWithoutSave( addTab, _index );
        return true;
    }
    else {
        tabWidget->setCurrentIndex( _index );
        switch( QMessageBox::question(this, tr("The document has modifications"), tr("Do you want to save the modifications ?"), QMessageBox::Abort | QMessageBox::Save | QMessageBox::Ignore ) ) {
            case QMessageBox::Ignore:
                closeWithoutSave( addTab, _index );
                return true;
            case QMessageBox::Save:
                save( false, _index );
                if( textStdOf( _index )->textStdIn()->isContentModified() ) {
                    QMessageBox::warning(this, tr("Problem on saving"), tr("Unable to save the file"));
                    return false;
                }
                closeWithoutSave( addTab, _index );
                return true;
            default:
                return false;
        }
    }
}

bool TabManager::closeAll() {
    while( tabWidget->count() > 0 ) {
        if( !close( false, tabWidget->count()-1 ) )
            return false;
    }
    return true;
}

void TabManager::selectAll() {
    currentTextStd()->selectAll();
}
void TabManager::copy() {
    currentTextStd()->copy();
}
void TabManager::cut() {
    currentTextStd()->cut();
}
void TabManager::paste() {
    currentTextStd()->paste();
}
void TabManager::undo() {
    currentTextStd()->undo();
}
void TabManager::redo() {
    currentTextStd()->redo();
}
void TabManager::search() {
    currentTextStd()->search();
}
void TabManager::searchNext() {
    currentTextStd()->searchNext();
}
void TabManager::replace() {
    currentTextStd()->replace();
}
void TabManager::goToLine() {
    currentTextStd()->goToLine();
}
void TabManager::indent() {
    currentTextStd()->indentSelection();
}
void TabManager::unindent() {
    currentTextStd()->unindentSelection();
}
void TabManager::comment() {
    currentTextStd()->commentSelection();
}
void TabManager::uncomment() {
    currentTextStd()->uncommentSelection();
}
void TabManager::commentLines() {
    currentTextStd()->commentLines();
}
void TabManager::uncommentLines() {
    currentTextStd()->uncommentLines();
}
void TabManager::insert(QString text) {
    currentTextStd()->insert(text);
}

void TabManager::next() {
    currentTextStd()->next();
}
void TabManager::previous() {
    currentTextStd()->previous();
}
void TabManager::goToStart() {
    currentTextStd()->goToStart();
}
void TabManager::goToEnd() {
    currentTextStd()->goToEnd();
}
void TabManager::goToCursor() {
    currentTextStd()->goToCursor();
}
void TabManager::interrupt() {
    currentTextStd()->interrupt();
}
void TabManager::restart() {
    currentTextStd()->restart();
}

void TabManager::setCurrentIndex(int index) {
    tabWidget->setCurrentIndex(index);
}

///////////// private ////////////////////////////
void TabManager::emitDocName(int index) {
    int _index = trueIndex(index);

    if( _index >= 0 ) {
        QString name = textStdOf( _index )->textStdIn()->name();

        if(name.isEmpty())
            name = tabWidget->tabText( _index );

        if( ( name.size() > 0 ) && ( name.at(0) == QChar('*') ) )
            name.remove(0,1);

        emit displayDocName(name);

        emit contentModified( textStdOf( _index )->textStdIn()->isContentModified() );
    }
}

int TabManager::addTab(const QString& name) {
    TextStd* textStd = new TextStd( this, m_settings );
    int index = tabWidget->addTab(textStd, name);

    connect(textStd, SIGNAL(contentModified(bool)), this, SLOT(onDocumentModified(bool)));

    return index;
}

void TabManager::onDocumentModified(bool b) {
    int _index = tabWidget->currentIndex();

    TextEditor* editor = textStdOf( _index )->textStdIn();

    QString text = editor->name();

    if(text.isEmpty())
        text = tr("New file");

    if(b)
        tabWidget->setTabText( _index, "*" + text );
    else
        tabWidget->setTabText( _index, text );

    emit contentModified(b);
}

int TabManager::trueIndex(int index) {
    if( index < 0 )
        return tabWidget->currentIndex();
    else
        return index;
}

bool TabManager::rename(QEditor* editor) {
    //asks for a name (a path)
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file ..."), "", tr("Coq Files (*.v *.V *.g)"));
    //if the user cancelled the operation or the name is not valid
    if(fileName.isEmpty()) {
        QMessageBox::warning(this, tr("File name not changed"), tr("Operation cancelled or not a valid name"));
        return false;
    }
    else {
        if ((!fileName.endsWith(".v", Qt::CaseInsensitive)) && (!fileName.endsWith(".g")))
            fileName += ".v";

        editor->setFileName(fileName);
        return true;
    }
}

TextStd* TabManager::textStdOf(int index) {
    int _index = trueIndex( index );

    return qobject_cast<TextStd*>( tabWidget->widget( _index ) );
}

TextStd* TabManager::currentTextStd() {
    return qobject_cast<TextStd*>(tabWidget->currentWidget());
}

void TabManager::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

FilesLinkList *TabManager::openedDocuments() {
    FilesLinkList *result = new FilesLinkList(tabWidget->count());
    for( int i = 0; i < tabWidget->count(); i++ )
        result->addFile( textStdOf( i )->textStdIn()->fileName() );
    return result;
}

QStringList TabManager::tabNameList()
{
    QStringList list;

    for(int i = 0; i < tabWidget->count(); ++i) {
        list << tabWidget->tabText(i);
    }

    return list;
}

QString TabManager::wordUnderCursor() {
    return currentTextStd()->wordUnderCursor();
}

void TabManager::setTabText(int index, const QString& text)
{
    tabWidget->setTabText(index, text);
    emit tabNameChange(text, index);
}
