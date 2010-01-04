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
#include <QtGui/QApplication>
#include <QTranslator>
#include <QSplashScreen>
#include <QDebug>

//Project Libs

//Project Files
#include "mainwindow.h"
#include "settings.h"

//////////////
//   main   //
//////////////

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName( "ENS Lyon" );
    QCoreApplication::setOrganizationDomain( "ens-lyon.fr" );
    QCoreApplication::setApplicationName( "Coquille" );

    QString locale = QLocale::system().name().section( '_', 0, 0 );

    QTranslator translator;
    translator.load( QString( "coquille_" ) + locale, ":/LanguagePrefix/languages" );
    a.installTranslator( &translator );
    //a.processEvents();

    //splashscreen
    QSplashScreen *splash = new QSplashScreen( QPixmap( ":/IconPrefix/images/poussin.png" ) );
    splash->show();

    splash->showMessage( QObject::tr( "Build settings" ), Qt::AlignCenter | Qt::AlignBottom );
    Settings *settings = new Settings();
    settings->readSettingsFile();

    splash->showMessage( QObject::tr( "Build main window" ), Qt::AlignCenter | Qt::AlignBottom );
    MainWindow w(settings);

    splash->finish( &w );
    delete splash;

    w.show();
   
    a.exec();

    qDebug() << "fin de l'application : write settings";
    settings->writeSettingsFile();

    return 0;
}
