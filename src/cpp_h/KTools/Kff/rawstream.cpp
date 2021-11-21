#include "rawstream.h"

#include "manager.h"

const qint64 KTools::Kff::RawStream::Sizes::rawData = 4080;

KTools::Kff::RawStream::RawStream(Manager *man, const bool writeInInode)
{
    manager = man;
    file = &manager->file;
    appendCluster();

    inodeWrited = writeInInode;
    if (writeInInode)
        manager->writeInode(clusters.last());

    vsize = 0;
    position = 0;
}

KTools::Kff::RawStream::~RawStream() {}

qint64 KTools::Kff::RawStream::write(const QByteArray &content)
{
    qint64 writed = 0;
    qint64 firstPiece;

    if (position > Sizes::rawData)
        firstPiece = Sizes::rawData - (position % Sizes::rawData);
    else
        firstPiece = Sizes::rawData - position;

    file->seek(clusters.last() + 16 + (Sizes::rawData - firstPiece));
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
            if (content.length() - writed > Sizes::rawData)
            {
                filler = content.mid(writed, Sizes::rawData);
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
    if ((position + writed) > size())
        vsize = position + writed;
    position += writed;
    writeSizeVariable();
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
        if (size() - readed < Sizes::rawData)
            result += file->read<QByteArray>(size() - readed);
        else
            result += file->read<QByteArray>(Sizes::rawData);
        readed = result.size();
    }
    return result;
}

void KTools::Kff::RawStream::seek(const qint64 pos)
{
    position = pos + dataOffset;
}

qint64 KTools::Kff::RawStream::pos()
{
    return position - dataOffset;
}

QByteArray KTools::Kff::RawStream::read(qint64 len)
{
    if (len + position > size())
        len -= (len + position) - size();

    QByteArray result;
    qint64 localPos;
    qint64 clsNumber;
    qint64 readed = 0;
    if (position > Sizes::rawData)
    {
        localPos = position % Sizes::rawData;
        clsNumber = (position - localPos) / Sizes::rawData;
    }
    else
    {
        localPos = position;
        clsNumber = 0;
    }
    for (int i = 0; readed < len; i++)
    {
        file->seek(clusters[clsNumber] + 16 + localPos);
        if ((len - readed) > (Sizes::rawData - localPos))
            result.append(file->read<QByteArray>(Sizes::rawData - localPos));
        else
            result.append(file->read<QByteArray>(len - readed));
        localPos = 0;
        clsNumber++;
        readed = result.size();
    }

    return result;
}

void KTools::Kff::RawStream::toEnd()
{
    position = size();
}

qint64 KTools::Kff::RawStream::size()
{
    return vsize;
}

void KTools::Kff::RawStream::resize(const qint64 nsize)
{
    qint64 diff = nsize - size();
    if (diff > 0)
    {
        qint64 modulo = vsize % Sizes::rawData;
        vsize -= modulo;
        diff += modulo;
        modulo = diff % Sizes::rawData;
        qint64 clsNumb = (diff - modulo) / Sizes::rawData;
        for (int i = 0; i < clsNumb; i++)
        {
            appendCluster();
        }
        vsize = ((clusters.length() - 1) * Sizes::rawData) + modulo;
    }
    else
    {
        diff = diff * -1;
        qint64 modulo = Sizes::rawData - (vsize % Sizes::rawData);
        vsize -= modulo;
        diff +=modulo;
        modulo = diff % Sizes::rawData;
        qint64 clsNumb = (diff - modulo) / Sizes::rawData;
        for (int i = 0; i < clsNumb; i++)
        {
            manager->freeCluster(clusters.last());
            clusters.removeLast();
        }
        vsize = (clusters.length() * Sizes::rawData) - modulo;
    }
    writeSizeVariable();
}

void KTools::Kff::RawStream::writeSizeVariable()
{
    file->seek(clusters.first() + 16);
    file->write(size());
}
