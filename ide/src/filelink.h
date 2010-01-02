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

#ifndef FILELINK_H
#define FILELINK_H

/////////////////
//   Headers   //
/////////////////

//C++ Libs

//Qt Libs
#include <QFileInfo>

//Project Libs

//Project Files

//File Classes
class FileLink;
class FilesLinkList;

//File NameSpace

//////////////////
//   FileLink   //
//////////////////

class FileLink : public QFileInfo {
    public:
        FileLink();
        FileLink( const QString & file );
        ~FileLink();

        QString name();
        bool operator==( FileLink &other ) const;
        void setFullDisplayed( bool full = false );

        QString basicDisplayName();
        QString fullDisplayName();

    private:
        bool m_displayFull;
};

//////////////////////
//   FileLinkList   //
//////////////////////

class FilesLinkList {
    public:
        FilesLinkList( int size );
        ~FilesLinkList();

        QStringList toPathList();
        QStringList toDisplayNameList();
        bool isEmpty();
        FileLink* first();
        FileLink* at( int index );
        int size();
        void clear();
        void addFile( QString path );
        void refreshDisplay( QString basicDisplayName );
        void rmFile( QString path, bool refresh = true );

    private:
        int m_maxSize;
        QList<FileLink*> m_recentFiles;
};

#endif // FILELINK_H
