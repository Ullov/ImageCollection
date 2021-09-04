#include "file.h"

#include <QFile>
#include "converter.h"
#include <QDir>
#include "log.h"

KTools::File::File() {}

bool KTools::File::open(const QString &path, const QIODevice::OpenMode &flags)
{
    file = new QFile(path);
    if (file->open(flags))
    {
        return true;
    }
    else
        return false;
}

bool KTools::File::seek(const qint64 &pos)
{
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
        KTools::Log::writeError("Write error. data: " + tmp, "KTools::File::write()");
}

template<>
void KTools::File::write(const QByteArray &data)
{
    //file->write(data);
    if (file->write(data) == -1)
        KTools::Log::writeError("Write error. data: " + data, "KTools::File::write()");
}

template<typename T>
T KTools::File::read(const qint64 &lenght)
{
    return KTools::Converter::byteArrayToT<T>(file->read(lenght));
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
    }
    else
    {
        //Logging::writeError("Can't open file. directory: " + directory + "; fileName: " + fileName, "NativeFs::writeFile()");
        QFile::FileError ferr = file.error();
        file.close();
        return false;
    }
    file.close();
    return true;
}

void KTools::File::makePath(const QString &path)
{
    QDir dir = QDir(path);
    bool qexi = dir.exists();
    if (!qexi)
        qexi = dir.mkpath("."); // if derictory does not exist make it
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
    if (!dirExist(newPath))
        makePath(newPath);

    return QFile::copy(oldPathToFile, newPath + '/' + newFileName);
}

template<typename T>
T KTools::File::readFile(const QString &directory, const QString &fileName, const QIODevice::OpenMode &flags)
{
    if (!fileExist(directory + '\\' + fileName))
        return T();

    QFile rFile(directory + '\\' + fileName);
    if (!rFile.open(flags))
        return T();

    if (typeid (T) != typeid (QByteArray))
        return KTools::Converter::byteArrayToT<T>(rFile.readAll());

    return rFile.readAll();
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

template QString KTools::File::readFile<QString>(const QString&, const QString&, const QIODevice::OpenMode&);
template QByteArray KTools::File::readFile<QByteArray>(const QString&, const QString&, const QIODevice::OpenMode&);
