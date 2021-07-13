#ifndef KTOOLS_FILEIDENTIFIER_H
#define KTOOLS_FILEIDENTIFIER_H

#include "../KTools/file.h"

namespace KTools
{
    class FileIdentifier
    {
    public:
        FileIdentifier();

        static QStringList identifyFileFromFileSystem(const QString &path);
        static QStringList identifyFileFromString(const QByteArray &file);

    private:
        static QStringList fileTypeSelector(const QByteArray &bytes);
        static QByteArray cutQString(const int &from, const int &lenghtCuttedString, const QByteArray &string);
    };
}

#endif // KTOOLS_FILEIDENTIFIER_H
