#include "pointer.h"

#include "../log.h"
#include "../converter.h"
#include "manager.h"
#include "fixedtypes.h"
#include "variabletypes.h"
#include "list.h"

KTools::Kff::Pointer::Pointer()
{
    isNull = true;
    data.fill('\0', 9);
}

KTools::Kff::Pointer::Pointer(Manager *man, const QByteArray &content)
{
    if (content.length() != 9)
        KLOG_ERROR("Wrong content lenght. content.lenght(): " + QString::number(content.length()));
    else
    {
        data = content;
        manager = man;
    }
}

KTools::Kff::Pointer::Pointer(Manager *man, const PointerType type, const qint64 position)
{
    if (position < 0)
        KLOG_ERROR("Position less than 0. position: " + QString::number(position));
    else
    {
        data.append(KTools::Converter::toByteArray(static_cast<qint8>(type)));
        data.append(KTools::Converter::toByteArray(position));
        manager = man;
    }
}

QByteArray KTools::Kff::Pointer::makePointer(const PointerType type, const qint64 position)
{
    QByteArray pointer;
    pointer.append(KTools::Converter::toByteArray(static_cast<qint8>(type)));
    pointer.append(KTools::Converter::toByteArray(position));
    return pointer;
}

qint64 KTools::Kff::Pointer::getPosition()
{
    return KTools::Converter::byteArrayToT<qint64>(data.mid(1));
}

KTools::Kff::Pointer::PointerType KTools::Kff::Pointer::getType()
{
    return static_cast<PointerType>(data.at(0));
}

QByteArray KTools::Kff::Pointer::getAll() const
{
    return data;
}

template <>
qint8 KTools::Kff::Pointer::getData()
{
    if (comparePointerType(PointerType::FixedTypes, data.at(0)))
    {
        FixedTypes *numbs = manager->getNumbers();
        return numbs->get<qint8>(KTools::Converter::byteArrayToT<qint64>(data.mid(1)));
    }
    else
    {
        KLOG_ERROR("Wrong pointer type. type:" + QString(data.at(0)));
        return 0;
    }
}

template <>
qint16 KTools::Kff::Pointer::getData()
{
    if (comparePointerType(PointerType::FixedTypes, data.at(0)))
    {
        FixedTypes *numbs = manager->getNumbers();
        return numbs->get<qint16>(KTools::Converter::byteArrayToT<qint64>(data.mid(1)));
    }
    else
    {
        KLOG_ERROR("Wrong pointer type. type:" + QString(data.at(0)));
        return 0;
    }
}

template <>
qint32 KTools::Kff::Pointer::getData()
{
    if (comparePointerType(PointerType::FixedTypes, data.at(0)))
    {
        FixedTypes *numbs = manager->getNumbers();
        return numbs->get<qint32>(KTools::Converter::byteArrayToT<qint64>(data.mid(1)));
    }
    else
    {
        KLOG_ERROR("Wrong pointer type. type:" + QString(data.at(0)));
        return 0;
    }
}

template <>
qint64 KTools::Kff::Pointer::getData()
{
    if (comparePointerType(PointerType::FixedTypes, data.at(0)))
    {
        FixedTypes *numbs = manager->getNumbers();
        return numbs->get<qint64>(KTools::Converter::byteArrayToT<qint64>(data.mid(1)));
    }
    else
    {
        KLOG_ERROR("Wrong pointer type. type:" + QString(data.at(0)));
        return 0;
    }
}

template <>
QByteArray KTools::Kff::Pointer::getData()
{
    if (comparePointerType(PointerType::VariableTypes, data.at(0)))
    {
        VariableTypes *strs = manager->getStrings();
        return strs->readString(KTools::Converter::byteArrayToT<qint64>(data.mid(1)));
    }
    else
    {
        KLOG_ERROR("Wrong pointer type. type:" + QString(data.at(0)));
        return QByteArray();
    }
}

template <>
QString KTools::Kff::Pointer::getData()
{
    if (comparePointerType(PointerType::VariableTypes, data.at(0)))
    {
        VariableTypes *strs = manager->getStrings();
        return strs->readString(KTools::Converter::byteArrayToT<qint64>(data.mid(1)));
    }
    else
    {
        KLOG_ERROR("Wrong pointer type. type:" + QString(data.at(0)));
        return QString();
    }
}

template <>
KTools::Kff::RawStream KTools::Kff::Pointer::getData()
{
    if (comparePointerType(PointerType::File, data.at(0)))
        return RawStream(manager, KTools::Converter::byteArrayToT<qint64>(data.mid(1)));
    else
    {
        KLOG_ERROR("Wrong pointer type. type:" + QString(data.at(0)));
        return static_cast<RawStream>(NULL);
    }
}

bool KTools::Kff::Pointer::comparePointerType(const PointerType first, const quint8 second)
{
    if (isNull)
    {
        KLOG_ERROR("Attempt get null pointer.");
        return false;
    }
    else
    {
        if (first == static_cast<PointerType>(second))
            return true;
        else
        {
            KLOG_ERROR("Attempt get pointer with wrong type. first: " + QByteArray::number(static_cast<quint8>(first)) + " second: " + QByteArray::number(second));
            return false;
        }
    }
}

