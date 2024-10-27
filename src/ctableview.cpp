/***
 * Filename: ctableview.cpp
 * Project: ID3Editor.  A Qt/C++ utility to visually edit MP3 ID3 tag information.
 * Description: Extends QTableView to allow access to selected cells.
 * Author: Andrew Shepherd
 * Date: 09/06/2017
 */

#include "ctableview.h"

#include <QModelIndexList>

CTableView::CTableView( QWidget * aParent ) :
    QTableView( aParent )
{}

QModelIndexList CTableView::getSelectedIndexes() const
{
    return selectedIndexes();
}

void CTableView::selectionChanged(const QItemSelection & aSelected, const QItemSelection & aDeselected)
{
    QTableView::selectionChanged( aSelected, aDeselected );
}

void CTableView::currentChanged(const QModelIndex & aCurrent, const QModelIndex & aPrevious)
{
    QTableView::currentChanged( aCurrent, aPrevious );
}
