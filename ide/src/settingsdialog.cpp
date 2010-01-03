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
#include <QFontComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

//Project Libs

//Project Files
#include "mainwindow.h"
#include "settings.h"
#include "settingsdialog.h"
#include "ui_settingsdialog.h"

////////////////////////
//   SettingsDialog   //
////////////////////////

SettingsDialog::SettingsDialog( Settings *settings ) :
    QDialog(0),
    m_ui(new Ui::SettingsDialog),
    m_settings(settings)
{
    m_ui->setupUi(this);
    QStringList actionNames( m_settings->mainWindowModifiableShortcuts() );
    for( int i = 0; i < actionNames.size(); i++ )
        addShortcutForm( actionNames.at(i), m_settings->mainWindowShortcuts.value( actionNames.at(i) ) );

    addFontForm( m_settings->m_mainWindow->menuWidget(), tr( "Menus" ) );
    addFontForm( m_settings->m_mainWindow->tabManager(), tr( "Document Editor" ) );

    m_ui->checkScrollText->setChecked( m_settings->textStdScrollCursorOnSend );
    m_ui->checkLatexMode->setChecked( m_settings->textStdLatexMode );

    connect(m_ui->buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(apply()));
    setWindowIcon( m_settings->m_mainWindow->getAction( "settings" )->icon() );
}

SettingsDialog::~SettingsDialog()
{
    delete m_ui;
}

void SettingsDialog::addShortcutForm( QString actionName, QKeySequence defaultValue ) {
    m_shortcutForm.append( new ShortcutForm( actionName, m_settings->getActionText( actionName ), defaultValue, m_ui->shortcutsForm, m_shortcutForm.size() ) );
}
void SettingsDialog::addFontForm( QWidget *widget, QString label ) {
    m_fontsForm.append( new FontForm( widget, label, widget->font(), m_ui->fontsForm, m_fontsForm.size() ) );
}

void SettingsDialog::readShortcuts() {
    for( int i = 0; i < m_shortcutForm.size(); i++ ) {
        m_settings->mainWindowShortcuts.remove( m_shortcutForm.at(i)->actionName() );
        m_settings->mainWindowShortcuts.insert( m_shortcutForm.at(i)->actionName(), m_shortcutForm.at(i)->readShortcut() );
    }
}

void SettingsDialog::apply() {
    qDebug()<< "apply";
    readShortcuts();
    m_settings->textStdScrollCursorOnSend = m_ui->checkScrollText->isChecked();
    m_settings->textStdLatexMode = m_ui->checkLatexMode->isChecked();
}

