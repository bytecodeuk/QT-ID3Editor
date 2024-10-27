/***
 * Filename: cstringutils.cpp
 * Project: ID3Editor.  A Qt/C++ utility to visually edit MP3 ID3 tag information.
 * Description: QString manipulation utils.
 * Author: Andrew Shepherd
 * Date: 30/06/2017
 */

#include <QRegExp>
#include <QStringList>

#include "cstringutils.h"


QString CStringUtils::fixFilename( const QString & aFilename, const QString & aArtist )
{
    QString retVal( aFilename );
    retVal = retVal.trimmed();

    // Get number / year and remove from string
    QRegExp rx("^[0-9]{1,4}[ -.]*");

    rx.indexIn( retVal );
    QString leadingNum = rx.cap(0);
    leadingNum = leadingNum.remove(QRegExp("[ -.]"));
    retVal = retVal.remove( rx );

    // Remove artist in string
    retVal = retVal.remove( aArtist, Qt::CaseInsensitive );

    // Remove all non-alpha chars from beginning
    rx.setPattern("^\\W*");
    retVal = retVal.remove( rx );

    retVal = leadingNum + " " + retVal;

    return retVal;
}

QString CStringUtils::getYearFromString( const QString & aDirectory )
{
    // Get 4 digit year from beginning of dir
    QRegExp rx("^[0-9]{4,4} *");
    rx.indexIn( aDirectory );
    QString year = rx.cap(0);

    // Get 4 digit year from end of dir
    if( year.isEmpty() )
    {
        rx.setPattern("$ [0-9]{4,4}");
        year = rx.cap(0);
    }

    // Get 4 digit year in () from anywhere in string
    if( year.isEmpty() )
    {
        rx.setPattern("\\([0-9]{4,4}\\)");
        year = rx.cap(0);
    }

    // Get 4 digit year in  from anywhere in string
    if( year.isEmpty() )
    {
        rx.setPattern("\\[[0-9]{4,4}\\]");
        year = rx.cap(0);
    }

    // Remove white space from beginning and end of string
    year = year.trimmed();

    return year;
}

QString CStringUtils::stripPreceedingDigitsFromString( const QString & aFilename )
{
    QRegExp rx("^[0-9]{1,4} *");
    QString filename( aFilename );
    filename = filename.remove( rx );
    return filename;
}

QString CStringUtils::capitaliseString( const QString & aInput )
{
    static char * kExcudedWordArray[] = {
      (char *)"a",  (char *)"of", (char *)"to", (char *)"is", (char *)"the", (char *)"at", (char *)"and"
    };
    const int kExcludedWordArraySize = sizeof(kExcudedWordArray) / sizeof(char *);

    static char * kAllCapsWordArray[] = {
      (char *)"TV", (char *)"ZZ"
    };
    const int kAllCapsWordArraySize = sizeof(kAllCapsWordArray) / sizeof(char *);


    QStringList wordList = aInput.split(' ');
    QString str = wordList.at(0);
    bool ok;
    QString leadingDigits;
    str.toInt( &ok );

    if( ok )
    {
        leadingDigits = wordList.at(0);
        wordList.removeFirst();
    }

    for( int wordNumber = 0; wordNumber < wordList.count(); wordNumber++ )
    {
        str = wordList.at(wordNumber);
        str = str.toLower();
        bool wordExcluded = false;

        for( int excludedWordNumber = 0; excludedWordNumber < kExcludedWordArraySize; excludedWordNumber++ )
        {
            // Is word in excluded list?
            if( str.toLatin1() == kExcudedWordArray[ excludedWordNumber ] )
            {
                wordExcluded = true;
                break;
            }
        }

        // Capitize first char inside opening bracket.
        if( str.length() > 1 )
        {
            QChar possBracket = str.at( 0 );
            if( possBracket == '(' || possBracket == '[' || possBracket == '{' )
            {
                str = str.replace( 1, 1, str[1].toUpper());
            }
        }

        // Capitalize first char in word.
        if( !wordExcluded || wordNumber == 0 )
        {
            str = str.replace( 0, 1, str[0].toUpper());
        }

        for( int allCapsWordNumber = 0; allCapsWordNumber < kAllCapsWordArraySize; allCapsWordNumber++ )
        {
            // Is word in all caps list?
            if( str.toLatin1().toUpper() == kAllCapsWordArray[ allCapsWordNumber ] )
            {
                str = str.toUpper();
                break;
            }
        }

        wordList.replace( wordNumber, str);
    }

    QString output;

    if( ok )
    {
        output = QString("%1 ").arg( leadingDigits );
    }

    for( int x = 0; x < wordList.count(); x++ )
    {
        output.append( wordList.at(x) );

        if( x < wordList.count() - 1)
        {
            output.append(" ");
        }
    }

    return output;
}
