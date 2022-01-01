#include "interface.h"
#include "../KTools/Kff/metainfofs.h"
#include "../KTools/Kff/image.h"

ImageStorage::Interface::Interface(KTools::Kff::MetainfoFs *ldb)
{
    db = ldb;
}

void ImageStorage::Interface::addPhoto(const QByteArray path, const QVariantMap attributes, const QVariantList tags, const QByteArray convertTo)
{
    KTools::Kff::Pointer pointer = db->addImage(path, convertTo, 100);
    KTools::Kff::Image img(db, pointer.getPosition());
    img.addAttributes(attributes);
    img.addTags(tags);
}

void ImageStorage::Interface::addTag(const QByteArray name, const QByteArray description)
{
    db->addTagName(name, description);
}

void ImageStorage::Interface::addAttribute(const QByteArray name, const QByteArray description, const KTools::Kff::DataType dataType)
{
    db->addAttrName(name, dataType, description);
}

QVariantList ImageStorage::Interface::getNames()
{
    QVariantList result;
    KTools::Kff::NameInfoList names = db->getNames();
    for (int i = 0; i < names.items.size(); i++)
    {
        QVariantMap item;
        item["addr"] = names.at(i).addr;
        item["affinity"] = static_cast<int>(names.at(i).affinity);
        item["description"] = QString(names.at(i).description);
        item["name"] = QString(names.at(i).name);
        item["type"] = static_cast<int>(names.at(i).type);
        result.append(item);
    }
    return result;
}
