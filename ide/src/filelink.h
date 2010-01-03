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

/*! \brief A representation of a file

It provides an easy way to display it in a list, such as a list of opened documents, or a list of recently edited documents.
*/
class FileLink : public QFileInfo {
public:
    FileLink();
    FileLink( const QString & file );
    ~FileLink();

    /*! Chooses whether the name has to short or full */
    void setFullDisplayed( bool full = false );
    /*! returns the name of the file */
    QString name();
    /*! returns the short name of the file */
    QString basicDisplayName();
    /*! returns the full name fo the file */
    QString fullDisplayName();

    /*! compares 2 FileLinks by their short names */
    bool operator==( FileLink &other ) const;

protected:
    bool m_displayFull;
};

//////////////////////
//   FileLinkList   //
//////////////////////

/*! \brief A convenient file list */
class FilesLinkList {
public:
    FilesLinkList( int size );
    ~FilesLinkList();

    /*! \brief Add a file to the list. */
    void addFile( QString path );
    /*! Return true if the list is empty. */
    bool isEmpty();
    /*! Return the first file of the list. */
    FileLink* first();
    /*! \brief Clear the list. */
    void clear();
    /*! \brief The number of files in the list */
    int size();
    /*!
    The file at a given position.

    \param index The position of the file in the list
    */
    FileLink* at( int index );
    /*! A list of paths to the files */
    QStringList toPathList();

protected:
    QStringList toDisplayNameList();
    void refreshDisplay( QString basicDisplayName );
    void rmFile( QString path, bool refresh = true );

    int m_maxSize;
    QList<FileLink*> m_recentFiles;
};

#endif // FILELINK_H
