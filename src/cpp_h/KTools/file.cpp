#include "file.h"

#include <QFile>
#include "converter.h"
#include <QDir>
#include "log.h"

KTools::File::File()
{
    file = NULL;
}

KTools::File::~File()
{
    if (file != NULL)
        delete file;
}

bool KTools::File::open(const QString &path, const QIODevice::OpenMode &flags)
{
    if (file != NULL)
        delete file;

    file = new QFile(path);
    if (file->open(flags))
    {
        return true;
    }
    else
    {
        KLOG_ERROR("Failed to open file. path: " + path);
        return false;
    }
}

bool KTools::File::seek(const qint64 &pos)
{
    if (file->seek(pos))
        return true;
    else
    {
        KLOG_ERROR("Seek failed. pos: " + QString::number(pos));
        return false;
    }
    return file->seek(pos);
}

qint64 KTools::File::size()
{
    return file->size();
}

qint64 KTools::File::pos()
{
    return file->pos();
}

template<typename T>
void KTools::File::write(const T &data)
{
    QByteArray tmp = KTools::Converter::toByteArray(data);
    if (file->write(tmp) == -1)
        KLOG_ERROR("Write error. data: " + tmp);
}

template<>
void KTools::File::write(const QByteArray &data)
{
    if (file->write(data) == -1)
        KLOG_ERROR("Write error. data: " + data);
}

template<typename T>
T KTools::File::read(const qint64 &lenght)
{
    QByteArray content = file->read(lenght);
    return KTools::Converter::byteArrayToT<T>(content);
}

template<typename T>
T KTools::File::read()
{
    return KTools::Converter::byteArrayToT<T>(file->read(sizeof(T)));
}

bool KTools::File::writeFile(const QByteArray &data, const QString &directory, const QString &fileName, const QIODevice::OpenMode &flags)
{
    QString correctPath = directory;
    correctPath.replace("\\", "/").replace("//", "/");
    if (correctPath[correctPath.size() - 1] == "/")
        correctPath.chop(1);

    makePath(correctPath);
    QFile file(correctPath + '/' + fileName);
    if (file.open(flags))
    {
        file.write(data);
        file.close();
        return true;
    }
    else
    {
        KLOG_ERROR("Can't open file. full path: " + correctPath + '/' + fileName);
        file.close();
        return false;
    }
}

void KTools::File::makePath(const QString &path)
{
    if (!dirExist(path))
    {
        QDir dir(path);
        if (!dir.mkpath("."))
        {
            KLOG_ERROR("Can't open make path. path: " + path);
        }
    }
}

bool KTools::File::fileExist(const QString &path)
{
    QFileInfo checkFile(path);
    if (checkFile.exists() && checkFile.isFile())
        return true;
    else
        return false;
}

bool KTools::File::dirExist(const QString &path)
{
    QFileInfo checkFile(path);
    if (checkFile.exists() && checkFile.isDir())
        return true;
    else
        return false;
}

bool KTools::File::toEnd()
{
   return seek(size());
}

bool KTools::File::atEnd()
{
    return file->atEnd();
}

bool KTools::File::resize(const qint64 &localSize)
{
    return file->resize(localSize);
}

bool KTools::File::copyFile(const QString &oldPathToFile, const QString &newPath, const QString &newFileName)
{
    makePath(newPath);
    if (QFile::copy(oldPathToFile, newPath + '/' + newFileName))
        return true;
    else
    {
        KLOG_ERROR("Can't open make path. oldPathToFile: " + oldPathToFile + ";   newPath: " + newPath + ";   newFileName: " + newFileName);
        return false;
    }
}

template<typename T>
T KTools::File::readFile(const QString &path, const QIODevice::OpenMode &flags)
{
    if (!fileExist(path))
    {
        KLOG_ERROR("File does not exist or it's not file. path: " + path);
        return T();
    }

    QFile rFile(path);
    if (!rFile.open(flags))
    {
        KLOG_ERROR("Failed to open file. path: " + path);
        return T();
    }

    if (typeid (T) != typeid (QByteArray))
        return KTools::Converter::byteArrayToT<T>(rFile.readAll());

    return rFile.readAll();
}

bool KTools::File::isFile(const QString &path)
{
    QFileInfo file(path);
    return file.isFile();
}

void KTools::File::close()
{
    file->close();
}

template void KTools::File::write<qint16>(const qint16&);
template void KTools::File::write<qint64>(const qint64&);

template qint8 KTools::File::read<qint8>(const qint64&);
template qint16 KTools::File::read<qint16>(const qint64&);
template qint64 KTools::File::read<qint64>(const qint64&);
template QByteArray KTools::File::read<QByteArray>(const qint64&);

template qint64 KTools::File::read<qint64>();
template quint64 KTools::File::read<quint64>();
template qint16 KTools::File::read<qint16>();

template QString KTools::File::readFile<QString>(const QString&, const QIODevice::OpenMode&);
template QByteArray KTools::File::readFile<QByteArray>(const QString&, const QIODevice::OpenMode&);
