/***
 * Filename: cmp3fileitem.h
 * Project: ID3Editor.  A Qt/C++ utility to visually edit MP3 ID3 tag information.
 * Description: Mp3FileItem contains file details such as QFileInfo and MP3 tag data.
 * Author: Andrew Shepherd
 * Date: 09/06/2017
 */

#ifndef CMP3FILEITEM_H
#define CMP3FILEITEM_H

#include <QObject>
#include <QTableWidgetItem>
#include <QList>
#include <QFileInfo>
#include <QVariant>
#include <QMetaType>
#include <QString>

class CMp3Info
{
public:
    CMp3Info() : m_TrackNumber(0), m_Year(0) {}

    QString         m_Artist;
    QString         m_Album;
    QString         m_TrackTitle;
    unsigned int    m_TrackNumber;
    QString         m_Comment;
    QString         m_Genre;
    unsigned int    m_Year;

    bool operator ==( const CMp3Info & aOther )
    {
        if(         m_Artist        != aOther.m_Artist
                ||  m_Album         != aOther.m_Album
                ||  m_TrackTitle    != aOther.m_TrackTitle
                ||  m_TrackNumber   != aOther.m_TrackNumber
                ||  m_Comment       != aOther.m_Comment
                ||  m_Genre         != aOther.m_Genre
                ||  m_Year          != aOther.m_Year )
        {
            return false;
        }
        return true;
    }

    bool operator !=( const CMp3Info & aOther ) { return ! (*this == aOther); }
};
Q_DECLARE_METATYPE( CMp3Info )

class CAudioInfo
{
public:
    CAudioInfo() : m_Bitrate(-1), m_SampleRate(-1), m_Channels(-1), m_Length(-1) {}

    int     m_Bitrate;
    int     m_SampleRate;
    int     m_Channels;
    int     m_Length;
    QString m_LengthText;

    bool operator ==( const CAudioInfo & aOther )
    {
        if(         m_Bitrate       != aOther.m_Bitrate
                ||  m_SampleRate    != aOther.m_SampleRate
                ||  m_Channels      != aOther.m_Channels
                ||  m_Length        != aOther.m_Length)
        {
            return false;
        }
        return true;
    }

    bool operator !=( const CAudioInfo & aOther ) { return ! (*this == aOther); }
};
Q_DECLARE_METATYPE( CAudioInfo )


class CMp3FileItem : public QTableWidgetItem
{
public:
    CMp3FileItem() : m_selected(false) {}

    CMp3FileItem(const QFileInfo & fileInfo);

    QFileInfo   m_FileInfo;
    CMp3Info    m_Mp3Info;
    CAudioInfo  m_AudioInfo;

    QFileInfo   m_FileInfoOriginal;
    CMp3Info    m_Mp3InfoOriginal;

    virtual bool writeFile();

    // TODO: Move these functions to file to see if it fixes clang warning:
    // Warning:  has no out-of-line virtual method definitions; its vtable will be emitted in every translation unit
    virtual bool operator ==( const CMp3FileItem & aOther );
    virtual bool operator !=( const CMp3FileItem & aOther );

    bool Mp3InfoChanged(const CMp3FileItem & aOther);
    bool fileInfoChanged(const CMp3FileItem & aOther);

    QVariant getDataForColumn(int aColumn);
    void setDataForColumn(int aColumn, const QVariant & aData);

    bool isSelected() { return m_selected; }
    void setSelected( bool aSelect ) { m_selected = aSelect; }

    void setOriginalFileInfo( const QFileInfo & aFileInfo);
    void setOriginalMp3Info(const CMp3Info & aMp3Info);

private:
    bool m_selected;
    void readMp3FileInfo(const QFileInfo & aFileInfo);

    // aFileInfo is file to write to, aInfoToWrite is info to write to file.
    bool writeMp3FileInfo(const QFileInfo & aFileInfo, const CMp3FileItem &aInfoToWrite);
};

Q_DECLARE_METATYPE( CMp3FileItem )

#endif // CMP3FILEITEM_H
