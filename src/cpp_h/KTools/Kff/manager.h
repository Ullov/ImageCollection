#ifndef KTOOLS_KFF_MANAGER_H
#define KTOOLS_KFF_MANAGER_H

#include "../file.h"
#include "rawstream.h"

namespace KTools::Kff {
    class Manager
    {
    public:
        enum class OpenMode {
            Clear,
            Keep
        };
        Manager();
        Manager(const QString &path, const OpenMode lMode);

        RawStream getStream();
        qint64 allocCluster();
        void writeInode(const qint64 clust);

        KTools::File file;

    private:
        void constructFs();
        void intToChar(char result[], const qint64 numb);

        static const QByteArray signature;

        struct Offsets {
            static const qint64 inodes;
            qint64 data = 0;
        };
        struct Sizes {
            static const qint64 inode;
            static const qint64 cluster;
            static const qint64 signature;
        };

        Sizes sizes;
        Offsets offsets;

        OpenMode mode;
        QList<QPair<qint64, bool>> clusters;
    };
}

#endif // KTOOLS_KFF_MANAGER_H
