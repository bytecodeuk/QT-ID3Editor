/***
 * Filename: mainwindow.cpp
 * Project: ID3Editor.  A Qt/C++ utility to visually edit MP3 ID3 tag information.
 * Description: UI and 'control'.
 * Author: Andrew Shepherd
 * Date: 09/06/2017
 */

#include <QDebug>
#include <QFileSystemModel>
#include <QTableView>
#include <QModelIndexList>
#include <QFile>
#include <QDateTime>
#include <QLocale>
#include <QDirModel>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ccelldelegate.h"
#include "tablecolumns.h"
#include "cstringutils.h"

// Test:
// Add button to set album case

// TODO:
// Genre combo with selectable genre.
// Test genre against list for validity.
// App Icon
// Rescan sub folders after save
// Set column to cell button text should update after editing current cell.
// Set changed data colour in table
//  - Yellow if dubious validity, Artist not == parent folder, Title not == Filename, 0 value, Genre not in known list
// Alternate line colouring
// Add 'Play mp3' ability
// Add delete file ability
// Add rescan current folder
// Change CFileListModel so as not to need directory at construction
// Set start dir
// Stop dir changing if unsaved data
// Delete key to blank selected cells
// Make 'files' tree splitter bigger
// remove 'Name' header from file tree
// support 'file name' edits


// Uncomment line below for event debug output
#define AS_USE_DEBUG
#ifdef AS_USE_DEBUG
#include <QDebug>
    static int debugCounter = 0;
    #define AS_DEBUG(x) qDebug() << "[MAIN]:" << "[" << debugCounter++ << "]" << "(" << __FUNCTION__ << ")" << x;
#else
    #define AS_DEBUG(x)
#endif


namespace
{
#if defined(PATH_CONFIG1)
//    const QString kStartDir("M:/!Music From Steve/AC-DC/Back in Black [DualDisc] Disc 1");
//        const QString kStartDir("M:/tmp/01234 Back in Black [DualDisc] Disc 1");
//    const QString kStartDir("M:/S7/Music/ZZ Top/1983 Eliminator");
    const QString kStartDir("M:/!Sorted/Pink Floyd/1994 The Division Bell");
#elif defined(PATH_CONFIG2)
//    const QString kStartDir("F:/Music/Iron Maiden/01 Iron Maiden");
//    const QString kStartDir("F:/Music/Guns N' Roses/Appetite for Destruction");
    const QString kStartDir("F:/Music/ZZ Top/Eliminator");
#else
    const QString kStartDir("");
#endif

}

