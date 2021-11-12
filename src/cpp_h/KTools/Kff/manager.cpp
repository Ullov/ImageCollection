#include "manager.h"

const QByteArray KTools::Kff::Manager::signature = "KFFS0000";
const qint64 KTools::Kff::Manager::Offsets::inodes = signature.length();
const qint64 KTools::Kff::Manager::Sizes::signature = Manager::signature.length();
const qint64 KTools::Kff::Manager::Sizes::cluster = 4096;
const qint64 KTools::Kff::Manager::Sizes::inode = 16;

KTools::Kff::Manager::Manager(const QString &path, const OpenMode lMode = OpenMode::Clear)
{
    mode = lMode;
    if (mode == OpenMode::Keep)
        file.open(path, QIODevice::ReadWrite);
    else if (mode == OpenMode::Clear)
        file.open(path, QIODevice::Truncate | QIODevice::ReadWrite);
    constructFs();
}

void KTools::Kff::Manager::constructFs()
{
    file.seek(0);
    file.write(signature);

    QByteArray inodeArea;
    inodeArea.append(160, 0);
    file.write(inodeArea);
    offsets.data = offsets.inodes + inodeArea.length();
}

void KTools::Kff::Manager::intToChar(char result[], const qint64 numb)
{
    for (int i = 0; i < 8; i++)
        result[i] = numb >> (8 - 1 - i) * 8;
}

KTools::Kff::RawStream KTools::Kff::Manager::getStream()
{
    RawStream raw(this);
    return raw;
}

qint64 KTools::Kff::Manager::allocCluster()
{
    clusters.append({(clusters.length() * sizes.cluster) + offsets.data, false});

    char zero[8];
    char singleZero = 0;
    intToChar(zero, 0ll);
    QByteArray content;
    content.append(2, *zero);
    content.append(4080, singleZero);

    file.seek(clusters.last().first);
    file.write(content);

    return clusters.last().first;
}

void KTools::Kff::Manager::writeInode(const qint64 clust)
{
    char addr[8];
    char zero[8];
    intToChar(addr, clust);
    intToChar(zero, 0ll);
    QByteArray content;
    content.append(addr, 8); // First cluster address
    content.append(zero, 8); // Entity size
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
