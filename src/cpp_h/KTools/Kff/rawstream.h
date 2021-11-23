#ifndef KTOOLS_KFF_RAWSTREAM_H
#define KTOOLS_KFF_RAWSTREAM_H

#include "../file.h"
#include "../log.h"

namespace KTools::Kff {
    class Manager;
    class RawStream
    {
    public:
        RawStream(Manager *man, const bool writeInInode = false);
        RawStream(Manager *man, const qint64 lPosition);
        ~RawStream();

        qint64 write(const QByteArray &content);
        QByteArray readAll();
        void seek(const qint64 pos);
        qint64 pos();
        QByteArray read(qint64 len);
        void toEnd();
        qint64 size();
        void resize(const qint64 nsize);

    protected:
        Manager *manager;

    private:
        struct Sizes {
            static const qint64 nextCls = sizeof(qint64); // Next cluster address size
            static const qint64 prevCls = sizeof(qint64); // Previous cluster address size
            static const qint64 rawData; // How much data can be writed in one cluster
            static const qint64 all = 4096;
        };
        static const qint64 dataOffset = 8;

        KTools::File *file;
        qint64 vsize;
        qint64 position;
        QList<qint64> clusters;

        void appendCluster();
        void writeSizeVariable();
    };
}

#endif // KTOOLS_KFF_RAWSTREAM_H