int numChildDirs( const QString aPath )
{
    QDir dir( aPath );
    QStringList dirList = dir.entryList( QDir::AllDirs | QDir::NoDotAndDotDot );
    return dirList.count();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pFileSystemModel( NULL ),
    m_pFileListModel( NULL )
{
    AS_DEBUG("Constructor")
    ui->setupUi(this);

    setupTableView();
    setupDirList();

    QList<int> sizes_all;
    sizes_all << 150 << 700;
    ui->splitter_all->setSizes( sizes_all );

    QList<int> sizes_files;
    sizes_files << 600 << 100;
    ui->splitter_files->setSizes( sizes_files );
}

MainWindow::~MainWindow()
{
    AS_DEBUG("Destructor")
    delete ui;
    delete m_pFileListModel;
    delete m_pFileSystemModel;
}

void MainWindow::setupDirList()
{
    AS_DEBUG("")

    m_pFileSystemModel = new QDirModel;

    m_pFileSystemModel->setFilter( QDir::AllDirs | QDir::NoDotAndDotDot );
    m_pFileSystemModel->setSorting( QDir::Name | /*QDir::Reversed |*/ QDir::IgnoreCase );

    ui->treeViewDir->setModel( m_pFileSystemModel );
    ui->treeViewDir->setSortingEnabled( true );

    ui->treeViewDir->setColumnHidden( 1, true) ;
    ui->treeViewDir->setColumnHidden( 2, true) ;
    ui->treeViewDir->setColumnHidden( 3, true) ;

    connect( ui->treeViewDir, SIGNAL( clicked(const QModelIndex &) ), this, SLOT( dir_clicked(const QModelIndex &) ) );
}

void MainWindow::setupTableView()
{
    AS_DEBUG("")
    m_pFileListModel = new CFileListModel( kStartDir, this );
    ui->tableViewFiles->setSizeAdjustPolicy(QTableView::AdjustToContents);
    ui->tableViewFiles->setModel( m_pFileListModel );
    m_lastClickedPath = kStartDir;
    setPath( kStartDir );

    ui->tableViewFiles->setItemDelegate(new CCellDelegate());
    resizeTableCells();
    setCellEditTriggers();
    ui->tableViewFiles->setColumnHidden( 0, true) ;
    ui->tableViewFiles->setEnabled(true);

    // Connect table click
    connect( ui->tableViewFiles, SIGNAL( clicked(const QModelIndex &) ), this, SLOT( table_clicked(const QModelIndex &) ) );

    // Connext dataChanged
    connect( ui->tableViewFiles->itemDelegate(), SIGNAL(dataChanged(const QModelIndex &, QVariant)), this, SLOT(dataChanged(const QModelIndex &, QVariant)));
    connect( ui->tableViewFiles->itemDelegate(), SIGNAL(dataChanged(const QModelIndex &, QVariant)), m_pFileListModel, SLOT(dataChanged(const QModelIndex &, QVariant)));

}

void MainWindow::setCellEditTriggers()
{
    if( ui->checkBox_editCell->isChecked() )
    {
        ui->tableViewFiles->setEditTriggers(QAbstractItemView::AllEditTriggers);
    }
    else
    {
        ui->tableViewFiles->setEditTriggers(
                                    //QAbstractItemView::CurrentChanged
                                    /*|*/ QAbstractItemView::DoubleClicked
                                    | QAbstractItemView::SelectedClicked
                                    | QAbstractItemView::EditKeyPressed);
    }
}

// SLOT
void MainWindow::dataChanged( const QModelIndex & aIndex, QVariant aData)
{
    AS_DEBUG("")
    Q_UNUSED(aIndex)
    Q_UNUSED(aData)
    ui->pushButton_save->setEnabled( true );
    ui->pushButton_revert->setEnabled( true );
    resizeTableCells();
}

void MainWindow::table_clicked(const QModelIndex & aIndex)
{
    AS_DEBUG( "Clicked Row:" << aIndex.row() << ", Col:" << aIndex.column())
    QApplication::setOverrideCursor(Qt::WaitCursor);
    updateAudioStats( aIndex );
    updateColumnAsCellButton( aIndex );
//    m_pFileListModel->toggleTableRowSelection( aIndex );
    QApplication::restoreOverrideCursor();
}

void MainWindow::updateColumnAsCellButton( const QModelIndex & aIndex )
{
    int column = aIndex.column();

    if( column == eTableColumns_Artist
            || column == eTableColumns_Album
            || column == eTableColumns_Year
            || column == eTableColumns_Genre
            || column == eTableColumns_Comment
            )
    {
        QString columnText = m_pFileListModel->getColumnText( column );
        QString cellText = m_pFileListModel->getCellText( aIndex);
        QString buttonText = QString("Set %1 to %2").arg( columnText ).arg( cellText );
        ui->pushButton_setColumn->setText( buttonText );
        ui->pushButton_setColumn->setEnabled( true );
    }
    else
    {
        ui->pushButton_setColumn->setEnabled( false );
    }
}

void MainWindow::setPathEditorPath( const QString & aPath )
{
    ui->lineEdit_path->setText( QDir::toNativeSeparators( aPath ) );
}

const QString MainWindow::getPathEditorPath()
{
    return QDir::fromNativeSeparators( ui->lineEdit_path->text() );
}

void MainWindow::dir_clicked(const QModelIndex & aIndex)
{
    AS_DEBUG("aIndex" << aIndex)

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QString path = m_pFileSystemModel->filePath(aIndex);

    if( path != m_lastClickedPath )
    {
        m_lastClickedPath = path;
        setPath( path );
    }
    QApplication::restoreOverrideCursor();
}

void MainWindow::setPath( const QString & aPath, bool aScanSubFolders )
{
    AS_DEBUG("aPath:" << aPath)

    if( QFile::exists( aPath ) )
    {
        setPathEditorPath( aPath );
        m_pFileListModel->updateFileList( aPath, aScanSubFolders );
        ui->tableViewFiles->update();
        resizeTableCells();
        ui->pushButton_save->setEnabled( false );
        ui->pushButton_revert->setEnabled( false );

        checkActionsValidWRTDirectory( aPath );
    }
    ui->tableViewFiles->setColumnHidden( 0, !ui->checkBox_showDir->isChecked()) ;
}

void MainWindow::checkActionsValidWRTDirectory( const QString & aPath )
{
    bool actionButtonsEnable = numChildDirs(aPath) == 0 ? true : false;

    // [set Artist, Set Album, Auto-Fix, guess year].
    //ui->pushButton_setArtistFromParent->setEnabled( actionButtonsEnable );
    ui->pushButton_setAlbumFromDirectory->setEnabled( actionButtonsEnable );
    ui->pushButton_AutoFixAll->setEnabled( actionButtonsEnable );
    ui->pushButton_YearFromDirectory ->setEnabled( actionButtonsEnable );
}

void MainWindow::updateAudioStats( const QModelIndex & aIndex )
{
    CMp3FileItem Mp3FileItem = m_pFileListModel->getMp3FileItem( aIndex );

    ui->label_bitrate->setText( QString::number( Mp3FileItem.m_AudioInfo.m_Bitrate ) );
    ui->label_channels->setText( QString::number( Mp3FileItem.m_AudioInfo.m_Channels ) );
    ui->label_fileDate->setText( Mp3FileItem.m_FileInfo.lastModified().toString("dd/MM/yyyy"));
    ui->label_fileSize->setText( QLocale(QLocale::English).toString( double(Mp3FileItem.m_FileInfo.size()) , 'f', 0) );
    ui->label_length->setText( Mp3FileItem.m_AudioInfo.m_LengthText );
    ui->label_sampleRate->setText( QString::number( Mp3FileItem.m_AudioInfo.m_SampleRate ));
}

void MainWindow::resizeTableCells()
{
    ui->tableViewFiles->resizeColumnsToContents();
    ui->tableViewFiles->resizeRowsToContents();
}

//-----------------------------------------------------------------------------
// App Buttons
void MainWindow::on_pushButtonExit_clicked()
{
    close();
}

void MainWindow::on_pushButton_save_clicked()
{
    // get save list from model.  Save list is derived from comparing tableModel with m_Mp3FileItemList to find differences.
    //
    QApplication::setOverrideCursor(Qt::WaitCursor);

    bool writeResult = m_pFileListModel->writeChangedFiles();
    setPath( getPathEditorPath() );

    QApplication::restoreOverrideCursor();

    if( writeResult )
    {
        // TODO: display error message
         QMessageBox::critical( this, tr("Error"), tr("There was an error writing file(s).") );
    }
}

void MainWindow::on_pushButton_revert_clicked()
{
    setPath( getPathEditorPath() );
}

void MainWindow::on_pushButton_goPath_clicked()
{
    QString path = getPathEditorPath();
    if( path != m_lastClickedPath )
    {
        m_lastClickedPath = path;
        setPath( path );
    }
}

void MainWindow::on_pushButton_scanSubFolders_clicked()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    setPath( getPathEditorPath(), true );
    QApplication::restoreOverrideCursor();
}

