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
#include "documentationwidget.h"
#include "ui_documentationwidget.h"

/////////////////////////////
//   DocumentationWidget   //
/////////////////////////////

DocumentationWidget::DocumentationWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::DocumentationWidget)
{
    m_ui->setupUi(this);
}

DocumentationWidget::~DocumentationWidget()
{
    delete m_ui;
}

void DocumentationWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DocumentationWidget::showDocumentationAbout( QString keyword ) {
    m_ui->searchText->setText( keyword );
    m_ui->searchPage->setText( "Some documentation on _" + keyword + "_ !!" );
}

void DocumentationWidget::on_searchButton_clicked() {
    showDocumentationAbout( m_ui->searchText->text() );
}
