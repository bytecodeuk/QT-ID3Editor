/***
 * Filename: ctableview.h
 * Project: ID3Editor.  A Qt/C++ utility to visually edit MP3 ID3 tag information.
 * Description: Extends QTableView to allow access to selected cells.
 * Author: Andrew Shepherd
 * Date: 09/06/2017
 */

#ifndef CTABLEVIEW_H
#define CTABLEVIEW_H

#include <QObject>
#include <QTableView>

class CTableView : public QTableView
{
    Q_OBJECT

public:
    CTableView( QWidget * parent = 0 );

    QModelIndexList getSelectedIndexes() const;
protected:
    void selectionChanged(const QItemSelection &aSelected, const QItemSelection &aDeselected);
protected slots:
    void currentChanged(const QModelIndex &aCurrent, const QModelIndex &aPrevious);
};

#endif // CTABLEVIEW_H
