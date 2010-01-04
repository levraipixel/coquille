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
#include <QStringList>

//Project Libs

//Project Files
#include "filelink.h"

//////////////////
//   FileLink   //
//////////////////

FileLink::FileLink()
        : QFileInfo()
{}

FileLink::FileLink( const QString & file )
        : QFileInfo( file )
{}

FileLink::~FileLink() {}

QString FileLink::name() {
    if(m_displayFull)
        return fullDisplayName();
    else
        return basicDisplayName();
}

bool FileLink::operator==(FileLink &other) const {
    return ( other.absoluteFilePath() == absoluteFilePath() );
}

void FileLink::setFullDisplayed(bool full) {
    m_displayFull = full;
}

QString FileLink::basicDisplayName() {
    return fileName();
}

QString FileLink::fullDisplayName() {
    return fileName()+" ("+absolutePath()+")";
}

//////////////////////
//   FileLinkList   //
//////////////////////

FilesLinkList::FilesLinkList(int size)
        : m_maxSize( size )
{}

FilesLinkList::~FilesLinkList() {}

QStringList FilesLinkList::toPathList() {
    QStringList result;
    for( int i = 0; i < m_recentFiles.size(); i++ )
        result.append( m_recentFiles.at(i)->absoluteFilePath() );
    return result;
}

QStringList FilesLinkList::toDisplayNameList() {
    QStringList result;
    for( int i = 0; i < m_recentFiles.size(); i++ )
        result.append( m_recentFiles.at(i)->name() );
    return result;
}

bool FilesLinkList::isEmpty() {
    return m_recentFiles.isEmpty();
}

FileLink* FilesLinkList::first() {
    return m_recentFiles.first();
}

FileLink* FilesLinkList::at( int index ) {
    return m_recentFiles.at( index );
}

int FilesLinkList::size() {
    return m_recentFiles.size();
}

void FilesLinkList::clear() {
    m_recentFiles.clear();
}

void FilesLinkList::addFile( QString path ) {
    //if no history, nothing removed
    if( !m_maxSize )
        return;

    rmFile( path, false );

    if( m_recentFiles.size() == m_maxSize )
        m_recentFiles.removeLast();
    m_recentFiles.prepend( new FileLink( path ) );

    refreshDisplay( path );

//    qDebug() << "m_recentFiles : " << toPathList();
}

void FilesLinkList::refreshDisplay( QString basicDisplayName ) {
    int prev = -1;
    bool moreThanTwo = false;
    for( int i = 0; i < m_recentFiles.size()-1; i++ ) {
        if( QString( m_recentFiles.at( i )->basicDisplayName()).compare( basicDisplayName ) == 0 ) {
            m_recentFiles.at( i )->setFullDisplayed( false );
            if( prev < 0 ) {
                prev = i;
            }
            else {
                if( !moreThanTwo ) {
                    moreThanTwo = true;
                    m_recentFiles.at( prev )->setFullDisplayed( true );
                }
                m_recentFiles.at( i )->setFullDisplayed( true );
            }
        }
    }
}

void FilesLinkList::rmFile(QString path, bool refresh) {
    FileLink f( path );
    int i = 0;
    while( i < m_recentFiles.size() ) {
        if( *m_recentFiles.at( i ) == f )
            m_recentFiles.removeAt( i );
        else
            i++;
    }
    if( refresh )
        refreshDisplay( f.basicDisplayName() );

//    qDebug() << "m_recentFiles : " << toPathList();
}
