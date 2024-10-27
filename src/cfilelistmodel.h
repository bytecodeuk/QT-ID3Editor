/***
 * Filename: cfilelistmodel.h
 * Project: ID3Editor.  A Qt/C++ utility to visually edit MP3 ID3 tag information.
 * Description: Model to contain Mp3FileItem s.
 * Author: Andrew Shepherd
 * Date: 09/06/2017
 */


#ifndef CFILELISTMODEL_H
#define CFILELISTMODEL_H

#include <QAbstractItemModel>
#include <QStandardItemModel>

#include "cmp3fileitem.h"
#include "tablecolumns.h"


class CFileListModel : public QStandardItemModel // QAbstractTableModel
{
    Q_OBJECT

    typedef QList<CMp3FileItem> TMp3FileItemList;

public:
    CFileListModel( const QString & aDirectory, QObject * parent = 0 );

    QString m_directory;

    void updateFileList(const QString & aDirectory, bool scanSubFolders);
    CMp3FileItem getMp3FileItem(const QModelIndex &aIndex);
    void getFilelistChangedFiles(TMp3FileItemList & aMp3FileItemList);
    bool clearAllComments();

    QString getColumnText(int section) const;
    QString getCellText(const QModelIndex & aIndex);
    bool setColumnText(int aColumn, const QString & aText);
    bool autoTrackNumber(int aStartNumber);
    bool titleAsFilename();
    bool titleCase();
    bool albumCase();
    bool setAlbumName(const QString & aDirName);
    bool setArtistName(const QString & aDirName);
    bool setYearFromDirectory();
    bool artistCase();
    void toggleTableRowSelection(const QModelIndex &aIndex);
    bool filenameAutoFix();
    bool writeChangedFiles();
    bool filenameCase();

public slots:
    void dataChanged( const QModelIndex & aIndex, QVariant aData);

protected:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    TMp3FileItemList m_Mp3FileItemList;

    void getFilelistFromModel(TMp3FileItemList &aMp3FileItemList);
    void populateModelFromFilelist();
    void addFilesToList(const QString & aDirectory, bool aScanSubFolders );
    bool setChangedData(const QModelIndex & aIndex, QVariant aData);
    bool setColumnCase(ETableColumns eColumn);
    void setAlternateLineBackgrounds();
    void selectLineBackground(int aRow);
    void unselectLineBackground(int aRow);
    void setBackgroundColour(int aRow, int aCol, const QColor & aBgColour);
    void setBackgroundColour(const QModelIndex & aModelIndex, const QColor & aBgColour);
};

#endif // CFILELISTMODEL_H
