#include "manager.h"


KTools::Kff::Manager::Manager(const QString &path, const OpenMode lMode = OpenMode::Clear)
{
    mode = lMode;
    if (mode == OpenMode::Keep)
        file.open(path, QIODevice::ReadWrite);
    else if (mode == OpenMode::Clear)
        file.open(path, QIODevice::Truncate | QIODevice::ReadWrite);
    constructFs();
}

KTools::Kff::Manager::~Manager()
{
    delete numbers;
    delete strs;
}

void KTools::Kff::Manager::constructFs()
{
    file.seek(0);
    file.write<QByteArray>(signature);

    QByteArray inodeArea;
    inodeArea.append(160, 0);
    file.write(inodeArea);
    offsets.data = offsets.inodes + inodeArea.length();
    numbers = new FixedTypes(this);
    strs = new VariableTypes(this);
}

KTools::Kff::RawStream KTools::Kff::Manager::getStream()
{
    RawStream raw(this, true);
    return raw;
}

qint64 KTools::Kff::Manager::allocCluster()
{
    for (int i = 0; i < clusters.length(); i++)
    {
        if (clusters[i].second == true)
        {
            clusters[i].second = false;
            return clusters[i].first;
        }
    }
    clusters.append({(clusters.length() * sizes.cluster) + offsets.data, false});

    QByteArray content;
    content.append(Sizes::cluster, '\0');

    file.seek(clusters.last().first);
    file.write(content);

    return clusters.last().first;
}

void KTools::Kff::Manager::writeInode(const qint64 clust, const qint64 size)
{
    if (size == -1)
    {
        QByteArray content;
        content.append(KTools::Converter::toByteArray(clust)); // First cluster address
        content.append(KTools::Converter::toByteArray<qint64>(0ll)); // Entity size
        file.seek(offsets.inodes);
        for (int i = 0; ;i++)
        {
            if (file.read<qint64>() == 0)
            {
                qint64 pos = offsets.inodes + (i * sizes.inode);
                file.seek(pos); // Seeks current inode
                break;
            }
            file.seek(offsets.inodes + ((i + 1) * sizes.inode)); // Seeks next inode
        }
        file.write(content);
    }
    else
    {
        for (int i = 0; file.pos() < offsets.data; i++)
        {
            file.seek(offsets.inodes + (i * sizes.inode));
            if (file.read<qint64>() == clust)
            {
                file.seek(offsets.inodes + (i * sizes.inode) + 16);
                file.write<qint64>(size);
            }
        }
    }
}

KTools::Kff::FixedTypes* KTools::Kff::Manager::getNumbers()
{
    return numbers;
}

void KTools::Kff::Manager::freeCluster(const qint64 cls)
{
    QByteArray content;
    content.append(4096, char(0));
    file.seek(cls);
    file.write(content);
    for (int i = 0; i < clusters.length(); i++)
    {
        if (clusters[i].first == cls)
        {
            if (clusters.length() == (i + 1))
            {
                clusters.removeLast();
                file.resize(file.size() - 4096);
            }
            else
                clusters[i].second = true;

            break;
        }
    }
}

KTools::Kff::VariableTypes* KTools::Kff::Manager::getStrings()
{
    return strs;
}

QByteArray KTools::Kff::Manager::makePointer(const PointerType type, const qint64 position)
{
    QByteArray pointer;
    pointer.append(KTools::Converter::toByteArray(static_cast<qint8>(type)));
    pointer.append(KTools::Converter::toByteArray(position));
    return pointer;
}
