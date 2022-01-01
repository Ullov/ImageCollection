#include "image.h"
#include "manager.h"
#include "fixedtypes.h"
#include "variabletypes.h"
#include "metainfofs.h"
#include <QDateTime>

KTools::Kff::Image::Image(MetainfoFs *ldb, const QByteArray &data) : RawStream(ldb->manager, false)
{
    db = ldb;
    dataOffset += 27; // 9 * 3 (3 x Pointer)
    seek(0);
    write(data);
    VariableTypes *strs = manager->getStrings();
    manager->file.seek(clusters.first() + (dataOffset - 27)); // Pointer to attributes names
    manager->file.write<QByteArray>(Pointer(ldb->manager, Pointer::PointerType::VariableTypes, strs->add("", VariableTypes::Type::String)).getAll());
    manager->file.seek(clusters.first() + (dataOffset - 18)); // Pointer to attributes values
    manager->file.write<QByteArray>(Pointer(ldb->manager, Pointer::PointerType::VariableTypes, strs->add("", VariableTypes::Type::ListOfPointers)).getAll());
    manager->file.seek(clusters.first() + (dataOffset - 9)); // Pointer to tag names
    manager->file.write<QByteArray>(Pointer(ldb->manager, Pointer::PointerType::VariableTypes, strs->add("", VariableTypes::Type::String)).getAll());

    QList<NameInfo> info;
    info.append(db->addAttrName("System:Time:Modification", DataType::Int64, "Modification time of file."));
    info.last().data = KTools::Converter::toByteArray(QDateTime::currentMSecsSinceEpoch());

    info.append(db->addAttrName("System:Time:Access", DataType::Int64, "Acces time of file."));
    info.last().data = KTools::Converter::toByteArray(QDateTime::currentMSecsSinceEpoch());

    info.append(db->addAttrName("System:Time:Creation", DataType::Int64, "Creation time of file."));
    info.last().data = KTools::Converter::toByteArray(QDateTime::currentMSecsSinceEpoch());
    addAttributes(info);
}

KTools::Kff::Image::Image(MetainfoFs *ldb, const qint64 position) : RawStream(ldb->manager, position)
{
    db = ldb;
    dataOffset += 27; // 9 * 3 (3 x Pointer)
    seek(0);
}

