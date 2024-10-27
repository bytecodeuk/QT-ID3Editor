/***
 * Filename: cfilelistmodel.cpp
 * Project: ID3Editor.  A Qt/C++ utility to visually edit MP3 ID3 tag information.
 * Description: Model to contain Mp3FileItem s.
 * Author: Andrew Shepherd
 * Date: 09/06/2017
 */

#include <QDir>
#include <QColor>
#include <QFont>
#include <QDebug>
#include <QStringList>
#include <QTableWidgetItem>
#include <QMetaType>
#include <QVariant>
#include <QRegExp>
#include "cfilelistmodel.h"
#include "cstringutils.h"

#include "debughelpers.h"

namespace
{
    const QString kBGColorFilename("#f3ecee");
    const QString kBGColorFileDirectory("#d9d2d4");
    const QString kBGColorNotValid("#f1f361");
    const QString kBGColorChanged("#aaaa00");
    const QString kBGColorDefaultOddLine("#f0f8ff");
    const QString kBGColorDefaultEvenLine("#90ee90");
    const QString kBGColorSelected("#900090");
}


CFileListModel::CFileListModel( const QString & aDirectory, QObject * parent ) :
    QStandardItemModel( parent )
{
    updateFileList( aDirectory, false );
}

void CFileListModel::updateFileList( const QString & aDirectory, bool aScanSubFolders )
{
    m_directory = aDirectory;

    m_Mp3FileItemList.clear();
    addFilesToList( m_directory, aScanSubFolders );
    populateModelFromFilelist();
}

//-------------------------------------------

void CFileListModel::addFilesToList(const QString & aDirectory, bool aScanSubFolders )
{
    QDir dir( aDirectory );
    QFileInfoList fileInfoList = dir.entryInfoList( QDir::Files, QDir::Name );

    foreach( QFileInfo fileInfo, fileInfoList )
    {
        if( fileInfo.suffix().toUpper() == kMP3SuffixUpper )
        {
            CMp3FileItem Mp3FileItem( fileInfo );
            m_Mp3FileItemList.append( Mp3FileItem );
        }

        if( fileInfo.suffix().toUpper() == kFLACSuffixUpper )
        {
            CMp3FileItem Mp3FileItem( fileInfo );
            m_Mp3FileItemList.append( Mp3FileItem );
        }
    }

    if( aScanSubFolders  )
    {
        QFileInfoList dirInfoList = dir.entryInfoList( QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name );

        foreach( QFileInfo dirInfo, dirInfoList )
        {
            addFilesToList( dirInfo.filePath(), aScanSubFolders  );
        }
    }
}

void CFileListModel::setAlternateLineBackgrounds()
{
    QModelIndex setIndex;
    QColor BGColor;

    for( int row = 0; row < m_Mp3FileItemList.count(); row++)
    {
        BGColor = row % 2 ? QColor(kBGColorDefaultOddLine) : QColor(kBGColorDefaultEvenLine);

        for( int col = 0; col < columnCount(); col++ )
        {
            setBackgroundColour( row, col, BGColor );
        }
    }
}

void CFileListModel::toggleTableRowSelection( const QModelIndex & aIndex )
{
    int row = aIndex.row();
    if( row < m_Mp3FileItemList.count())
    {
        CMp3FileItem Mp3FileItem = m_Mp3FileItemList.at( row );
        if( !Mp3FileItem.isSelected() )
        {
            selectLineBackground( row );
            Mp3FileItem.setSelected( true );
            m_Mp3FileItemList.replace( row, Mp3FileItem );
        }
        else
        {
            unselectLineBackground( row );
            Mp3FileItem.setSelected( false );
            m_Mp3FileItemList.replace( row, Mp3FileItem );
        }
    }
}

void CFileListModel::selectLineBackground( int aRow )
{
    QModelIndex setIndex;
    QColor BGColor = QColor(kBGColorSelected);

    for( int col = 0; col < columnCount(); col++ )
    {
        setBackgroundColour( aRow, col, BGColor );
    }
}