void MainWindow::on_pushButton_refresh_clicked()
{
    m_pFileSystemModel->refresh();
}

//-----------------------------------------------------------------------------
// Check boxes
void MainWindow::on_checkBox_editCell_clicked()
{
    setCellEditTriggers();
}

void MainWindow::on_checkBox_showDir_clicked()
{
    ui->tableViewFiles->setColumnHidden( 0, !ui->checkBox_showDir->isChecked()) ;
}

//-----------------------------------------------------------------------------
// Edit Buttons

void MainWindow::on_pushButton_FilenameAutoFix_clicked()
{
    if( m_pFileListModel->filenameAutoFix() )
    {
        dataChanged( QModelIndex(), QVariant() );
    }
}

void MainWindow::on_pushButton_FilenameCase_clicked()
{
    AS_DEBUG("")
    if( m_pFileListModel->filenameCase() )
    {
        dataChanged( QModelIndex(), QVariant() );
    }
}

void MainWindow::on_pushButton_setArtistFromParent_clicked()
{
    AS_DEBUG("")
    QString path = getPathEditorPath();
    QDir dir( path );

    QStringList dirList = dir.path().split('/');

    QString artist;
    if( dirList.count() > 2 && numChildDirs( path ) == 0 )
    {
        artist = dirList.at(dirList.count()-2 );
    }
    else
    {
        artist = dir.dirName();
    }

    if( !artist.isEmpty() && artist != "." )
    {
        if( m_pFileListModel->setArtistName( artist ) )
        {
            dataChanged( QModelIndex(), QVariant() );
        }
    }
}

