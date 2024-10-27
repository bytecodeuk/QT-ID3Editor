/***
 * Filename: ccelldelegate.cpp
 * Project: ID3Editor.  A Qt/C++ utility to visually edit MP3 ID3 tag information.
 * Description: Delegate class to use in editing cells.
 * Author: Andrew Shepherd
 * Date: 09/06/2017
 */

#include <QtWidgets>
#include "ccelldelegate.h"
#include "tablecolumns.h"

// Uncomment line below for event debug output
//#define AS_USE_DEBUG
#ifdef AS_USE_DEBUG
#include <QDebug>
    static int debugCounter = 0;
    #define AS_DEBUG(x) qDebug() << "[DELEGATE]:" << "[" << debugCounter++ << "]" << "(" << __FUNCTION__ << ")" << x;
#else
    #define AS_DEBUG(x)
#endif

CCellDelegate::CCellDelegate(QObject *parent)
        : QItemDelegate(parent)
{
    AS_DEBUG("Constructor")
}

QWidget *CCellDelegate::createEditor(QWidget * aParent,
                                     const QStyleOptionViewItem &,
                                     const QModelIndex & aIndex) const
{
    AS_DEBUG("aIndex" << aIndex)

    // Block Directory and filename columns from being edited.
    if( aIndex.column() == eTableColumns_FileDirectory )
    {
        return NULL;
    }

    QLineEdit *editor = new QLineEdit(aParent);

    // create a completer with the strings in the column as model
    QStringList allStrings;
    for (int i = 1; i<aIndex.model()->rowCount(); i++)
    {
        QString strItem(aIndex.model()->data(aIndex.sibling(i, aIndex.column()),
            Qt::EditRole).toString());

        if (!allStrings.contains(strItem))
            allStrings.append(strItem);
    }

    QCompleter *autoComplete = new QCompleter(allStrings);
    editor->setCompleter(autoComplete);
    connect(editor, &QLineEdit::editingFinished, this, &CCellDelegate::commitAndCloseEditor);
    return editor;
}


void CCellDelegate::setEditorData(QWidget * aEditor,
                                  const QModelIndex & aIndex) const
{
    AS_DEBUG("aIndex" << aIndex)
    QLineEdit *edit = qobject_cast<QLineEdit*>(aEditor);
    if (edit)
    {
        edit->setText(aIndex.model()->data(aIndex, Qt::EditRole).toString());
    }
}


void CCellDelegate::setModelData(QWidget * aEditor,
                                 QAbstractItemModel * aModel,
                                 const QModelIndex & aIndex) const
{
    AS_DEBUG("aIndex" << aIndex)
    QLineEdit *edit = qobject_cast<QLineEdit *>(aEditor);
    if (edit)
    {
        if( aModel->data( aIndex, Qt::EditRole ).toString() != edit->text() )
        {
            emit dataChanged( aIndex, edit->text() );
        }
    }
}


void CCellDelegate::commitAndCloseEditor()
{
    AS_DEBUG("")
    QLineEdit *editor = qobject_cast<QLineEdit *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
