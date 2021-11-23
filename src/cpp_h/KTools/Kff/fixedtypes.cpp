#include "fixedtypes.h"

KTools::Kff::FixedTypes::FixedTypes(Manager *man) : RawStream(man, true) {}

KTools::Kff::FixedTypes::FixedTypes(Manager *man, const qint64 position) : RawStream(man, position) {}

template<typename T>
qint64 KTools::Kff::FixedTypes::add(const T data, const Type type)
{
    if (Size::get(type) != sizeof(T))
    {
        KLOG_ERROR("Size of type value and passed to template type not equal. " + QString::number(Size::get(type)) + " != " + QString::number(sizeof(T)));
        return -1;
    }

    bool writed = false;
    qint64 lastSeek = 0;
    seek(lastSeek);
    for (int i = 0; pos() < size() ; i++)
    {
        QByteArray voidNumb = QByteArray() + (char)Size::getVoidTypeNumb(type);
        QByteArray tryRead = read(1);
        if (*tryRead.data() == '\0' || tryRead == voidNumb)
        {
            QByteArray content;
            content.append(static_cast<quint8>(type));
            content.append(KTools::Converter::toByteArray(data));
            seek(lastSeek);
            write(content);
            writed = true;
            break;
        }
        else
        {
            Type readedType = static_cast<Type>(*tryRead.data());
            seek(lastSeek + 1 + Size::get(readedType));
            lastSeek = pos();
        }
    }
    if (!writed)
    {
        QByteArray content;
        content.append(static_cast<quint8>(type));
        content.append(KTools::Converter::toByteArray(data));
        write(content);
    }
    return lastSeek;
}

template<typename T>
T KTools::Kff::FixedTypes::get(const qint64 position)
{
    seek(position);
    if (Size::get(static_cast<Type>(*read(1).data())) != sizeof(T))
    {
        KLOG_ERROR("Attempt read wrong data type.");
        return T();
    }
    seek(position + 1);
    return KTools::Converter::byteArrayToT<T>(read(sizeof(T)));
}

bool KTools::Kff::FixedTypes::remove(const qint64 position)
{
    if (position > size())
    {
        KLOG_ERROR("Position > size.");
        return false;
    }
    seek(position);
    QByteArray tryRead = read(1);
    if (*tryRead.data() == '\0')
    {
        KLOG_ERROR("Attempt remove already empty value or position is wrong.");
        return false;
    }
    qint8 valSize = Size::get(static_cast<Type>(*tryRead.data()));
    QByteArray content;
    content.append(Size::getVoidTypeNumb(static_cast<Type>(*tryRead.data())));
    content.append(valSize, char(0));
    seek(position);
    write(content);
    if ((size() - position) == content.length())
    {
        resize(size() - content.length());
    }
    return true;
}

template <typename T>
bool KTools::Kff::FixedTypes::change(const T data, const qint64 position)
{
    if (position > size())
    {
        KLOG_ERROR("Position > size.");
        return false;
    }
    seek(position);
    qint8 valSize = Size::get(static_cast<Type>(*read(1).data()));
    if (valSize != sizeof(T))
    {
        KLOG_ERROR("Size of type value and passed to template type not equal. " + QString::number(valSize) + " != " + QString::number(sizeof(T)));
        return false;
    }
    seek(position + 1);
    write(KTools::Converter::toByteArray(data));
    return true;
}

template qint64 KTools::Kff::FixedTypes::add<qint8>(const qint8, const Type);
template qint64 KTools::Kff::FixedTypes::add<qint16>(const qint16, const Type);
template qint64 KTools::Kff::FixedTypes::add<qint32>(const qint32, const Type);
template qint64 KTools::Kff::FixedTypes::add<qint64>(const qint64, const Type);
template qint64 KTools::Kff::FixedTypes::add<double>(const double, const Type);
template qint64 KTools::Kff::FixedTypes::add<bool>(const bool, const Type);

template qint8 KTools::Kff::FixedTypes::get<qint8>(const qint64);
template qint16 KTools::Kff::FixedTypes::get<qint16>(const qint64);
template qint32 KTools::Kff::FixedTypes::get<qint32>(const qint64);
template qint64 KTools::Kff::FixedTypes::get<qint64>(const qint64);
template double KTools::Kff::FixedTypes::get<double>(const qint64);
template bool KTools::Kff::FixedTypes::get<bool>(const qint64);

template bool KTools::Kff::FixedTypes::change<qint8>(const qint8, const qint64);
template bool KTools::Kff::FixedTypes::change<qint16>(const qint16, const qint64);
template bool KTools::Kff::FixedTypes::change<qint32>(const qint32, const qint64);
template bool KTools::Kff::FixedTypes::change<qint64>(const qint64, const qint64);
template bool KTools::Kff::FixedTypes::change<double>(const double, const qint64);
template bool KTools::Kff::FixedTypes::change<bool>(const bool, const qint64);
