#ifndef KTOOLS_KFF_IMAGE_H
#define KTOOLS_KFF_IMAGE_H

#include <QVariantMap>
#include "rawstream.h"
#include "nameinfo.h"

namespace KTools::Kff {
    class MetainfoFs;
    class Image : private RawStream
    {
    public:
        Image(MetainfoFs *ldb, const QByteArray &data);
        Image(MetainfoFs *ldb, const qint64 position);

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

#endif // KTOOLS_KFF_IMAGE_H
