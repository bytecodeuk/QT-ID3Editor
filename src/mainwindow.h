/***
 * Filename: mainwindow.h
 * Project: ID3Editor.  A Qt/C++ utility to visually edit MP3 ID3 tag information.
 * Description: UI and 'control'.
 * Author: Andrew Shepherd
 * Date: 09/06/2017
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "cfilelistmodel.h"
#include "ctableview.h"

class QDirModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void updateAudioStats(const QModelIndex &aIndex);
public slots:
    void dir_clicked( const QModelIndex & index );
    void dataChanged( const QModelIndex & aIndex, QVariant aData );
    void table_clicked( const QModelIndex & aIndex );

private slots:
    void on_checkBox_showDir_clicked();
    void on_checkBox_editCell_clicked();

    void on_pushButtonExit_clicked();
    void on_pushButton_save_clicked();
    void on_pushButton_revert_clicked();
    void on_pushButton_goPath_clicked();
    void on_pushButton_scanSubFolders_clicked();
    void on_pushButton_refresh_clicked();

    void on_pushButton_FilenameAutoFix_clicked();
    void on_pushButton_FilenameCase_clicked();
    void on_pushButton_setArtistFromParent_clicked();
    void on_pushButton_artistCase_clicked();
    void on_pushButton_setAlbumFromDirectory_clicked();
    void on_pushButton_setAlbumCase_clicked();
    void on_pushButton_setTitleAsFilename_clicked();
    void on_pushButton_setTitleCase_clicked();
    void on_pushButton_setColumn_clicked();
    void on_pushButton_setAutoTrackNumber_clicked();
    void on_pushButton_clearComments_clicked();
    void on_pushButton_YearFromDirectory_clicked();
    void on_pushButton_AutoFixAll_clicked();
    void on_pushButton_folderFix_clicked();

private:
    Ui::MainWindow *ui;
    QDirModel * m_pFileSystemModel;
    CFileListModel * m_pFileListModel;
    QString m_lastClickedPath;

private:
    void setupTable();
    void setupDirList();
    void setupTableView();
    void setPath(const QString & aPath, bool aScanSubFolders = false );
    void setCellEditTriggers();
    void updateColumnAsCellButton(const QModelIndex & aIndex);
    void resizeTableCells();
    void setPathEditorPath(const QString & aPath);
    const QString getPathEditorPath();
    void checkActionsValidWRTDirectory(const QString & aPath);
};

#endif // MAINWINDOW_H
