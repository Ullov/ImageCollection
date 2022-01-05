#include "variabletypes.h"

KTools::Kff::VariableTypes::VariableTypes(Manager *man) : RawStream(man, true) {}
KTools::Kff::VariableTypes::VariableTypes(Manager *man, const qint64 position) : RawStream(man, position) {}

qint64 KTools::Kff::VariableTypes::writeVariable(const QByteArray &data, const qint64 position)
{
    bool run = true;
    bool first = true;
    qint64 firstClsAddr = -1;
    qint64 lastSeek = 0;
    qint64 writed = 0;
    qint64 sizeAddress = 0;
    qint64 previousClsAddr = 0;
    if (position != -1)
    {
        seek(position + 17);
        if (data.size() <= Sizes::data)
        {
            write(data);
            run = false;
            writed = data.size();
        }
        else
        {
            write(data.mid(0, Sizes::data));
            writed += Sizes::data;
        }
        if (!run)
        {// Write size because here is no next cluster
            seek(position + 9);
            write(KTools::Converter::toByteArray<qint64>(data.length()));
        }
        else
        {
            sizeAddress = position + 9;
            previousClsAddr = lastSeek;
        }
        firstClsAddr = position;
        first = false;
        lastSeek = Sizes::all + position;
    }
    while (run)
    {
        seek(lastSeek);
        qint8 tryRead = KTools::Converter::byteArrayToT<qint8>(read(1));
        if (tryRead != 0)
        {// Occupied cluster
            lastSeek += Sizes::all;
        }
        else if (!first && tryRead == 0)
        {// Writing second cluster
            seek(previousClsAddr + 1);
            write(KTools::Converter::toByteArray(lastSeek));

            QByteArray content;
            content.append(static_cast<quint8>(Type::OccupiedCls)); // Cluster type
            content.append(KTools::Converter::toByteArray(-1ll)); // Next cluster address
            if ((writed + Sizes::occupiedData) > data.length())
            {
                content.append(data.mid(writed));
                run = false;
                writed += data.length() - writed;
            }
            else
            {
                content.append(data.mid(writed, Sizes::occupiedData));
                writed += Sizes::occupiedData;
            }
            seek(lastSeek);
            write(content);

            if (!run)
            {
                seek(sizeAddress);
                write(KTools::Converter::toByteArray<qint64>(writed));
            }

            previousClsAddr = lastSeek;
            lastSeek += Sizes::all;
        }
        else if (first && tryRead == 0)
        {// Writing first cluster
            QByteArray content;
            content.append(static_cast<quint8>(Type::String)); // Cluster type
            content.append(KTools::Converter::toByteArray(-1ll)); // Next cluster address
            content.append(KTools::Converter::toByteArray(0ll)); // Size of variable
            if (data.size() <= Sizes::data)
            {
                content.append(data);
                run = false;
                writed = data.size();
            }
            else
            {
                content.append(data.mid(0, Sizes::data));
                writed += Sizes::data;
            }
            seek(lastSeek);
            write(content);
            if (!run)
            {// Write size because here is no next cluster
                seek(lastSeek + 9);
                write(KTools::Converter::toByteArray<qint64>(data.length()));
            }
            else
            {
                sizeAddress = lastSeek + 9;
                previousClsAddr = lastSeek;
            }
            firstClsAddr = lastSeek;
            first = false;
            lastSeek += Sizes::all;
        }
    }
    return firstClsAddr;
}

qint64 KTools::Kff::VariableTypes::rewriteVariable(const QByteArray &data, const qint64 position)
{
    seek(position);
    qint8 tryRead = KTools::Converter::byteArrayToT<qint8>(read(1));
    if (tryRead != static_cast<qint8>(Type::String))
    {
        KLOG_ERROR("Wrong type or wrong position. position: " + QString::number(position));
        return -1;
    }
    clearVariable(position);
    writeVariable(data, position);
    return position;
}

QByteArray KTools::Kff::VariableTypes::readString(const qint64 position)
{
    QByteArray result;
    seek(position);
    qint8 tryRead = KTools::Converter::byteArrayToT<qint8>(read(1));
    if (tryRead != static_cast<qint8>(Type::String))
    {
        KLOG_ERROR("Wrong type or wrong position. position: " + QString::number(position));
        return QByteArray();
    }
    seek(position + 1);
    qint64 next = KTools::Converter::byteArrayToT<qint64>(read(8));
    seek(position + 9);
    qint64 varSize = KTools::Converter::byteArrayToT<qint64>(read(8));
    seek(position + 17);
    if (varSize <= Sizes::data)
    {
        result.append(read(varSize));
    }
    else
    {
        qint64 readed = 0;
        qint64 curr = 0;
        result.append(read(Sizes::data));
        readed += Sizes::data;
        while (1)
        {
            curr = next;
            seek(curr + 1);
            next = KTools::Converter::byteArrayToT<qint64>(read(8));
            seek(curr + 9);
            if (next == -1)
            {
                result.append(read(varSize - readed));
                break;
            }
            else
            {
                result.append(read(Sizes::occupiedData));
            }
            readed += Sizes::occupiedData;
        }
    }
    return result;
}

bool KTools::Kff::VariableTypes::deleteVariable(const qint64 position)
{
    seek(position);
    qint8 tryRead = KTools::Converter::byteArrayToT<qint8>(read(1));
    if (!(tryRead > 0 && tryRead < 3))
    {
        KLOG_ERROR("Wrong position. position: " + QString::number(position));
        return false;
    }
    QByteArray content;
    content.append(Sizes::all, '\0');
    qint64 lastPos = 0;
    qint64 next = position;
    while (next != -1)
    {
        lastPos = next;
        seek(lastPos + 1);
        next = KTools::Converter::byteArrayToT<qint64>(read(8));
        seek(lastPos);
        write(content);
    }
    return true;
}

bool KTools::Kff::VariableTypes::clearVariable(const qint64 position)
{
    seek(position);
    qint8 tryRead = KTools::Converter::byteArrayToT<qint8>(read(1));
    if (tryRead != static_cast<qint8>(Type::String))
    {
        KLOG_ERROR("Wrong position. position: " + QString::number(position));
        return false;
    }
    seek(position + 1);
    qint64 next = KTools::Converter::byteArrayToT<qint64>(read(8));
    if (next != -1)
    {
        deleteVariable(next);
    }
    QByteArray content;
    content.append(static_cast<quint8>(Type::String)); // Cluster type
    content.append(KTools::Converter::toByteArray(-1ll)); // Next cluster address
    content.append(KTools::Converter::toByteArray(0ll)); // Size of variable
    content.append(Sizes::data, '\0');
    seek(position);
    write(content);
    return true;
}
