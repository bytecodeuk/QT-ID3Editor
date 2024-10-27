#ifndef FILESMODEL_H
#define FILESMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QString>
#include <QStringList>
#include <QStringListModel>

class FilesModel : public QStringListModel
{
public:
    FilesModel( const QString & aDirectory, QObject *parent );

    QString m_directory;
    QStringList m_fileList;

    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;

protected:
    void populateFilelist();

};

#endif // FILESMODEL_H
