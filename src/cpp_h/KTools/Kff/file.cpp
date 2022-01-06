#include "file.h"
#include "manager.h"
#include "fixedtypes.h"
#include "variabletypes.h"
#include "metainfofs.h"
#include "list.h"
#include <QDateTime>

KTools::Kff::File::File(MetainfoFs *ldb, const QByteArray &data) : RawStream(ldb->manager, false)
{
    db = ldb;
    dataOffset += 27; // 9 * 3 (3 x Pointer)
    seek(0);
    write(data);
    VariableTypes *strs = manager->getStrings();
    manager->file.seek(clusters.first() + (dataOffset - 27)); // Pointer to attributes names
    manager->file.write<QByteArray>(Pointer(ldb->manager, Pointer::PointerType::VariableTypes, strs->writeVariable("")).getAll());
    manager->file.seek(clusters.first() + (dataOffset - 18)); // Pointer to attributes values
    manager->file.write<QByteArray>(Pointer(ldb->manager, Pointer::PointerType::VariableTypes, strs->writeVariable("")).getAll());
    manager->file.seek(clusters.first() + (dataOffset - 9)); // Pointer to tag names
    manager->file.write<QByteArray>(Pointer(ldb->manager, Pointer::PointerType::VariableTypes, strs->writeVariable("")).getAll());

    QByteArray nowTime = KTools::Converter::toByteArray(QDateTime::currentMSecsSinceEpoch());
    QList<NameInfo> info;
    info.append(db->addAttrName("System:Time:Modification", DataType::Int64, "Modification time of file."));
    info.last().data = nowTime;

    info.append(db->addAttrName("System:Time:Access", DataType::Int64, "Acces time of file."));
    info.last().data = nowTime;

    info.append(db->addAttrName("System:Time:Creation", DataType::Int64, "Creation time of file."));
    info.last().data = nowTime;
    addAttributes(info);
}

KTools::Kff::File::File(MetainfoFs *ldb, const qint64 position) : RawStream(ldb->manager, position)
{
    db = ldb;
    dataOffset += 27; // 9 * 3 (3 x Pointer)
    seek(0);
}

void KTools::Kff::File::addAttributes(QList<NameInfo> name)
{
    manager->file.seek(clusters.first() + (dataOffset - 27));
    Pointer pointer(manager, manager->file.read<QByteArray>(9));
    List<qint64> pointersToNames(&pointer);

    manager->file.seek(clusters.first() + (dataOffset - 18));
    pointer.setAll(manager->file.read<QByteArray>(9));
    List<Pointer> pointersToValues(&pointer);

    for (int i = 0; i < pointersToNames.size(); i++)
    {
        NameInfo readedNameinfo = db->getAttrName(pointersToNames[i]);
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
                    case DataType::String : pointersToValues[i].writeData(name[n].data); break;
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
                qint64 valueAddr = manager->getStrings()->writeVariable(name[i].data);
                pointer.setAll(Pointer::PointerType::VariableTypes, valueAddr);
                break;
            }
            case DataType::List : KLOG_ERROR("Currently unsupported"); break;
        }
        manager->file.seek(clusters.first() + (dataOffset - 27)); // Names
        Pointer pairPointer(manager, manager->file.read<QByteArray>(9));
        pointersToNames.~List();
        pointersToNames = List<qint64>(&pairPointer);
        pointersToNames += nameInfo.addr;

        manager->file.seek(clusters.first() + (dataOffset - 18)); // Values
        pairPointer.setAll(manager->file.read<QByteArray>(9));
        pointersToValues.~List();
        pointersToValues = List<Pointer>(&pairPointer);
        pointersToValues += pointer;
    }
}

void KTools::Kff::File::addAttributes(QVariantMap name)
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

void KTools::Kff::File::addTags(QList<QByteArray> name)
{
    manager->file.seek(clusters.first() + (dataOffset - 9));
    Pointer pointer(manager, manager->file.read<QByteArray>(9));
    List<qint64> pointerData(&pointer);
    QList<qint64> newNamesNumbers;
    QList<qint64> oldNamesNumbers;

    for (int i = 0; i < pointerData.size(); i++)
    {
        oldNamesNumbers.append(pointerData[i]);
    }

    for (int i = 0; i < name.size(); i++)
    {
        newNamesNumbers.append(db->getTagName(name[i]).addr);
    }

    newNamesNumbers = QSet<qint64>(newNamesNumbers.begin(), newNamesNumbers.end()).subtract(QSet<qint64>(oldNamesNumbers.begin(), oldNamesNumbers.end())).values();
    oldNamesNumbers.append(newNamesNumbers);

    pointerData = oldNamesNumbers;
}

void KTools::Kff::File::addTags(QVariantList name)
{
    QList<QByteArray> result;
    for (int i = 0; i < name.size(); i++)
    {
        result.append(name.at(i).toByteArray());
    }
    addTags(result);
}

template <typename T>
bool KTools::Kff::File::changeAttribute(const QByteArray &name, const T &value)
{
    NameInfo info = db->getAttrName(name);
    if (info.addr == -1)
    {
        KLOG_ERROR("Attribute with name " + name + " does not exist.");
        return false;
    }

    manager->file.seek(clusters.first() + (dataOffset - 27));
    Pointer pointer(manager, manager->file.read<QByteArray>(9));
    List<qint64> pointersToNames(&pointer);

    manager->file.seek(clusters.first() + (dataOffset - 18));
    pointer.setAll(manager->file.read<QByteArray>(9));
    QList<Pointer> pointersToValues = pointer.getData<QList<Pointer>>();

    for (int i = 0; i < pointersToNames.size(); i++)
    {
        if (pointersToNames[i] == info.addr)
        {
            NameInfo readedNameinfo = db->getAttrName(pointersToNames[i]);
            if (readedNameinfo.type == DataType::List)
                KLOG_ERROR("Currently unsupported");
            else if (readedNameinfo.type == DataType::String)
            {
                String str(&pointersToValues[i]);
                str = value;
            }
            else
                pointersToValues[i].writeData(KTools::Converter::byteArrayToT<T>(value));

            break;
        }
    }
    return true;
}
