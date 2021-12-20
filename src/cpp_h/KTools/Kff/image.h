#ifndef KTOOLS_KFF_IMAGE_H
#define KTOOLS_KFF_IMAGE_H

#include <QVariantMap>
#include "rawstream.h"
#include "../../ImageStorage/nameinfo.h"

namespace ImageStorage {
    class Db;
}

namespace KTools::Kff {
    class Image : private RawStream
    {
    public:
        Image(ImageStorage::Db *ldb, const QByteArray &data);
        Image(ImageStorage::Db *ldb, const qint64 position);

        void addAttributes(QList<ImageStorage::NameInfo> name);
        void addAttributes(QVariantMap name);
        void addTags(QList<QByteArray> name);
        void addTags(QVariantList name);
        using KTools::Kff::RawStream::getPointer;

        template <typename T>
        bool changeAttribute(const QByteArray &name, const T &value);

    private:
        ImageStorage::Db *db;
    };
}

#endif // KTOOLS_KFF_IMAGE_H
