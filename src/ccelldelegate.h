/***
 * Filename: ccelldelegate.h
 * Project: ID3Editor.  A Qt/C++ utility to visually edit MP3 ID3 tag information.
 * Description: Delegate class to use in editing cells.
 * Author: Andrew Shepherd
 * Date: 09/06/2017
 */

#ifndef CCELLDELEGATE_H
#define CCELLDELEGATE_H

#include <QItemDelegate>

class CCellDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    CCellDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *aParent, const QStyleOptionViewItem &,
                          const QModelIndex &aIndex) const Q_DECL_OVERRIDE;
    void setEditorData(QWidget *aEditor, const QModelIndex &aIndex) const Q_DECL_OVERRIDE;
    void setModelData(QWidget *aEditor, QAbstractItemModel *aModel,
                      const QModelIndex &aIndex) const Q_DECL_OVERRIDE;

signals:
    void dataChanged( QModelIndex aIndex, QVariant aData ) const ;

private slots:
    void commitAndCloseEditor();

};

#endif // CCELLDELEGATE_H
