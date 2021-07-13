#ifndef KTOOLS_FILE_H
#define KTOOLS_FILE_H

#include <QIODevice>
#include <QtGlobal>

class QFile;

namespace KTools
{
    class File
    {
    public:
        File();

        bool open(const QString &path, const QIODevice::OpenMode &flags);
        bool seek(const qint64 &pos);
        bool toEnd();
        qint64 size();
        qint64 pos();
        template<typename T>
        void write(const T &data);
        template<typename T>
        T read(const qint64 &lenght);
        template<typename T>
        T read();
        static bool writeFile(const QByteArray &data, const QString &directory, const QString &fileName, const QIODevice::OpenMode &flags = QIODevice::WriteOnly);
        static void makePath(const QString &path);
        static bool fileExist(const QString &path);
        static bool dirExist(const QString &path);
        static bool copyFile(const QString &oldPathToFile, const QString &newPath, const QString &newFileName);
        template<typename T>
        static T readFile(const QString &directory, const QString &fileName, const QIODevice::OpenMode &flags = QIODevice::ReadOnly);
        bool atEnd();
        bool resize(const qint64 &localSize);

    private:
        QFile *file;
    };
}

#endif // KTOOLS_FILE_H
