#ifndef IMAGESTORAGE_DB_H
#define IMAGESTORAGE_DB_H

#include "../KTools/log.h"
#include "../KTools/Kff/manager.h"
#include "../KTools/Kff/pointer.h"
#include "nameinfo.h"

namespace ImageStorage {
    class Db : private KTools::Kff::Manager
    {
    public:
        Db(KTools::Options *opts);

        NameInfo getAttrName(const QByteArray &name);
        NameInfo getAttrName(const qint64 &pos);
        NameInfo addAttrName(const QByteArray &name, const DataType dataType, const QByteArray &description);
        NameInfo addTagName(const QByteArray &name, const QByteArray &description);
        NameInfo getTagName(const QByteArray &name);
        NameInfo getTagName(const qint64 &numb);
        NameInfoList getNames();
        KTools::Kff::Pointer addImage(const QByteArray &path, const QByteArray &convertTo, const qint8 quality);

        KTools::Kff::Manager *manager = this->manager;

    private:
        enum class EntityAssociation : qint8 {
            Image = 1,
            Gallery = 2
        };
        enum class ConvertTo {
            KeepOriginal,
            Jpg,
            Png,
            Gif,
            Webp,
            Tiff
        };

        KTools::Options *options;
    };
}

#endif // IMAGESTORAGE_DB_H