void SettingsDialog::changeEvent(QEvent *e) {
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SettingsDialog::accept() {
    apply();
    QDialog::accept();
}

//////////////////////
//   ShortcutForm   //
//////////////////////

ShortcutForm::ShortcutForm( QString actionName, QString actionText, QKeySequence defaultValue, QGridLayout *shortcutsForm, int line )
        : m_actionName(actionName), m_defaultValue(defaultValue), m_shortcutsForm(shortcutsForm), m_line(line)
{
    QWidget* layout = m_shortcutsForm->layout()->widget();
    m_label = new QLabel( layout );
    m_label->setText( actionText );
    //m_label->setObjectName( QString::fromUtf8( "label" ) );
    m_shortcutsForm->addWidget( m_label, m_line, 0, 1, 1);

    m_lineEdit = new QLineEdit( layout );
    //nextShortcut->setObjectName(QString::fromUtf8("nextShortcut"));
    m_shortcutsForm->addWidget( m_lineEdit, m_line, 1, 1, 1);

    m_pushButton = new QPushButton( layout );
    m_pushButton->setText( "Default" );
    //nextDefault->setObjectName(QString::fromUtf8("nextDefault"));
    m_shortcutsForm->addWidget( m_pushButton, m_line, 2, 1, 1);

    m_lineEdit->setText( defaultValue.toString() );
    connect( m_pushButton, SIGNAL(clicked()), this, SLOT(defaultValue()) );
}
ShortcutForm::~ShortcutForm() {
    delete m_label;
    delete m_lineEdit;
    delete m_pushButton;
}
QKeySequence ShortcutForm::readShortcut() {
    return QKeySequence( m_lineEdit->text() );
}
QString ShortcutForm::actionName() {
    return m_actionName;
}
void ShortcutForm::defaultValue() {
    m_lineEdit->setText( m_defaultValue.toString() );
}

//////////////////
//   FontForm   //
//////////////////

FontForm::FontForm( QWidget *widget, QString label, QFont defaultValue, QGridLayout *fontsForm, int line )
        : m_widget(widget), m_defaultValue(defaultValue), m_fontsForm(fontsForm), m_line(line)
{
    QWidget* layout = m_fontsForm->layout()->widget();

    m_label = new QLabel( layout );
    m_label->setText( label );
    m_fontsForm->addWidget( m_label, m_line, 0, 1, 1);

    m_family = new QFontComboBox( widget );
    m_fontsForm->addWidget( m_family, m_line, 1, 1, 1);

    m_size = new QComboBox( layout );
    m_size->insertItem( 0, "10" );
    m_size->insertItem( 1, "12" );
    m_size->insertItem( 2, "14" );
    m_size->insertItem( 3, "16" );
    m_fontsForm->addWidget( m_size, m_line, 2, 1, 1);
    refreshSize();

    m_bold = new QCheckBox( layout );
    m_bold->setText( "Bold" );
    m_fontsForm->addWidget( m_bold, m_line, 3, 1, 1);
    refreshBold();

    m_italic = new QCheckBox( layout );
    m_italic->setText( "Italic" );
    m_fontsForm->addWidget( m_italic, m_line, 4, 1, 1);
    refreshItalic();

    m_pushButton = new QPushButton( layout );
    m_pushButton->setText( "Default" );
    m_fontsForm->addWidget( m_pushButton, m_line, 5, 1, 1);

    connect( m_pushButton, SIGNAL(clicked()), this, SLOT(defaultValue()) );
    connect( m_family, SIGNAL(currentFontChanged(QFont)), this, SLOT(changeFont(QFont)));
    connect( m_size, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeSize(QString)));
    connect( m_bold, SIGNAL(clicked(bool)), this, SLOT(changeBold(bool)));
    connect( m_italic, SIGNAL(clicked(bool)), this, SLOT(changeItalic(bool)));
}
FontForm::~FontForm() {
    delete m_family;
    delete m_label;
    delete m_bold;
    delete m_italic;
    delete m_size;
}
void FontForm::defaultValue() {
    QFontInfo fontInfo( m_defaultValue );
    m_bold->setChecked( fontInfo.bold() );
    m_italic->setChecked( fontInfo.italic() );
    m_size->setCurrentIndex( m_size->findText( QString::number( fontInfo.pixelSize() ) ) );
    m_family->setCurrentFont( m_defaultValue );
    m_widget->setFont( m_defaultValue );
}
void FontForm::changeFont(QFont font) {
    QFont _font(font);
    _font.setPixelSize( m_size->currentText().toInt() );
    _font.setBold( m_bold->isChecked() );
    _font.setItalic( m_italic->isChecked() );
    m_widget->setFont(_font);
}
void FontForm::refreshSize() {
    int size = m_widget->fontInfo().pixelSize();
    //qDebug() << "wanted size : " << size;
    if( m_size->findText( QString::number(size) ) < 0 ) {
      //qDebug() << "not found ! add it" << size;
        m_size->insertSeparator( -1 );
        m_size->insertItem( -1, QString::number(size) );
    }
    m_size->setCurrentIndex( m_size->findText( QString::number(size) ) );
}
void FontForm::changeSize( QString size ) {
    QFont f(m_widget->font());
    f.setPixelSize( size.toInt() );
    changeFont(f);
}
void FontForm::refreshBold() {
    m_bold->setChecked( m_widget->fontInfo().bold() );
}
void FontForm::changeBold( bool b ) {
    QFont f(m_widget->font());
    f.setBold(b);
    changeFont(f);
}
void FontForm::refreshItalic() {
    m_italic->setChecked( m_widget->fontInfo().italic() );
}
void FontForm::changeItalic( bool b ) {
    QFont f(m_widget->font());
    f.setItalic(b);
    changeFont(f);
}
