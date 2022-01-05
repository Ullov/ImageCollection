#include "metainfofs.h"
#include "../options.h"
#include "image.h"
#include "list.h"
#include <QImage>

KTools::Kff::MetainfoFs::MetainfoFs(KTools::Options *opts) : KTools::Kff::Manager(opts->getParam<QByteArray>("Path:Data") + "/iamgeStorage.kff", KTools::Kff::Manager::OpenMode::Keep)
{
    options = opts;
    if (mode == KTools::Kff::Manager::OpenMode::Clear)
    {
        KTools::Kff::Pointer pointer(this, Pointer::PointerType::VariableTypes, strs->writeVariable("")); // Attribute names
        defaultStream->seek(0);
        defaultStream->write(pointer.getAll());

        pointer.setPos(strs->writeVariable("")); // Tag names
        defaultStream->seek(9);
        defaultStream->write(pointer.getAll());

        pointer.setPos(strs->writeVariable("")); // Pointers to images
        defaultStream->seek(18);
        defaultStream->write(pointer.getAll());
    }
}

KTools::Kff::NameInfo KTools::Kff::MetainfoFs::getAttrName(const QByteArray &name)
{
    NameInfo result;
    result.addr = -1;
    defaultStream->seek(0);
    Pointer pointer(this, defaultStream->read(9));
    QByteArray names = pointer.getData<QByteArray>();
    for (qint64 i = 0; i < names.size(); i += 10)
    {
        QByteArray entity(names.mid(i, 10));
        QByteArray nameAndDescr = Pointer(this, entity.mid(0, 9)).getData<QByteArray>();
        qint64 nameLen = KTools::Converter::byteArrayToT<qint64>(nameAndDescr.mid(0, 8));
        QByteArray readedName = nameAndDescr.mid(8, nameLen);
        if (readedName == name)
        {
            result.addr = i;
            result.type = static_cast<DataType>(entity.at(entity.size() - 1));
            result.name = readedName;
            result.affinity = NameAffinity::Attribute;
            if (nameLen != nameAndDescr.size() - 8)
                result.description = nameAndDescr.mid(nameLen + 8);

            break;
        }
    }
    return result;
}

KTools::Kff::NameInfo KTools::Kff::MetainfoFs::getAttrName(const qint64 &pos)
{
    NameInfo result;
    defaultStream->seek(0);
    Pointer pointer(this, defaultStream->read(9));
    QByteArray names = pointer.getData<QByteArray>();

    QByteArray entity(names.mid(pos, 10));
    QByteArray nameAndDescr = Pointer(this, entity.mid(0, 9)).getData<QByteArray>();
    qint64 nameLen = KTools::Converter::byteArrayToT<qint64>(nameAndDescr.mid(0, 8));
    QByteArray readedName = nameAndDescr.mid(8, nameLen);

    result.addr = pos;
    result.type = static_cast<DataType>(entity.at(entity.size() - 1));
    result.name = readedName;
    result.affinity = NameAffinity::Attribute;
    if (nameLen != nameAndDescr.size() - 8)
        result.description = nameAndDescr.mid(nameLen + 8);
    return result;
}

KTools::Kff::NameInfo KTools::Kff::MetainfoFs::addAttrName(const QByteArray &name, const DataType dataType, const QByteArray &description)
{
    NameInfo result = getAttrName(name);
    if (result.addr != -1)
    { // Name already exist. Returned address of name.
        return result;
    }
    QByteArray content;
    content.append(KTools::Converter::toByteArray<qint64>(name.length()));
    content.append(name);
    content.append(description);
    Pointer pointer(this, Pointer::PointerType::VariableTypes, strs->writeVariable(content));

    content = "";
    content.append(pointer.getAll());
    content.append(static_cast<qint8>(dataType));

    defaultStream->seek(0);
    pointer.setAll(defaultStream->read(9));
    QByteArray names = pointer.getData<QByteArray>();
    result.addr = names.size();
    names.append(content);
    pointer.writeData(names);

    result.affinity = NameAffinity::Attribute;
    result.name = name;
    result.description = description;
    result.type = dataType;
    return result;
}

