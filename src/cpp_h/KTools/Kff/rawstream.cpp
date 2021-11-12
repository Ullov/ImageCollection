#include "rawstream.h"

#include "manager.h"

const int KTools::Kff::RawStream::blockSize = 4096;

KTools::Kff::RawStream::RawStream(Manager *man)
{
    manager = man;
    file = &manager->file;
    appendCluster();
    manager->writeInode(clusters.last());
    size = 0;
    position = 0;
}

KTools::Kff::RawStream::~RawStream() {}

qint64 KTools::Kff::RawStream::write(const QByteArray &content)
{
    qint64 writed = 0;
    qint64 firstPiece;

    if (size > 4080)
        firstPiece = 4080 - (size % 4080);
    else
        firstPiece = 4080 - size;

    file->seek(clusters.last() + 16 + (4080 - firstPiece));
    if (content.length() < firstPiece)
    {
        file->write(content);
        writed += content.size();
    }
    else
    {
        QByteArray filler = content.mid(0, firstPiece);
        file->write(filler);
        writed = filler.length();
        for (int i = 0; writed < content.length(); i++)
        {
            appendCluster();
            if (content.length() - writed > 4080)
            {
                filler = content.mid(writed, 4080);
            }
            else
            {
                filler = content.mid(writed);
            }
            file->seek(clusters.last() + 16);
            file->write(filler);
            writed += filler.length();
        }
    }
    size += writed;
    return writed;
}

void KTools::Kff::RawStream::appendCluster()
{
    clusters.append(manager->allocCluster());
    if (clusters.length() > 1)
    {
        file->seek(clusters.last()); // Seek and write address of previous cluster in last one
        file->write(clusters[clusters.size() - 2]);
        file->seek(clusters[clusters.size() - 2] + 8); // Seek and write address of last cluster in previous one
        file->write(clusters.last());
    }
}

QByteArray KTools::Kff::RawStream::readAll()
{
    qint64 readed = 0;
    QByteArray result;
    for (int i = 0; i < clusters.length(); i++)
    {
        file->seek(clusters[i] + 16);
        if (size - readed < 4080)
            result += file->read<QByteArray>(size - readed);
        else
            result += file->read<QByteArray>(4080);
        readed = result.size();
    }
    return result;
}

void KTools::Kff::RawStream::seek(const qint64 pos)
{
    position = pos;
}

qint64 KTools::Kff::RawStream::pos()
{
    return position;
}

QByteArray KTools::Kff::RawStream::read(const qint64 len)
{
    if (len + position > size)
    {
        KTools::Log::writeError("Attempt read more than you can. len + position:" + QString::number(len + position) + "  size:" + QString::number(size), "KTools::Kff::RawStream::read()");
        return "Attempt read more than you can. len + position:" + QByteArray::number(len + position) + "  size:" + QByteArray::number(size);
    }
    QByteArray result;
    qint64 localPos;
    qint64 clsNumber;
    qint64 readed = 0;
    if (position > 4080)
    {
        localPos = position % 4080;
        clsNumber = (position - localPos) / 4080;
    }
    else
    {
        localPos = position;
        clsNumber = 0;
    }
    for (int i = 0; readed < len; i++)
    {
        file->seek(clusters[clsNumber] + 16 + localPos);
        if ((len - readed) > (4080 - localPos))
            result.append(file->read<QByteArray>(4080 - localPos));
        else
            result.append(file->read<QByteArray>(len - readed));
        localPos = 0;
        clsNumber++;
        readed = result.size();
    }

    return result;
}