void CFileListModel::unselectLineBackground( int aRow )
{
    QModelIndex setIndex;
    QColor BGColor = QColor(kBGColorSelected);

    // set base colour
    BGColor = aRow % 2 ? QColor(kBGColorDefaultOddLine) : QColor(kBGColorDefaultEvenLine);

    for( int col = 0; col < columnCount(); col++ )
    {
        setBackgroundColour( aRow, col, BGColor );
    }

    // Set file and dir backgrounds
    setBackgroundColour( aRow, eTableColumns_FileDirectory, QColor(kBGColorFileDirectory) );
    setBackgroundColour( aRow, eTableColumns_FileName, QColor(kBGColorFilename) );

    // Set data changed background
    BGColor = QColor( kBGColorChanged );
    for( int col = 0; col < columnCount(); col++ )
    {
        setIndex = index( aRow, col, QModelIndex());
        QVariant itemData = data( setIndex, Qt::EditRole );

        CMp3FileItem Mp3FileItem = m_Mp3FileItemList.at(aRow);
        QVariant originalData = Mp3FileItem.getDataForColumn(col);

        if( itemData != originalData)
        {
            setBackgroundColour( aRow, col, BGColor );
        }
    }
}

void CFileListModel::populateModelFromFilelist()
{
    beginResetModel();
    clear();
    setColumnCount( kColumnCount );
    setRowCount(m_Mp3FileItemList.count());

    setAlternateLineBackgrounds();

    for( int row = 0; row < rowCount(); row++)
    {
        CMp3FileItem Mp3FileItem = m_Mp3FileItemList.at( row );

        QModelIndex setIndex;
        for( int col = 0; col < eTableColumns_NumberOfColumns; col++ )
        {
            setIndex =  index( row, col, QModelIndex());
            setData( setIndex, Mp3FileItem.getDataForColumn( col ), Qt::EditRole );
        }

        setBackgroundColour( row, eTableColumns_FileDirectory, QColor( kBGColorFileDirectory ) );
        setBackgroundColour( row, eTableColumns_FileName, QColor( kBGColorFilename ) );
    }

    endResetModel();
}

inline void CFileListModel::setBackgroundColour( const QModelIndex & aModelIndex, const QColor & aBgColour )
{
    setData( aModelIndex, aBgColour, Qt::BackgroundRole );
}

void CFileListModel::setBackgroundColour( int aRow, int aCol, const QColor & aBgColour )
{
    QModelIndex setIndex =  index( aRow, aCol, QModelIndex());
    setBackgroundColour( setIndex, aBgColour );
}

// SLOT
void CFileListModel::dataChanged( const QModelIndex & aIndex, QVariant aData)
{
    setChangedData( aIndex, aData );
}

bool CFileListModel::setChangedData(const QModelIndex & aIndex, QVariant aData)
{
    bool retVal = false;

    if( !aIndex.isValid() || !aData.isValid() )
    {
        return false;
    }

    if( aData != data( aIndex, Qt::EditRole ) )
    {
        setData( aIndex, aData, Qt::EditRole );
        setBackgroundColour( aIndex, QColor( kBGColorChanged ) );
        retVal = true;
    }

    return retVal;
}

bool CFileListModel::writeChangedFiles()
{
    bool retVal( false );
    TMp3FileItemList Mp3FileItemListChangedFiles;
    getFilelistChangedFiles( Mp3FileItemListChangedFiles );

    for( int x = 0; x < Mp3FileItemListChangedFiles.count(); x++ )
    {
        CMp3FileItem Mp3FileItem = Mp3FileItemListChangedFiles.at(x);
        if( Mp3FileItem.writeFile() )
        {
            retVal = true;
        }
    }
    return retVal;
}