KTools::Kff::NameInfo KTools::Kff::MetainfoFs::addTagName(const QByteArray &name, const QByteArray &description)
{
    NameInfo result = getTagName(name);
    if (result.addr != -1)
    {// Name already exist. Returning name number
        return result;
    }

    defaultStream->seek(9);
    Pointer basePointer(this, defaultStream->read(9));
    List<Pointer> list(&basePointer);

    QByteArray content;
    content.append(KTools::Converter::toByteArray<qint64>(name.length()));
    content.append(name);
    content.append(description);

    result.addr = list.size();
    Pointer pointer(this, Pointer::PointerType::VariableTypes, strs->writeVariable(content));
    list += pointer;

    result.affinity = NameAffinity::Tag;
    result.name = name;
    result.description = description;
    return result;
}

KTools::Kff::NameInfo KTools::Kff::MetainfoFs::getTagName(const QByteArray &name)
{
    NameInfo result;
    result.addr = -1;
    result.name = name;
    result.affinity = NameAffinity::Tag;
    defaultStream->seek(9);
    Pointer pointer = Pointer(this, defaultStream->read(9));
    List<Pointer> list(&pointer);
    for (int i = 0; i < list.size(); i++)
    {
        QByteArray item = list[i].getData<QByteArray>();
        qint64 namelen = KTools::Converter::byteArrayToT<qint64>(item.mid(0, 8));
        QByteArray readedName = item.mid(8, namelen);
        if (readedName == name)
        {
            result.addr = i;
            if (namelen != item.size() - 8)
                result.description = item.mid(8 + namelen);

            break;
        }
    }
    return result;
}

KTools::Kff::NameInfo KTools::Kff::MetainfoFs::getTagName(const qint64 &numb)
{
    NameInfo result;
    result.affinity = NameAffinity::Tag;
    defaultStream->seek(9);
    Pointer pointer = KTools::Kff::Pointer(this, defaultStream->read(9));
    List<Pointer> list(&pointer);

    QByteArray item = list[numb].getData<QByteArray>();
    qint64 namelen = KTools::Converter::byteArrayToT<qint64>(item.mid(0, 8));
    QByteArray readedName = item.mid(8, namelen);


    result.addr = numb;
    result.name = readedName;
    if (namelen != item.size() - 8)
        result.description = item.mid(8 + namelen);
    return result;
}

KTools::Kff::Pointer KTools::Kff::MetainfoFs::addImage(const QByteArray &path, const QByteArray &convertTo, const qint8 quality)
{
    QByteArray resImg;
    if (convertTo != "Keep original")
    {
        QImage img;
        img.load(path);
        img.save(resImg, convertTo, quality);
    }
    else
    {
        resImg = KTools::File::readFile<QByteArray>(path);
    }
    Image file(this, resImg);
    defaultStream->seek(18);
    Pointer pointer(this, defaultStream->read(9));
    List<Pointer> list(&pointer);
    list += file.getPointer();
    return file.getPointer();
}

KTools::Kff::NameInfoList KTools::Kff::MetainfoFs::getNames()
{
    NameInfoList result;
    // Getting tags
    defaultStream->seek(9);
    Pointer pointer = KTools::Kff::Pointer(this, defaultStream->read(9));
    List<Pointer> list(&pointer);
    for (int i = 0; i < list.size(); i++)
    {
        NameInfo nameItem;
        QByteArray item = list[i].getData<QByteArray>();
        qint64 namelen = KTools::Converter::byteArrayToT<qint64>(item.mid(0, 8));
        nameItem.name = item.mid(8, namelen);
        nameItem.addr = i;
        nameItem.affinity = NameAffinity::Tag;
        nameItem.description = item.mid(8 + namelen);
        result.add(nameItem);
    }

    // Getting attributes
    defaultStream->seek(0);
    pointer.setAll(defaultStream->read(9));
    QByteArray names = pointer.getData<QByteArray>();
    for (qint64 i = 0; i < names.size(); i += 10)
    {
        NameInfo nameItem;
        QByteArray entity(names.mid(i, 10));
        QByteArray nameAndDescr = Pointer(this, entity.mid(0, 9)).getData<QByteArray>();
        qint64 nameLen = KTools::Converter::byteArrayToT<qint64>(nameAndDescr.mid(0, 8));
        QByteArray readedName = nameAndDescr.mid(8, nameLen);
        nameItem.addr = i;
        nameItem.type = static_cast<DataType>(entity.at(entity.size() - 1));
        nameItem.name = readedName;
        nameItem.affinity = NameAffinity::Attribute;
        nameItem.description = nameAndDescr.mid(nameLen + 8);
        result.add(nameItem);
    }

    return result;
}
