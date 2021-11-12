#ifndef KTOOLS_KFF_RAWSTREAM_H
#define KTOOLS_KFF_RAWSTREAM_H

#include "../file.h"
#include "../log.h"

namespace KTools::Kff {
    class Manager;
    class RawStream
    {
    public:
        RawStream(Manager *man);
        ~RawStream();

        qint64 write(const QByteArray &content);
        QByteArray readAll();
        void seek(const qint64 pos);
        qint64 pos();
        QByteArray read(const qint64 len);

    private:
        Manager *manager;
        qint64 size;
        static const int blockSize;
        QList<qint64> clusters;
        KTools::File *file;
        qint64 position;

        void appendCluster();
    };
}

#endif // KTOOLS_KFF_RAWSTREAM_H
