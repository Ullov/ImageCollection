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

        void appendCluster();
        qint64 getAbolutePos();

    private:
        KTools::File *file;
        qint64 vsize;
        bool inodeWrited;
        qint64 position;
        QList<qint64> clusters;
        static const int blockSize;

        qint64 trueSeek(const qint64 posi); // This function sets pos of file and returns pos

    };
}

#endif // KTOOLS_KFF_RAWSTREAM_H