void CFileListModel::getFilelistChangedFiles( TMp3FileItemList & aMp3FileItemList )
{
    aMp3FileItemList.clear();

    TMp3FileItemList Mp3FileItemListFromModel;
    getFilelistFromModel( Mp3FileItemListFromModel );

    for( int row = 0; row < m_Mp3FileItemList.count(); row++)
    {
        CMp3FileItem Mp3FileItemOriginal = m_Mp3FileItemList.at( row );
        CMp3FileItem Mp3FileItemFromModel = Mp3FileItemListFromModel.at( row );

        if( Mp3FileItemOriginal != Mp3FileItemFromModel )
        {
            Mp3FileItemFromModel.setOriginalFileInfo( Mp3FileItemOriginal.m_FileInfo );
            Mp3FileItemFromModel.setOriginalMp3Info( Mp3FileItemOriginal.m_Mp3Info );
            aMp3FileItemList.append( Mp3FileItemFromModel );
        }
    }
}

bool CFileListModel::setColumnText( int aColumn, const QString & aText )
{
    bool retVal = false;
    for( int row = 0; row < rowCount(); row++)
    {
        QModelIndex setIndex = index( row, aColumn, QModelIndex() );
        if( setChangedData( setIndex, aText ))
        {
            retVal = true;
        }
    }
    return retVal;
}

bool CFileListModel::autoTrackNumber( int aStartNumber )
{
    bool retVal = false;
    for( int row = 0; row < rowCount(); row++)
    {
        int trackNum = row + aStartNumber;
        QModelIndex setIndex = index( row, eTableColumns_TrackNum, QModelIndex() );
        if( setChangedData( setIndex, trackNum ))
        {
            retVal = true;
        }
    }
    return retVal;
}

bool CFileListModel::filenameAutoFix()
{
    bool retVal = false;

    for( int row = 0; row < rowCount(); row++)
    {
        CMp3FileItem Mp3FileItem = m_Mp3FileItemList.at(row);
        QString filename = Mp3FileItem.getDataForColumn( eTableColumns_FileName ).toString();
        QString artist = Mp3FileItem.getDataForColumn( eTableColumns_Artist ).toString();

        QString newFilename = CStringUtils::fixFilename( filename, artist );

        if( !newFilename.isEmpty() && newFilename != filename )
        {
            QModelIndex setIndex =  index( row, eTableColumns_FileName, QModelIndex());
            setData( setIndex, newFilename, Qt::EditRole );
            setBackgroundColour( setIndex, QColor( kBGColorChanged ) );
            retVal = true;
        }
    }

    return retVal;
}


bool CFileListModel::titleAsFilename()
{
    bool retVal = false;
    for( int row = 0; row < rowCount(); row++)
    {
        QString filename;
        QModelIndex indexFilname = index( row, eTableColumns_FileName, QModelIndex() );
        filename =  data( indexFilname, Qt::EditRole ).toString();

        if( filename.toLower().endsWith(".mp3") || filename.toLower().endsWith(".flac") )
        {
            // Remove extension
            if( filename.toLower().endsWith(".mp3") )
            {
                filename.chop( 4 );
            }
            if( filename.toLower().endsWith(".flac") )
            {
                filename.chop( 5 );
            }

            filename = CStringUtils::stripPreceedingDigitsFromString( filename );

            QModelIndex setIndex = index( row, eTableColumns_TrackTitle, QModelIndex() );
            if( setChangedData( setIndex, filename ))
            {
                retVal = true;
            }
        }
    }
    return retVal;
}

bool CFileListModel::filenameCase()
{
    return setColumnCase( eTableColumns_FileName );
}

bool CFileListModel::titleCase()
{
    return setColumnCase( eTableColumns_TrackTitle );
}

bool CFileListModel::artistCase()
{
    return setColumnCase( eTableColumns_Artist );
}

bool CFileListModel::albumCase()
{
    return setColumnCase( eTableColumns_Album );
}

bool CFileListModel::setAlbumName( const QString & aDirName )
{
    bool retVal = false;

    QString albumName = CStringUtils::stripPreceedingDigitsFromString( aDirName );

    for( int row = 0; row < rowCount(); row++)
    {
        QModelIndex setIndex = index( row, eTableColumns_Album, QModelIndex() );
        if( setChangedData( setIndex, albumName ))
        {
            retVal = true;
        }
    }
    return retVal;
}

