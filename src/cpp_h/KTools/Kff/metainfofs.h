#ifndef KTOOLS_KFF_METAINFOFS_H
#define KTOOLS_KFF_METAINFOFS_H

#include "../log.h"
#include "manager.h"
#include "pointer.h"
#include "nameinfo.h"

namespace KTools::Kff {
    class MetainfoFs : private Manager
    {
    public:
        MetainfoFs(KTools::Options *opts);

        NameInfo getAttrName(const QByteArray &name);
        NameInfo getAttrName(const qint64 &pos);
        NameInfo addAttrName(const QByteArray &name, const DataType dataType, const QByteArray &description);
        NameInfo addTagName(const QByteArray &name, const QByteArray &description);
        NameInfo getTagName(const QByteArray &name);
        NameInfo getTagName(const qint64 &numb);
        NameInfoList getNames();
        KTools::Kff::Pointer addImage(const QByteArray &path, const QByteArray &convertTo, const qint8 quality);

        Manager *manager = this->manager;

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

#endif // KTOOLS_KFF_METAINFOFS_H
