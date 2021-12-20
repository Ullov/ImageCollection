#include "interface.h"
#include "db.h"
#include "../KTools/Kff/image.h"

ImageStorage::Interface::Interface(Db *ldb)
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

void ImageStorage::Interface::addAttribute(const QByteArray name, const QByteArray description, const ImageStorage::DataType dataType)
{
    //ImageStorage::DataType type;

    /*if (dataType == "Int8") type = ImageStorage::DataType::Int8;
    else if (dataType == "Int16") type = ImageStorage::DataType::Int16;
    else if (dataType == "Int32") type = ImageStorage::DataType::Int32;
    else if (dataType == "Int64") type = ImageStorage::DataType::Int64;
    else if (dataType == "UInt8") type = ImageStorage::DataType::UInt8;
    else if (dataType == "UInt16") type = ImageStorage::DataType::UInt16;
    else if (dataType == "UInt32") type = ImageStorage::DataType::UInt32;
    else if (dataType == "UInt64") type = ImageStorage::DataType::UInt64;
    else if (dataType == "Float") type = ImageStorage::DataType::Float;
    else if (dataType == "Bool") type = ImageStorage::DataType::Bool;
    else if (dataType == "String") type = ImageStorage::DataType::String;
    else {KLOG_ERROR("Undefined data type. dataType: " + dataType); return;}*/

    db->addAttrName(name, dataType, description);
}

QVariantList ImageStorage::Interface::getNames()
{
    QVariantList result;
    NameInfoList names = db->getNames();
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
