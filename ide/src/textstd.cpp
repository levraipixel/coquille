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
#include <QMessageBox>
#include <QDebug>

//Project Libs
#include "qcodeedit.h"
#include "qdocumentline.h"
#include "qfoldpanel.h"
#include "qformatscheme.h"
#include "qgotolinepanel.h"
#include "qlanguagedefinition.h"
#include "qlanguagefactory.h"
#include "qlinechangepanel.h"
#include "qlinenumberpanel.h"
#include "qsearchreplacepanel.h"
#include "qstatuspanel.h"

//Project Files
#include "coqtop.h"
#include "documentsendcommand.h"
#include "settings.h"
#include "texteditor.h"
#include "textstd.h"
#include "ui_textstd.h"

/////////////////
//   TextStd   //
/////////////////

TextStd::TextStd( QWidget *parent, Settings *settings ) :
        QWidget(parent),
        m_ui(new Ui::TextStd),
        m_settings(settings)
{
    m_ui->setupUi(this);
    m_editControl = new QCodeEdit(new TextEditor(this), 0);
    m_ui->TextStdLeft->insertWidget(0, m_editControl->editor());

    m_formats = new QFormatScheme(":/SyntaxPrefix/defaultSyntax", this);
    m_languages = new QLanguageFactory(m_formats, this);
    m_languages->addDefinitionPath(":/SyntaxPrefix/syntax");
    m_languages->setLanguage(m_editControl->editor(), ".v");
//    qDebug() << m_languages->fileFilters().join(" ");
//    qDebug() << m_languages->languages().join(" ");

    try {
        m_coqtop = new Coqtop(this);
        displayStdOut( m_coqtop->startMessage() );
    }
    catch(Except) {
        QMessageBox::critical(this, "Coqtop is missing", "You should install it before using this software" );
	    exit(1);
    }
    m_lineNumberPanel = new QLineNumberPanel;
    m_editControl->addPanel( m_lineNumberPanel, QCodeEdit::West );
    m_foldPanel = new QFoldPanel;
    m_editControl->addPanel( m_foldPanel, QCodeEdit::West );
    m_editControl->toggleViewAction( m_foldPanel )->setShortcut(QKeySequence("Ctrl+F9"));
    m_lineChangePanel = new QLineChangePanel;
    m_editControl->addPanel( m_lineChangePanel, QCodeEdit::West );
    m_statusPanel = new QStatusPanel;
    m_editControl->addPanel( m_statusPanel, QCodeEdit::South );
    m_searchReplacePanel = new QSearchReplacePanel;
    m_editControl->addPanel( m_searchReplacePanel, QCodeEdit::South );
    m_gotoLinePanel = new QGotoLinePanel;
    m_editControl->addPanel( m_gotoLinePanel, QCodeEdit::South );


    connect( m_editControl->editor(), SIGNAL(contentModified(bool)), this, SIGNAL(contentModified(bool)));
}

TextStd::~TextStd() {
    delete m_ui;
}

TextEditor* TextStd::textStdIn() {
    return (TextEditor*)m_editControl->editor();
}

QString TextStd::wordUnderCursor() {
    QDocumentCursor c = textStdIn()->cursor();
    c.select( QDocumentCursor::WordUnderCursor );
    return c.selectedText();
}

//expand a line only if it is the beginning of a collapsed block, and returns if it did something (= if lineIndex is at the beginning of a block)
void TextStd::expandLine( const int line ) {
  qDebug() << "expandLine " << line;
    //m_languages->languageData("Coq").d->expand( textStdIn()->document(), line );
    textStdIn()->document()->languageDefinition()->expand( textStdIn()->document(), line );
}

