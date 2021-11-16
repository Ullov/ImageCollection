#include "fixedtypes.h"

KTools::Kff::FixedTypes::FixedTypes(Manager *man) : RawStream(man, true) {}

template<typename T>
qint64 KTools::Kff::FixedTypes::add(const T data, const Type type)
{
    if (Size::get(type) != sizeof(T))
    {
        KTools::Log::writeError("Size of type value and passed to template type not equal. " + QString::number(Size::get(type)) + " != " + QString::number(sizeof(T)), "KTools::Kff::FixedTypes::add()");
        return -1;
    }

    bool writed = false;
    qint64 lastSeek = 0;
    seek(lastSeek);
    for (int i = 0; pos() < size() ; i++)
    {
        QByteArray tryRead = read(1);
        if (*tryRead.data() != '\0')
        {
            Type readedType = static_cast<Type>(*tryRead.data());
            seek(lastSeek + 1 + Size::get(readedType));
            lastSeek = pos();
        }
        else
        {
            QByteArray content;
            content.append(static_cast<quint8>(type));
            content.append(KTools::Converter::toByteArray(data));
            seek(lastSeek);
            write(content);
            writed = true;
            break;
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
        KTools::Log::writeError("Attempt read wrong data type.", "KTools::Kff::FixedTypes::get()");
        return T();
    }
    seek(position + 1);
    return KTools::Converter::byteArrayToT<T>(read(sizeof(T)));
}

bool KTools::Kff::FixedTypes::remove(const qint64 position)
{
    if (position > size())
    {
        KTools::Log::writeError("Position > size.", "KTools::Kff::FixedTypes::remove()");
        return false;
    }
    seek(position);
    QByteArray tryRead = read(1);
    if (*tryRead.data() == '\0')
    {
        KTools::Log::writeError("Attempt remove already empty value or position is wrong.", "KTools::Kff::FixedTypes::remove()");
        return false;
    }
    qint8 valSize = Size::get(static_cast<Type>(*tryRead.data()));
    QByteArray content;
    content.append((1 + valSize), char(0));
    seek(position);
    write(content);
    if ((size() - position) == content.length())
    {
        resize(size() - content.length());
    }
    return true;
}

template qint64 KTools::Kff::FixedTypes::add<qint8>(const qint8, const Type);
template qint64 KTools::Kff::FixedTypes::add<qint16>(const qint16, const Type);
template qint64 KTools::Kff::FixedTypes::add<qint32>(const qint32, const Type);
template qint64 KTools::Kff::FixedTypes::add<qint64>(const qint64, const Type);
//template qint64 KTools::Kff::FixedTypes::add<double>(const double, const Type);
//template qint64 KTools::Kff::FixedTypes::add<bool>(const bool, const Type);

template qint8 KTools::Kff::FixedTypes::get<qint8>(const qint64);
template qint16 KTools::Kff::FixedTypes::get<qint16>(const qint64);
template qint32 KTools::Kff::FixedTypes::get<qint32>(const qint64);
template qint64 KTools::Kff::FixedTypes::get<qint64>(const qint64);
//template double KTools::Kff::FixedTypes::get<double>(const qint64);
//template bool KTools::Kff::FixedTypes::get<bool>(const qint64);
