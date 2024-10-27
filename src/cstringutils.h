/***
 * Filename: cstringutils.h
 * Project: ID3Editor.  A Qt/C++ utility to visually edit MP3 ID3 tag information.
 * Description: QString manipulation utils.
 * Author: Andrew Shepherd
 * Date: 30/06/2017
 */

#ifndef CSTRINGUTILS_H
#define CSTRINGUTILS_H

#include <QString>

class CStringUtils
{
public:

    static QString fixFilename(const QString & aFilename, const QString & aArtist);
    static QString getYearFromString(const QString & aDirectory);
    static QString stripPreceedingDigitsFromString(const QString & aFilename);
    static QString capitaliseString(const QString & aInput);
};

#endif // CSTRINGUTILS_H
