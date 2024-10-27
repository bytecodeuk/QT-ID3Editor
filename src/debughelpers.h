/***
 * Filename: debughelpers.h
 * Project: ID3Editor.  A Qt/C++ utility to visually edit MP3 ID3 tag information.
 * Description: Quick and dirty include to dump debug info.
 * Author: Andrew Shepherd
 * Date: 09/06/2017
 */
#ifndef DEBUGHELPERS_H
#define DEBUGHELPERS_H

#include <QDebug>

void DebugIndexAndRole(const QModelIndex &index, int role )
{
    QString sRole;

    switch( role )
    {
        case Qt::DisplayRole: sRole = "Qt::DisplayRole"; break;
        case Qt::DecorationRole: sRole = "Qt::DecorationRole"; break;
        case Qt::EditRole: sRole = "Qt::EditRole"; break;
        case Qt::ToolTipRole: sRole = "Qt::ToolTipRole"; break;
        case Qt::StatusTipRole: sRole = "Qt::StatusTipRole"; break;
        case Qt::WhatsThisRole: sRole = "Qt::WhatsThisRole"; break;
        case Qt::SizeHintRole: sRole = "Qt::SizeHintRole"; break;

        case Qt::FontRole: sRole = "Qt::FontRole"; break;
        case Qt::TextAlignmentRole: sRole = "Qt::TextAlignmentRole"; break;
        case Qt::BackgroundRole: sRole = "Qt::BackgroundRole"; break;
        case Qt::ForegroundRole: sRole = "Qt::ForegroundRole"; break;
        case Qt::CheckStateRole: sRole = "Qt::CheckStateRole"; break;
        case Qt::InitialSortOrderRole: sRole = "Qt::InitialSortOrderRole"; break;

        case Qt::AccessibleTextRole: sRole = "Qt::AccessibleTextRole"; break;
        case Qt::AccessibleDescriptionRole: sRole = "Qt::AccessibleDescriptionRole"; break;

        case Qt::UserRole: sRole = "Qt::UserRole"; break;

        default: sRole = QString("Unknown %1").arg(role); break;
    }

    qDebug() << "Row:" << index.row() << "Col:" << index.column() << sRole ;
}

#endif // DEBUGHELPERS_H
