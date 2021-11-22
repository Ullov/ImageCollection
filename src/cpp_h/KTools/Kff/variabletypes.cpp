#include "variabletypes.h"

KTools::Kff::VariableTypes::VariableTypes(Manager *man) : RawStream(man, true) {}

qint64 KTools::Kff::VariableTypes::add(const QByteArray &data, const Type type)
{
    bool run = true;
    bool first = true;
    qint64 firstClsAddr = -1;
    qint64 lastSeek = 0;
    qint64 writed = 0;
    qint64 sizeAddress = 0;
    qint64 previousClsAddr = 0;
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
            content.append(static_cast<quint8>(type)); // Cluster type
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

QByteArray KTools::Kff::VariableTypes::readString(const qint64 position)
{
    return readVariable(position, Type::String);
}

qint64 KTools::Kff::VariableTypes::appendPointers(const QList<QByteArray> &pointers, const qint64 position)
{
    QByteArray content;
    for (int i = 0; i < pointers.size(); i++)
        content.append(pointers[i]);
    seek(position);
    qint8 tryRead = KTools::Converter::byteArrayToT<qint8>(read(1));
    if (position == -1)
    {
        QByteArray content;
        for (int i = 0; i < pointers.length(); i++)
            content.append(pointers[i]);
        return add(content, Type::ListOfPointers);
    }
    else if (tryRead == static_cast<qint8>(Type::String) || tryRead == static_cast<qint8>(Type::OccupiedCls))
    {
        KLOG_ERROR("Trying rewrite variable with different type or wrong position. position: " + QString::number(position));
    }
    else if (tryRead == 0)
    {
        return add(content, Type::ListOfPointers);
    }
    else if (tryRead == static_cast<qint8>(Type::ListOfPointers))
    {
        QByteArray currValue = readVariable(position, Type::ListOfPointers);
        return rewriteVariable(currValue + content, position, Type::ListOfPointers);
    }
    else
    {
        KLOG_ERROR("Wrong position. position: " + QString::number(position) + " ... tryRead: " + QString::number(tryRead));
    }
    return -1;
}

qint64 KTools::Kff::VariableTypes::rewriteVariable(const QByteArray &data, const qint64 position, const Type type)
{
    seek(position);
    qint8 tryRead = KTools::Converter::byteArrayToT<qint8>(read(1));
    if (tryRead != static_cast<qint8>(type))
    {
        KLOG_ERROR("Wrong type or wrong position. position: " + QString::number(position));
        return -1;
    }
    deleteVariable(position);
    return add(data, type);
}

QByteArray KTools::Kff::VariableTypes::readVariable(const qint64 position, const Type type)
{
    QByteArray result;
    seek(position);
    qint8 tryRead = KTools::Converter::byteArrayToT<qint8>(read(1));
    if (tryRead != static_cast<qint8>(type))
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
    if (tryRead < 1 || tryRead > 3)
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

QList<QByteArray> KTools::Kff::VariableTypes::getPointers(const qint64 position)
{
    QList<QByteArray> result;
    QByteArray rawData = readVariable(position, Type::ListOfPointers);
    for (int i = 0; i < rawData.length(); i += 9)
    {
        result.append(rawData.mid(i, 9));
    }
    return result;
}
