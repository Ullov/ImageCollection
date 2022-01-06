#ifndef KTOOLS_KFF_FILE_H
#define KTOOLS_KFF_FILE_H

#include <QVariantMap>
#include "rawstream.h"
#include "nameinfo.h"

namespace KTools::Kff {
    class MetainfoFs;
    class File : private RawStream
    {
    public:
        File(MetainfoFs *ldb, const QByteArray &data);
        File(MetainfoFs *ldb, const qint64 position);

        void addAttributes(QList<NameInfo> name);
        void addAttributes(QVariantMap name);
        void addTags(QList<QByteArray> name);
        void addTags(QVariantList name);
        using RawStream::getPointer;

        template <typename T>
        bool changeAttribute(const QByteArray &name, const T &value);

    private:
        MetainfoFs *db;
    };
}

#endif // KTOOLS_KFF_FILE_H
