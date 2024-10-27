/***
 * Filename: cmp3fileitem.cpp
 * Project: ID3Editor.  A Qt/C++ utility to visually edit MP3 ID3 tag information.
 * Description: Mp3FileItem contains file details such as QFileInfo and MP3 tag data.
 * Author: Andrew Shepherd
 * Date: 09/06/2017
 */

#include <QTime>
#include <taglib/tag_c.h>

#include "cmp3fileitem.h"
#include "tablecolumns.h"

//#define TEST_MODE

// Uncomment line below for event debug output
#define AS_USE_DEBUG
#ifdef AS_USE_DEBUG
#include <QDebug>
    static int debugCounter = 0;
    #define AS_DEBUG(x) qDebug() << "[FileItem]:" << "[" << debugCounter++ << "]" << "(" << __FUNCTION__ << ")" << x;
#else
    #define AS_DEBUG(x)
#endif

CMp3FileItem::CMp3FileItem( const QFileInfo & aFileInfo )
    : m_selected(false)
{
    readMp3FileInfo( aFileInfo );
}

bool CMp3FileItem::operator ==( const CMp3FileItem & aOther )
{
    if(    m_FileInfo != aOther.m_FileInfo
        || m_Mp3Info  != aOther.m_Mp3Info )
    {
        return false;
    }
    return true;
}

bool CMp3FileItem::Mp3InfoChanged( const CMp3FileItem & aOther )
{
    if( m_Mp3Info  != aOther.m_Mp3Info )
    {
        return false;
    }
    return true;
}

bool CMp3FileItem::fileInfoChanged( const CMp3FileItem & aOther )
{
    if( m_Mp3Info  != aOther.m_Mp3Info )
    {
        return false;
    }
    return true;
}

void CMp3FileItem::setOriginalFileInfo(const QFileInfo & aFileInfo)
{
    m_FileInfoOriginal = aFileInfo;
}

void CMp3FileItem::setOriginalMp3Info(const CMp3Info & aMp3Info)
{
    m_Mp3InfoOriginal = aMp3Info;
}

bool CMp3FileItem::operator !=( const CMp3FileItem & aOther )
{
    return ! (*this == aOther);
}

QVariant CMp3FileItem::getDataForColumn( int aColumn )
{
    QVariant retVal = QVariant();

    switch( aColumn )
    {
        case eTableColumns_FileDirectory:
            retVal = m_FileInfo.path();
            break;

        case eTableColumns_FileName:
            retVal = m_FileInfo.fileName();
            break;

        case eTableColumns_Artist:
            retVal = m_Mp3Info.m_Artist;
            break;

        case eTableColumns_Album:
            retVal = m_Mp3Info.m_Album;
            break;

        case eTableColumns_TrackNum:
            retVal = m_Mp3Info.m_TrackNumber;
            break;

        case eTableColumns_TrackTitle:
            retVal = m_Mp3Info.m_TrackTitle;
            break;

        case eTableColumns_Year:
            retVal = m_Mp3Info.m_Year;
            break;

        case eTableColumns_Genre:
            retVal = m_Mp3Info.m_Genre;
            break;

        case eTableColumns_Comment:
            retVal = m_Mp3Info.m_Comment;
            break;
    }

    return retVal;
}

void CMp3FileItem::setDataForColumn( int aColumn, const QVariant & aData )
{
    switch( aColumn )
    {
//        case eTableColumns_FileDirectory:
//            retVal = m_FileInfo.path();
//            break;

//        case eTableColumns_FileName:
//            retVal = m_FileInfo.fileName();
//            break;

        case eTableColumns_Artist:
            m_Mp3Info.m_Artist = aData.toString();
            break;

        case eTableColumns_Album:
            m_Mp3Info.m_Album  = aData.toString();
            break;

        case eTableColumns_TrackNum:
            m_Mp3Info.m_TrackNumber = aData.toUInt();
            break;

        case eTableColumns_TrackTitle:
            m_Mp3Info.m_TrackTitle = aData.toString();
            break;

        case eTableColumns_Year:
            m_Mp3Info.m_Year = aData.toUInt();
            break;

        case eTableColumns_Genre:
            m_Mp3Info.m_Genre = aData.toString();
            break;

        case eTableColumns_Comment:
            m_Mp3Info.m_Comment = aData.toString();
            break;
    }
}

