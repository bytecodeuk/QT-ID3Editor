#ifndef TABLECOLUMNS_H
#define TABLECOLUMNS_H

namespace
{
    typedef enum ETableColumns
    {
        eTableColumns_FileDirectory = 0,
        eTableColumns_FileName,
        eTableColumns_Artist,
        eTableColumns_Album,
        eTableColumns_TrackNum,
        eTableColumns_TrackTitle,
        eTableColumns_Year,
        eTableColumns_Genre,
        eTableColumns_Comment,
        eTableColumns_NumberOfColumns,
    } ETableColumns;

    int kColumnCount = eTableColumns_NumberOfColumns;

    const QString kMP3SuffixUpper("MP3");
    const QString kFLACSuffixUpper("FLAC");
}


#endif // TABLECOLUMNS_H
