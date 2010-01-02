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

#ifndef DOCUMENTATIONWIDGET_H
#define DOCUMENTATIONWIDGET_H

/////////////////
//   Headers   //
/////////////////

//C++ Libs

//Qt Libs
#include <QWidget>

//Project Libs

//Project Files

//File Classes
class DocumentationWidget;

//File NameSpace
namespace Ui {
    class DocumentationWidget;
}

/////////////////////////////
//   DocumentationWidget   //
/////////////////////////////

class DocumentationWidget : public QWidget {
    Q_OBJECT
public:
    DocumentationWidget(QWidget *parent = 0);
    ~DocumentationWidget();
    void showDocumentationAbout( QString keyword );

protected slots:
    void on_searchButton_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DocumentationWidget *m_ui;
};

#endif // DOCUMENTATIONWIDGET_H