bool CMp3FileItem::writeFile()
{
    AS_DEBUG("")
    bool fileRemaneResult = false;
    bool Mp3SaveResult = false;

    if( m_FileInfoOriginal != m_FileInfo )
    {
        AS_DEBUG("FileInfo different")
        QFile file( m_FileInfoOriginal.filePath() );
        fileRemaneResult = !file.rename( m_FileInfo.filePath() );
    }

    if( m_Mp3InfoOriginal != m_Mp3Info )
    {
        AS_DEBUG("Mp3Info different")
        Mp3SaveResult = writeMp3FileInfo( m_FileInfo, *this );
    }

    AS_DEBUG("Mp3SaveResult :" << Mp3SaveResult << ", fileRemaneResult :" << fileRemaneResult << ", Overall result" << (Mp3SaveResult | fileRemaneResult) )
    return Mp3SaveResult | fileRemaneResult;
}

void CMp3FileItem::readMp3FileInfo( const QFileInfo & aFileInfo )
{
    m_FileInfo = aFileInfo;

    TagLib_File *file;
    TagLib_Tag *tag;
    const TagLib_AudioProperties *properties;
    taglib_set_strings_unicode( 0 );

    file = taglib_file_new( m_FileInfo.filePath().toLatin1().constData());
    tag = taglib_file_tag( file );

    if(tag != NULL)
    {
        m_Mp3Info.m_TrackTitle      = taglib_tag_title(tag);
        m_Mp3Info.m_Artist          = taglib_tag_artist(tag);
        m_Mp3Info.m_Album           = taglib_tag_album(tag);
        m_Mp3Info.m_Year            = taglib_tag_year(tag);
        m_Mp3Info.m_Comment         = taglib_tag_comment(tag);
        m_Mp3Info.m_TrackNumber     = taglib_tag_track(tag);
        m_Mp3Info.m_Genre           = taglib_tag_genre(tag);
    }

    properties = taglib_file_audioproperties(file);
    if(properties != NULL)
    {
        m_AudioInfo.m_Bitrate       = taglib_audioproperties_bitrate(properties);
        m_AudioInfo.m_SampleRate    = taglib_audioproperties_samplerate(properties);
        m_AudioInfo.m_Channels      = taglib_audioproperties_channels(properties);
        m_AudioInfo.m_Length        = taglib_audioproperties_length(properties);

        int totalSeconds = taglib_audioproperties_length(properties);
        int seconds = totalSeconds % 60;
        int hours   = totalSeconds / (60 * 60);
        int minutes = (totalSeconds - (hours * (60 * 60))) / 60;

        m_AudioInfo.m_LengthText = QString("%1:%2:%3")
                .arg( hours,   2, 10, QChar('0'))
                .arg( minutes, 2, 10, QChar('0'))
                .arg( seconds, 2, 10, QChar('0'));
    }

    taglib_tag_free_strings();
    taglib_file_free(file);
}

bool CMp3FileItem::writeMp3FileInfo( const QFileInfo & aFileInfo, const CMp3FileItem & aInfoToWrite )
{
    TagLib_File *file;
    TagLib_Tag *tag;
    taglib_set_strings_unicode( 0 );

    file = taglib_file_new( aFileInfo.filePath().toLatin1().constData());
    tag = taglib_file_tag( file );

    taglib_tag_set_artist( tag, aInfoToWrite.m_Mp3Info.m_Artist.toLatin1().constData() );

    taglib_tag_set_album( tag, aInfoToWrite.m_Mp3Info.m_Album.toLatin1().constData() );
    taglib_tag_set_title( tag, aInfoToWrite.m_Mp3Info.m_TrackTitle.toLatin1().constData() );
    taglib_tag_set_track( tag, aInfoToWrite.m_Mp3Info.m_TrackNumber );
    taglib_tag_set_comment( tag, aInfoToWrite.m_Mp3Info.m_Comment.toLatin1().constData() );
    taglib_tag_set_genre( tag, aInfoToWrite.m_Mp3Info.m_Genre.toLatin1().constData() );
    taglib_tag_set_year( tag, aInfoToWrite.m_Mp3Info.m_Year );

    bool retVal = true;
#ifndef TEST_MODE
    retVal = taglib_file_save(file);
#endif

    taglib_tag_free_strings();
    taglib_file_free( file );

    return !retVal;
}