void KTools::Kff::Pointer::setType(const PointerType type)
{
    data[0] = *KTools::Converter::toByteArray<qint8>(static_cast<qint8>(type));
}

void KTools::Kff::Pointer::setPos(const qint64 position)
{
    data.insert(1, KTools::Converter::toByteArray<qint64>(position));
}

void KTools::Kff::Pointer::setAll(const PointerType type, const qint64 position)
{
    setType(type);
    setPos(position);
}

void KTools::Kff::Pointer::setAll(const QByteArray &content)
{
    if (content.size() == 9)
    {
        setType(static_cast<PointerType>(content[0]));
        setPos(KTools::Converter::byteArrayToT<qint64>(content.mid(1)));
    }
    else
    {
        KLOG_ERROR("Content size != 9.");
        return;
    }
}

template <>
bool KTools::Kff::Pointer::writeData(const QByteArray &content)
{
    if (!comparePointerType(PointerType::VariableTypes, data.at(0)))
    {
        KLOG_ERROR("Attempt write to wrong data type.");
        return false;
    }
    else
    {
        qint64 addr = manager->getStrings()->rewriteVariable(content, getPosition());
        setPos(addr);
        return true;
    }
}

template <>
bool KTools::Kff::Pointer::writeData(const qint8 &content)
{
    if (!comparePointerType(PointerType::FixedTypes, data.at(0)))
    {
        KLOG_ERROR("Attempt write to wrong data type.");
        return false;
    }
    else
    {
        manager->getNumbers()->change(content, getPosition());
        return true;
    }
}

template <>
bool KTools::Kff::Pointer::writeData(const qint16 &content)
{
    if (!comparePointerType(PointerType::FixedTypes, data.at(0)))
    {
        KLOG_ERROR("Attempt write to wrong data type.");
        return false;
    }
    else
    {
        manager->getNumbers()->change(content, getPosition());
        return true;
    }
}

template <>
bool KTools::Kff::Pointer::writeData(const qint32 &content)
{
    if (!comparePointerType(PointerType::FixedTypes, data.at(0)))
    {
        KLOG_ERROR("Attempt write to wrong data type.");
        return false;
    }
    else
    {
        manager->getNumbers()->change(content, getPosition());
        return true;
    }
}

template <>
bool KTools::Kff::Pointer::writeData(const qint64 &content)
{
    if (!comparePointerType(PointerType::FixedTypes, data.at(0)))
    {
        KLOG_ERROR("Attempt write to wrong data type.");
        return false;
    }
    else
    {
        manager->getNumbers()->change(content, getPosition());
        return true;
    }
}

template <>
bool KTools::Kff::Pointer::writeData(const quint8 &content)
{
    if (!comparePointerType(PointerType::FixedTypes, data.at(0)))
    {
        KLOG_ERROR("Attempt write to wrong data type.");
        return false;
    }
    else
    {
        manager->getNumbers()->change(content, getPosition());
        return true;
    }
}

template <>
bool KTools::Kff::Pointer::writeData(const quint16 &content)
{
    if (!comparePointerType(PointerType::FixedTypes, data.at(0)))
    {
        KLOG_ERROR("Attempt write to wrong data type.");
        return false;
    }
    else
    {
        manager->getNumbers()->change(content, getPosition());
        return true;
    }
}

template <>
bool KTools::Kff::Pointer::writeData(const quint32 &content)
{
    if (!comparePointerType(PointerType::FixedTypes, data.at(0)))
    {
        KLOG_ERROR("Attempt write to wrong data type.");
        return false;
    }
    else
    {
        manager->getNumbers()->change(content, getPosition());
        return true;
    }
}

template <>
bool KTools::Kff::Pointer::writeData(const quint64 &content)
{
    if (!comparePointerType(PointerType::FixedTypes, data.at(0)))
    {
        KLOG_ERROR("Attempt write to wrong data type.");
        return false;
    }
    else
    {
        manager->getNumbers()->change(content, getPosition());
        return true;
    }
}

template <>
bool KTools::Kff::Pointer::writeData(const double &content)
{
    if (!comparePointerType(PointerType::FixedTypes, data.at(0)))
    {
        KLOG_ERROR("Attempt write to wrong data type.");
        return false;
    }
    else
    {
        manager->getNumbers()->change(content, getPosition());
        return true;
    }
}

template <>
bool KTools::Kff::Pointer::writeData(const bool &content)
{
    if (!comparePointerType(PointerType::FixedTypes, data.at(0)))
    {
        KLOG_ERROR("Attempt write to wrong data type.");
        return false;
    }
    else
    {
        manager->getNumbers()->change(content, getPosition());
        return true;
    }
}

KTools::Kff::Manager* KTools::Kff::Pointer::getManager()
{
    return manager;
}

void KTools::Kff::Pointer::deleteData()
{
    if (getType() == PointerType::FixedTypes)
    {
        manager->getNumbers()->remove(getPosition());
    }
    else if (getType() == PointerType::VariableTypes)
    {
        manager->getStrings()->deleteVariable(getPosition());
    }
    else if (getType() == PointerType::File)
    {
        manager->deleteStream(getPosition());
    }
    else if (getType() == PointerType::Null)
    {
        KLOG_ERROR("There might be segfault (pointer already null).");
    }
    isNull = true;
    setType(PointerType::Null);
}