bool CFileListModel::setArtistName( const QString & aDirName )
{
    bool retVal = false;

    for( int row = 0; row < rowCount(); row++)
    {
        QModelIndex setIndex = index( row, eTableColumns_Artist, QModelIndex() );
        if( setChangedData( setIndex, aDirName ))
        {
            retVal = true;
        }
    }
    return retVal;
}

bool CFileListModel::setColumnCase( ETableColumns eColumn )
{
    bool retVal = false;
    for( int row = 0; row < rowCount(); row++)
    {
        QModelIndex setIndex = index( row, eColumn, QModelIndex() );
        QString unprocessTitle =  data( setIndex, Qt::EditRole ).toString();
        QString capitalizedTitle = CStringUtils::capitaliseString( unprocessTitle );
        if( setChangedData( setIndex, capitalizedTitle ))
        {
            retVal = true;
        }
    }
    return retVal;
}

bool CFileListModel::setYearFromDirectory()
{
    bool retVal = false;

    if( rowCount() > 0 )
    {
        QModelIndex setIndex = index( 0, eTableColumns_FileDirectory, QModelIndex() );
        QString path = data( setIndex, Qt::EditRole ).toString();
        QDir dir(path);

        QString year = CStringUtils::getYearFromString( dir.dirName() );

        if( !year.isEmpty() )
        {
            retVal = setColumnText( eTableColumns_Year, year );
        }
    }
    return retVal;
}


bool CFileListModel::clearAllComments()
{
    return setColumnText( eTableColumns_Comment, QString("") );
}

QString CFileListModel::getCellText( const QModelIndex & aIndex )
{
    return data( aIndex, Qt::EditRole ).toString();
}

void CFileListModel::getFilelistFromModel( TMp3FileItemList & aMp3FileItemList )
{
    aMp3FileItemList.clear();

    for( int row = 0; row < rowCount(); row++)
    {
        CMp3FileItem aMp3FileItem;
        QModelIndex setIndex;

        setIndex = index( row, eTableColumns_FileDirectory, QModelIndex() );
        QString directory = data( setIndex, Qt::EditRole ).toString();

        setIndex = index( row, eTableColumns_FileName, QModelIndex() );
        QString fileName = data( setIndex, Qt::EditRole ).toString();

        aMp3FileItem.m_FileInfo = QFileInfo( directory, fileName );

        for( int col = eTableColumns_Artist; col < eTableColumns_NumberOfColumns; col ++)
        {
            setIndex = index( row, col, QModelIndex() );
            aMp3FileItem.setDataForColumn( col, data( setIndex, Qt::EditRole ) );
        }

        aMp3FileItemList.append( aMp3FileItem );
    }
}

QVariant CFileListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( role == Qt::DisplayRole && orientation == Qt::Horizontal )
    {
        return getColumnText( section );
    }
    return QVariant();
}

QString CFileListModel::getColumnText( int section ) const
{
    switch( section )
    {
        case eTableColumns_FileDirectory: return  QString( tr("Directory") );
        case eTableColumns_FileName: return  QString( tr("File Name") );
        case eTableColumns_Artist: return  QString( tr("Artist") );
        case eTableColumns_Album: return  QString( tr("Album") );
        case eTableColumns_Year: return  QString( tr("Year") );
        case eTableColumns_Genre: return  QString( tr("Genre") );
        case eTableColumns_TrackNum: return  QString( tr("Num") );
        case eTableColumns_TrackTitle: return  QString( tr("Title") );
        case eTableColumns_Comment: return  QString( tr("Comment") );

        default: return QString();
    }
}

CMp3FileItem CFileListModel::getMp3FileItem( const QModelIndex & aIndex )
{
    return m_Mp3FileItemList.at( aIndex.row() );
}

//-----------------