void KTools::Kff::Image::addAttributes(QList<NameInfo> name)
{
    manager->file.seek(clusters.first() + (dataOffset - 27));
    Pointer pointer(manager, manager->file.read<QByteArray>(9));
    QByteArray pointersToNames = pointer.getData<QByteArray>(); // CAUTION!!! This is custom pointers.

    manager->file.seek(clusters.first() + (dataOffset - 18));
    pointer.setAll(manager->file.read<QByteArray>(9));
    QList<Pointer> pointersToValues = pointer.getData<QList<Pointer>>();

    for (int i = 0; i < pointersToNames.size(); i += 8)
    {
        NameInfo readedNameinfo = db->getAttrName(KTools::Converter::byteArrayToT<qint64>(pointersToNames.mid(i, 8)));
        for (int n = 0; n < name.size(); n++)
        {
            if (readedNameinfo.name == name[n].name)
            {
                switch (readedNameinfo.type)
                {
                    case DataType::Int8 : pointersToValues[i].writeData(KTools::Converter::byteArrayToT<qint8>(name[n].data)); break;
                    case DataType::Int16 : pointersToValues[i].writeData(KTools::Converter::byteArrayToT<qint16>(name[n].data)); break;
                    case DataType::Int32 : pointersToValues[i].writeData(KTools::Converter::byteArrayToT<qint32>(name[n].data)); break;
                    case DataType::Int64 : pointersToValues[i].writeData(KTools::Converter::byteArrayToT<qint64>(name[n].data)); break;
                    case DataType::UInt8 : pointersToValues[i].writeData(KTools::Converter::byteArrayToT<quint8>(name[n].data)); break;
                    case DataType::UInt16 : pointersToValues[i].writeData(KTools::Converter::byteArrayToT<quint16>(name[n].data)); break;
                    case DataType::UInt32 : pointersToValues[i].writeData(KTools::Converter::byteArrayToT<quint32>(name[n].data)); break;
                    case DataType::UInt64 : pointersToValues[i].writeData(KTools::Converter::byteArrayToT<quint64>(name[n].data)); break;
                    case DataType::Float : pointersToValues[i].writeData(KTools::Converter::byteArrayToT<double>(name[n].data)); break;
                    case DataType::Bool : pointersToValues[i].writeData(KTools::Converter::byteArrayToT<bool>(name[n].data)); break;
                    case DataType::String :
                    {
                        pointersToValues[i].writeData(name[n].data);
                        pointer.writeData(pointersToValues);
                        break;
                    }
                    case DataType::List : KLOG_ERROR("Currently unsupported"); break;
                }
                name.removeAt(n);
                break;
            }
        }
    }

    for (int i = 0; i < name.size(); i++)
    {
        NameInfo nameInfo = db->addAttrName(name[i].name, name[i].type, name[i].description);

        switch (name[i].type)
        {
            case DataType::Int8 :
            {
                qint8 convertedValue = KTools::Converter::byteArrayToT<qint8>(name[i].data);
                qint64 valueAddr = manager->getNumbers()->add<qint8>(convertedValue);
                pointer.setAll(Pointer::PointerType::FixedTypes, valueAddr);
                break;
            }
            case DataType::Int16 :
            {
                qint16 convertedValue = KTools::Converter::byteArrayToT<qint16>(name[i].data);
                qint64 valueAddr = manager->getNumbers()->add<qint16>(convertedValue);
                pointer.setAll(Pointer::PointerType::FixedTypes, valueAddr);
                break;
            }
            case DataType::Int32 :
            {
                qint32 convertedValue = KTools::Converter::byteArrayToT<qint32>(name[i].data);
                qint64 valueAddr = manager->getNumbers()->add<qint32>(convertedValue);
                pointer.setAll(Pointer::PointerType::FixedTypes, valueAddr);
                break;
            }
            case DataType::Int64 :
            {
                qint64 convertedValue = KTools::Converter::byteArrayToT<qint64>(name[i].data);
                qint64 valueAddr = manager->getNumbers()->add<qint64>(convertedValue);
                pointer.setAll(Pointer::PointerType::FixedTypes, valueAddr);
                break;
            }
            case DataType::UInt8 :
            {
                quint8 convertedValue = KTools::Converter::byteArrayToT<quint8>(name[i].data);
                qint64 valueAddr = manager->getNumbers()->add<quint8>(convertedValue);
                pointer.setAll(Pointer::PointerType::FixedTypes, valueAddr);
                break;
            }
            case DataType::UInt16 :
            {
                quint16 convertedValue = KTools::Converter::byteArrayToT<quint16>(name[i].data);
                qint64 valueAddr = manager->getNumbers()->add<quint16>(convertedValue);
                pointer.setAll(Pointer::PointerType::FixedTypes, valueAddr);
                break;
            }
            case DataType::UInt32 :
            {
                quint32 convertedValue = KTools::Converter::byteArrayToT<quint32>(name[i].data);
                qint64 valueAddr = manager->getNumbers()->add<quint32>(convertedValue);
                pointer.setAll(Pointer::PointerType::FixedTypes, valueAddr);
                break;
            }
            case DataType::UInt64 :
            {
                quint64 convertedValue = KTools::Converter::byteArrayToT<quint64>(name[i].data);
                qint64 valueAddr = manager->getNumbers()->add<quint64>(convertedValue);
                pointer.setAll(Pointer::PointerType::FixedTypes, valueAddr);
                break;
            }
            case DataType::Float :
            {
                double convertedValue = KTools::Converter::byteArrayToT<double>(name[i].data);
                qint64 valueAddr = manager->getNumbers()->add<double>(convertedValue);
                pointer.setAll(Pointer::PointerType::FixedTypes, valueAddr);
                break;
            }
            case DataType::Bool :
            {
                bool convertedValue = KTools::Converter::byteArrayToT<bool>(name[i].data);
                qint64 valueAddr = manager->getNumbers()->add<bool>(convertedValue);
                pointer.setAll(Pointer::PointerType::FixedTypes, valueAddr);
                break;
            }
            case DataType::String :
            {
                qint64 valueAddr = manager->getStrings()->add(name[i].data, VariableTypes::Type::String);
                pointer.setAll(Pointer::PointerType::VariableTypes, valueAddr);
                break;
            }
            case DataType::List : KLOG_ERROR("Currently unsupported"); break;
        }
        manager->file.seek(clusters.first() + (dataOffset - 27)); // Names
        Pointer pairPointer(manager, manager->file.read<QByteArray>(9));
        pointersToNames = pairPointer.getData<QByteArray>();
        pointersToNames.append(KTools::Converter::toByteArray(nameInfo.addr));
        pairPointer.writeData(pointersToNames);

        manager->file.seek(clusters.first() + (dataOffset - 18)); // Values
        pairPointer.setAll(manager->file.read<QByteArray>(9));
        pointersToValues = pairPointer.getData<QList<Pointer>>();
        pointersToValues.append(pointer);
        pairPointer.writeData(pointersToValues);
    }
}

