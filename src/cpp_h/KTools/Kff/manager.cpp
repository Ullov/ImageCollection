#include "manager.h"

#include "rawstream.h"
#include "fixedtypes.h"
#include "variabletypes.h"

KTools::Kff::Manager::Manager(const QString &path, const OpenMode lMode = OpenMode::Clear)
{
    if (!KTools::File::fileExist(path))
        mode = OpenMode::Clear;
    else
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
    if (mode == OpenMode::Clear)
    {
        file.seek(0);
        file.write<QByteArray>(signature);

        QByteArray inodeArea;
        inodeArea.append(Sizes::allInodes, '\0');
        file.write(inodeArea);
        numbers = new FixedTypes(this);
        strs = new VariableTypes(this);
        defaultStream = new RawStream(this, true);
    }
    else if (mode == OpenMode::Keep)
    {
        file.seek(0);
        if (file.read<QByteArray>(Sizes::signature) != signature)
        {
            KLOG_ERROR("Failed to recognize signature.");
            return;
        }
        QList<qint64> list = readInodes();
        numbers = new FixedTypes(this, list[0]);
        strs = new VariableTypes(this, list[1]);
        defaultStream = new RawStream(this, list[2]);
    }
}

KTools::Kff::RawStream* KTools::Kff::Manager::getDefaultStream()
{
    return defaultStream;
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

void KTools::Kff::Manager::writeInode(const qint64 clust)
{
    QByteArray content;
    content.append(KTools::Converter::toByteArray(clust)); // First cluster address
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

void KTools::Kff::Manager::addClusterPos(const qint64 position)
{
    clusters.append({position, false});
    //std::sort(clusters.begin(), clusters.end());
}

KTools::Kff::RawStream KTools::Kff::Manager::getNewStream()
{
    RawStream stream(this, false);
    return stream;
}

QList<qint64> KTools::Kff::Manager::readInodes()
{
    QList<qint64> list;
    for (int i = 0; i < Sizes::allInodes; i += Sizes::inode)
    {
        file.seek(i + offsets.inodes);
        list.append(file.read<qint64>());
    }
    return list;
}