//expand all the necessary for a line to be seen
void TextStd::showLine( const int line ) {
    QDocumentCursor blockStart = textStdIn()->document()->cursor( line );
    while( textStdIn()->document()->line(line).isHidden() ) {
        //ensure that blockStart is at the beginning of the first block hidding it
        while( !blockStart.line().hasFlag(QDocumentLine::CollapsedBlockStart) )
            blockStart.movePosition(1, QDocumentCursor::PreviousBlock, QDocumentCursor::MoveAnchor);
        //expands the found block
        expandLine( blockStart.lineNumber() );
    }
}

void TextStd::changeEvent(QEvent *e) {
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void TextStd::fillStdIn( QString text) {
    textStdIn()->setText( text );
}

QString TextStd::displayUnicode( const QString &text ) {
    QString result(text);
//    qDebug() << "before :\n" << text;

    result.replace( "<=", QChar(0x2264) );
    result.replace( ">=", QChar(0x2265) );
    result.replace( "<>", QChar(0x2260) );

    result.replace( "forall", QChar(0x2200) );
    result.replace( "exists", QChar(0x2203) );
    result.replace( "/\\", QChar(0x22C0) );
    result.replace( "\\/", QChar(0x22C1) );

    result.replace( "|-", QChar(0x22A2) );
    result.replace( "=>", QChar(0x21D2) );
    result.replace( "<->", QChar(0x2194) );
    result.replace( "->", QChar(0x2192) );
    result.replace( "~", QChar(0x00AC) );

    result.replace( "False", QChar(0x22A5) );
    result.replace( "True", QChar(0x22A4) );

    //result.replace( "Rint", "<img src=':/symbols/int.png' alt='Rint'/>" );//QImage( QString::fromUtf8(":/symbols/int.png") ).sr );
    result.replace( "Rint", QChar(0x222B) );

    int start = 0;
    int end = 0;
    while( ( start = result.indexOf( "====", end ) ) >= 0 ) {
        int end = result.indexOf( " ", start );
        for( int pos = start; pos < end-1; pos++ )
            result.replace( pos, 1, '_' );
    }

    return result;
}

QString TextStd::removePromptsFrom( QString text ) {
    QString result( text );
    result.remove( QRegExp( "\\w* <" ) );
    return result;
}

void TextStd::displayStdOut( QString text ) {
//    qDebug() << "display stdOut : " << text;
    QString result = removePromptsFrom( text );
    if( m_settings->textStdLatexMode ) {
        qDebug() << "latex mode";
        result = displayUnicode( result );
    }
    m_ui->textStdOut->setText( result );
    displayStdErr( "" );
}
void TextStd::displayStdErr( QString text ) {
    m_ui->textStdErr->setText( text );
}
void TextStd::displayAnswer( Answer *answer ) {
    displayStdOut( answer->stdOut );
    displayStdErr( answer->stdErr );
}

/* Actions */
void TextStd::next( bool limited ) {
    QDocumentCommand *command = new DocumentSendCommand( this, textStdIn()->document(), DocumentSendCommand::Next, limited );
    textStdIn()->document()->execute( command );
}
void TextStd::sendNext( bool limited ) {
    QDocumentCursor result = textStdIn()->searchPhraseToSend( limited );

    if( result.hasSelection() ) {
        //the result is a cursor selecting the sentence found

        //prevents any modification
        textStdIn()->sendCursor( result );
        //unselect the last character before overlaying
        QDocumentCursor overlayStart, overlayEnd;
        overlayStart = result.selectionStart();
        overlayEnd = result.selectionEnd();
        overlayEnd.movePosition( 1, QDocumentCursor::PreviousCharacter, QDocumentCursor::MoveAnchor );
        //overlays
        addSentOverlay( overlayStart, overlayEnd );
        //sends to Coqtop

        Answer *answer = m_coqtop->sendStdIn( result.selectedText() );

        displayAnswer( answer );
        qDebug() << "validation is : " << answer->accepted;
        if( answer->accepted ) {
            //colors the valid lines
            removeValidOverlay( overlayStart, overlayEnd );
            addValidOverlay( overlayStart, overlayEnd );
        }
        else {
            sendPrevious( false );
        }
        delete answer;
    }
}
void TextStd::previous() {
    QDocumentCommand *command = new DocumentSendCommand( this, textStdIn()->document(), DocumentSendCommand::Previous );
    textStdIn()->document()->execute( command );
}
void TextStd::sendPrevious( bool undo ) {
    if( textStdIn()->hasSentCursors() ) {
        //if the previous text is in a folded block, expand it
        showLine( textStdIn()->limitCursor().lineNumber() );

        //Undo if wanted (usual case)
        if( undo ) {
            Answer *answer = m_coqtop->undo( textStdIn()->lastSentCursor().selectedText() );
            displayAnswer( answer );
            qDebug() << "validation is : " << answer->accepted;
            if( !answer->accepted )
                return;
        }

        //uncolors the text
        removeValidOverlay( textStdIn()->lastSentCursor().selectionStart(), textStdIn()->lastSentCursor().selectionEnd() );

        //removes protection
        textStdIn()->restorePreviousCursor();
    }
}

void TextStd::goToStart() {
    while( textStdIn()->hasSentCursors() ) {
        previous();
    }
}
void TextStd::goToEnd() {
    QDocumentCursor currentSentCursor = 0;
    while( currentSentCursor != textStdIn()->limitCursor() ) {
        currentSentCursor = textStdIn()->limitCursor();
        next();
    }
}
void TextStd::goToCursor() {
    QDocumentCursor currentSentCursor = 0;
    while( currentSentCursor != textStdIn()->limitCursor() ) {
        currentSentCursor = textStdIn()->limitCursor();
        next( true );
    }
}

void TextStd::interrupt() {
    qDebug() << "interrupt";
    m_coqtop->kill();
}
void TextStd::restart() {
    qDebug() << "restart";
    m_coqtop->restart();
    textStdIn()->restart();
}

void TextStd::addFormatOverlay( const int formatId, const int lineIndex, const int columnStart, const int columnEnd ) {
    int _columnEnd = ( columnEnd < 0 ) ? textStdIn()->document()->line( lineIndex ).length() : columnEnd;
    if( columnStart < _columnEnd ) {
        if( m_settings->textStdScrollCursorOnSend )
            textStdIn()->ensureVisible( lineIndex );
        textStdIn()->document()->line( lineIndex ).addOverlay( QFormatRange( columnStart, _columnEnd-columnStart, formatId ) );
        //qDebug() << "color line " << lineIndex << " from " << columnStart << " to " << _columnEnd << " with id " << formatId;
    }
}
void TextStd::addFormatOverlay( const int formatId, QDocumentCursor start, QDocumentCursor end ) {
    if( end < start )
        return;

    if( start.lineNumber() == end.lineNumber() ) //only from start to end
        addFormatOverlay( formatId, start.lineNumber(), start.columnNumber(), end.columnNumber() );
    else {
        //start line (from start to EOL)
        addFormatOverlay( formatId, start.lineNumber(), start.columnNumber() );

        //lines strictly between these of start and end
        for( int lineIndex = start.lineNumber()+1; lineIndex < end.lineNumber(); lineIndex++ )
            addFormatOverlay( formatId, lineIndex );

        //end line (from SOL to end)
        addFormatOverlay( formatId, end.lineNumber(), 0, end.columnNumber() );
    }
}
void TextStd::addSentOverlay( const int lineIndex, const int columnStart, const int columnEnd ) {
    addFormatOverlay( m_formats->id( "sent" ), lineIndex, columnStart, columnEnd );
}
void TextStd::addSentOverlay( QDocumentCursor start, QDocumentCursor end ) {
    addFormatOverlay( m_formats->id( "sent" ), start, end );
}
void TextStd::addValidOverlay( const int lineIndex, const int columnStart, const int columnEnd ) {
    addFormatOverlay( m_formats->id( "valid" ), lineIndex, columnStart, columnEnd );
}
void TextStd::addValidOverlay( QDocumentCursor start, QDocumentCursor end ) {
    addFormatOverlay( m_formats->id( "valid" ), start, end );
}

//Warning : here formatId is not the format to remove but the format to set to the beginning of the line after removing
//the overlays all over the line
void TextStd::removeFormatOverlay( const int formatId, const int lineIndex, const int columnStart ) {
    textStdIn()->document()->line( lineIndex ).clearOverlays();
    //qDebug() << "uncolor line " << lineIndex ;
    textStdIn()->document()->highlight(); // TODO we can remove that line ?
    if( columnStart > 0 )
        addFormatOverlay( formatId, lineIndex, 0, columnStart );
}
void TextStd::removeFormatOverlay( const int formatId, QDocumentCursor start ) {
    removeFormatOverlay( formatId, start.lineNumber(), start.columnNumber() );
}
void TextStd::removeFormatOverlay( const int formatId, QDocumentCursor start, QDocumentCursor end ) {
    if( end < start )
        return;

    //lines strictly after start
    for( int lineIndex = end.lineNumber(); lineIndex > start.lineNumber(); lineIndex-- )
        removeFormatOverlay( formatId, lineIndex );

    //start line (from start to EOL)
    removeFormatOverlay( formatId, start );
}
void TextStd::removeSentOverlay( const int lineIndex, const int columnStart ) {
    removeFormatOverlay( m_formats->id( "sent" ), lineIndex, columnStart );
}
void TextStd::removeSentOverlay( QDocumentCursor start ) {
    removeFormatOverlay( m_formats->id( "sent" ), start );
}
void TextStd::removeSentOverlay( QDocumentCursor start, QDocumentCursor end ) {
    removeFormatOverlay( m_formats->id( "sent" ), start, end );
}
void TextStd::removeValidOverlay( const int lineIndex, const int columnStart ) {
    removeFormatOverlay( m_formats->id( "valid" ), lineIndex, columnStart );
}
void TextStd::removeValidOverlay( QDocumentCursor start ) {
    removeFormatOverlay( m_formats->id( "valid" ), start );
}
void TextStd::removeValidOverlay( QDocumentCursor start, QDocumentCursor end ) {
    removeFormatOverlay( m_formats->id( "valid" ), start, end );
}
void TextStd::removeAllOverlays() {
    for( int lineIndex = 0; lineIndex < textStdIn()->document()->lineCount(); lineIndex++ )
        textStdIn()->document()->line( lineIndex ).clearOverlays();
}

void TextStd::copy() {
    textStdIn()->copy();
}
void TextStd::cut() {
    textStdIn()->cut();
}
void TextStd::paste() {
    textStdIn()->paste();
}
void TextStd::selectAll() {
    textStdIn()->selectAll();
}
void TextStd::undo() {
    textStdIn()->undo();
}
void TextStd::redo() {
    textStdIn()->redo();
}
void TextStd::search() {
    textStdIn()->find();
}
void TextStd::searchNext() {
    textStdIn()->findNext();
}
void TextStd::replace() {
    textStdIn()->replace();
}
void TextStd::goToLine() {
    m_gotoLinePanel->show();
}
void TextStd::indentSelection() {
    textStdIn()->indentSelection();
}
void TextStd::unindentSelection() {
    textStdIn()->unindentSelection();
}
void TextStd::commentSelection() {
    textStdIn()->commentSelectedText();
}
void TextStd::uncommentSelection() {
    textStdIn()->uncommentSelectedText();
}
void TextStd::commentLines() {
    textStdIn()->commentSelectedLines();
}
void TextStd::uncommentLines() {
    textStdIn()->uncommentSelectedLines();
}
void TextStd::insert(QString text) {
    textStdIn()->cursor().insertText(text);
}