void KTools::Kff::Image::addAttributes(QVariantMap name)
{
    QList<NameInfo> result;
    QList<QString> keys = name.keys();
    for (int i = 0; i < keys.size(); i++)
    {
        NameInfo item = db->getAttrName(KTools::Converter::convert<QString, QByteArray>(keys[i]));
        item.data = name[keys[i]].toByteArray();
        result.append(item);
    }
    addAttributes(result);
}

void KTools::Kff::Image::addTags(QList<QByteArray> name)
{
    manager->file.seek(clusters.first() + (dataOffset - 9));
    Pointer pointer(manager, manager->file.read<QByteArray>(9));
    QByteArray pointerData = pointer.getData<QByteArray>();
    QList<qint64> newNamesNumbers;
    QList<qint64> oldNamesNumbers;

    for (int i = 0; i < pointerData.size(); i += 8)
    {
        oldNamesNumbers.append(KTools::Converter::byteArrayToT<qint64>(pointerData.mid(i, 8)));
    }

    for (int i = 0; i < name.size(); i++)
    {
        newNamesNumbers.append(db->getTagName(name[i]).addr);
    }

    newNamesNumbers = QSet<qint64>(newNamesNumbers.begin(), newNamesNumbers.end()).subtract(QSet<qint64>(oldNamesNumbers.begin(), oldNamesNumbers.end())).values();
    oldNamesNumbers.append(newNamesNumbers);

    pointerData = "";
    for (int i = 0; i < oldNamesNumbers.size(); i++)
    {
        pointerData.append(KTools::Converter::toByteArray<qint64>(oldNamesNumbers[i]));
    }
    pointer.writeData(pointerData);

    manager->file.seek(clusters.first() + (dataOffset - 9));
    manager->file.write<QByteArray>(pointer.getAll());
}

void KTools::Kff::Image::addTags(QVariantList name)
{
    QList<QByteArray> result;
    for (int i = 0; i < name.size(); i++)
    {
        result.append(name.at(i).toByteArray());
    }
    addTags(result);
}

template <typename T>
bool KTools::Kff::Image::changeAttribute(const QByteArray &name, const T &value)
{
    NameInfo info = db->getAttrName(name);
    if (info.addr == -1)
    {
        KLOG_ERROR("Attribute with name " + name + " does not exist.");
        return false;
    }

    manager->file.seek(clusters.first() + (dataOffset - 27));
    Pointer pointer(manager, manager->file.read<QByteArray>(9));
    QByteArray pointersToNames = pointer.getData<QByteArray>(); // CAUTION!!! This is custom pointers.

    manager->file.seek(clusters.first() + (dataOffset - 18));
    pointer.setAll(manager->file.read<QByteArray>(9));
    QList<Pointer> pointersToValues = pointer.getData<QList<Pointer>>();

    for (int i = 0; i < pointersToNames.size(); i += 8)
    {
        if (KTools::Converter::byteArrayToT<qint64>(pointersToNames.mid(i, 8)) == info.addr)
        {
            NameInfo readedNameinfo = db->getAttrName(KTools::Converter::byteArrayToT<qint64>(pointersToNames.mid(i, 8)));
            if (readedNameinfo.type == DataType::List)
                KLOG_ERROR("Currently unsupported");
            else if (readedNameinfo.type == DataType::String)
            {
                pointersToValues[i].writeData(value);
                pointer.writeData(pointersToValues);
                break;
            }
            else
                pointersToValues[i].writeData(KTools::Converter::byteArrayToT<T>(value));

            break;
        }
    }
    return true;
}