void MainWindow::on_pushButton_artistCase_clicked()
{
    AS_DEBUG("")
    if( m_pFileListModel->artistCase() )
    {
        dataChanged( QModelIndex(), QVariant() );
    }
}

void MainWindow::on_pushButton_setAlbumFromDirectory_clicked()
{
    QDir dir( getPathEditorPath() );

    if( m_pFileListModel->setAlbumName( dir.dirName() ) )
    {
        dataChanged( QModelIndex(), QVariant() );
    }
}

void MainWindow::on_pushButton_setAlbumCase_clicked()
{
    AS_DEBUG("")
    if( m_pFileListModel->albumCase() )
    {
        dataChanged( QModelIndex(), QVariant() );
    }
}

void MainWindow::on_pushButton_setTitleAsFilename_clicked()
{
    AS_DEBUG("")
    if( m_pFileListModel->titleAsFilename() )
    {
        dataChanged( QModelIndex(), QVariant() );
    }
}

void MainWindow::on_pushButton_setTitleCase_clicked()
{
    AS_DEBUG("")
    if( m_pFileListModel->titleCase() )
    {
        dataChanged( QModelIndex(), QVariant() );
    }
}

void MainWindow::on_pushButton_setColumn_clicked()
{
    QModelIndexList selectedModelIndexList = ui->tableViewFiles->getSelectedIndexes();

    if( selectedModelIndexList.count() == 1 )
    {
        QModelIndex ind = selectedModelIndexList.at(0);
        int column = ind.column();
        if( m_pFileListModel->setColumnText( column, m_pFileListModel->getCellText( ind ) ) )
        {
            dataChanged( QModelIndex(), QVariant() );
        }
    }
}

void MainWindow::on_pushButton_setAutoTrackNumber_clicked()
{
    AS_DEBUG("")
    int startNumber = 1;
    QModelIndexList selectedModelIndexList = ui->tableViewFiles->getSelectedIndexes();
    if( selectedModelIndexList.count() == 1 )
    {
        QModelIndex ind = selectedModelIndexList.at(0);
        if( ind.column() == eTableColumns_TrackNum )
        {
            QString cellContents = m_pFileListModel->getCellText( ind );
            bool ok;
            int val = cellContents.toInt(&ok);
            if( ok )
            {
                startNumber = val;
            }
        }
    }

    if( m_pFileListModel->autoTrackNumber( startNumber ) )
    {
        dataChanged( QModelIndex(), QVariant() );
    }
}

void MainWindow::on_pushButton_YearFromDirectory_clicked()
{
    if( m_pFileListModel->setYearFromDirectory() )
    {
        dataChanged( QModelIndex(), QVariant() );
    }
}

void MainWindow::on_pushButton_clearComments_clicked()
{
    AS_DEBUG("")
    if( m_pFileListModel->clearAllComments() )
    {
        dataChanged( QModelIndex(), QVariant() );
    }
}

void MainWindow::on_pushButton_AutoFixAll_clicked()
{
    on_pushButton_setArtistFromParent_clicked();
    on_pushButton_artistCase_clicked();
    on_pushButton_setAlbumFromDirectory_clicked();
    on_pushButton_setAlbumCase_clicked();
    on_pushButton_setTitleAsFilename_clicked();
    on_pushButton_setTitleCase_clicked();
    on_pushButton_setAutoTrackNumber_clicked();
    on_pushButton_clearComments_clicked();
    on_pushButton_YearFromDirectory_clicked();
}

//-----------------------------------------------------------------------------


void MainWindow::on_pushButton_folderFix_clicked()
{
    QDir currentDir( m_lastClickedPath );

    QStringList childDirList = currentDir.entryList( QDir::AllDirs | QDir::NoDotAndDotDot );

    foreach( QString orgDirName, childDirList )
    {
        QString newDirName = CStringUtils::fixFilename( orgDirName, QString() );

        AS_DEBUG("orgDirName :" << orgDirName << ", newDirName :" << newDirName );

        if( newDirName != orgDirName )
        {
            QDir dir;
            if( !dir.rename( m_lastClickedPath + '/' + orgDirName, m_lastClickedPath + '/' + newDirName ) )
            {
//                throw Exception( tr("move failed") );
            }
        }
    }
}
