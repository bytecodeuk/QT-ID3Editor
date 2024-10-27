#include <QDebug>
#include <QDir>

#include "filesmodel.h"

FilesModel::FilesModel( const QString &aDirectory, QObject *parent ) :
    QStringListModel( parent ),
    m_directory( aDirectory )
{
    populateFilelist();
}

void FilesModel::populateFilelist()
{
    QDir dir( m_directory );
    m_fileList = dir.entryList( QDir::Files, QDir::Name );
}

int FilesModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent);
    int fileCount = m_fileList.count();
//    qDebug() << "Count: " << fileCount;
    return fileCount;
}

QVariant FilesModel::data( const QModelIndex &index, int role ) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_fileList.size())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        QString fileName = m_fileList.at( index.row() );
//        qDebug() << "FileName:" << fileName;
        return fileName;
    }
    else
        return QVariant();
}
