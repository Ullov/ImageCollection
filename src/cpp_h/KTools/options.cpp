#include "options.h"
#include "../FsExplorer/fshandler.h"

KTools::Options::Options() : KTools::Kff::Manager(QDir::currentPath() + "/options.kff", KTools::Kff::Manager::OpenMode::Keep)
{
    using KTools::Converter;
    KTools::Log::assignOptionObject(this);
    FsHandler::optionsObj = this;
    if (mode == Kff::Manager::OpenMode::Clear)
    {
        addStringVariable("Path:Data", Converter::convert<QString, QByteArray>(QDir::currentPath()));
        addStringVariable("Path:Log", Converter::convert<QString, QByteArray>(QDir::currentPath()) + "/Log/");
        addStringVariable("FSExplorer:LastPath", "C:/");
    }
    KLOG_DEBUG(getParam("LastPath").toByteArray());
}

void KTools::Options::updateParam(const QString &name, const QByteArray &value, const ParamType type)
{
    for (int i = 0; i < defaultStream->size(); i += 19)
    {
        defaultStream->seek(i);
        QByteArray entity = defaultStream->read(19);
        if (entity[0] != '\0')
        {
            QByteArray readedName = getDataFromPointer(entity.mid(1, 9));
            if (readedName == name)
            {
                if (type == ParamType::String)
                {
                    qint64 addr = strs->rewriteVariable(value, KTools::Converter::byteArrayToT<qint64>(entity.mid(11)), KTools::Kff::VariableTypes::Type::String);
                    entity.insert(11, KTools::Converter::toByteArray(addr));
                    defaultStream->seek(i);
                    defaultStream->write(entity);
                }
                else
                {
                    switch (type)
                    {
                        case ParamType::Int8 : numbers->change(KTools::Converter::byteArrayToT<qint8>(value), KTools::Converter::byteArrayToT<qint64>(entity.mid(12))); break;
                        case ParamType::Int16 : numbers->change(KTools::Converter::byteArrayToT<qint16>(value), KTools::Converter::byteArrayToT<qint64>(entity.mid(12))); break;
                        case ParamType::Int32 : numbers->change(KTools::Converter::byteArrayToT<qint32>(value), KTools::Converter::byteArrayToT<qint64>(entity.mid(12))); break;
                        case ParamType::Int64 : numbers->change(KTools::Converter::byteArrayToT<qint64>(value), KTools::Converter::byteArrayToT<qint64>(entity.mid(12))); break;
                        case ParamType::UInt8 : numbers->change(KTools::Converter::byteArrayToT<quint8>(value), KTools::Converter::byteArrayToT<qint64>(entity.mid(12))); break;
                        case ParamType::UInt16 : numbers->change(KTools::Converter::byteArrayToT<quint16>(value), KTools::Converter::byteArrayToT<qint64>(entity.mid(12))); break;
                        case ParamType::UInt32 : numbers->change(KTools::Converter::byteArrayToT<quint32>(value), KTools::Converter::byteArrayToT<qint64>(entity.mid(12))); break;
                        case ParamType::UInt64 : numbers->change(KTools::Converter::byteArrayToT<quint64>(value), KTools::Converter::byteArrayToT<qint64>(entity.mid(12))); break;
                        case ParamType::Bool : numbers->change(KTools::Converter::byteArrayToT<bool>(value), KTools::Converter::byteArrayToT<qint64>(entity.mid(12))); break;
                        case ParamType::Float : numbers->change(KTools::Converter::byteArrayToT<double>(value), KTools::Converter::byteArrayToT<qint64>(entity.mid(12))); break;
                        default : KLOG_ERROR("There is something crazy"); break; // To silence warning
                    }
                }
                break;
            }
        }
    }
}

QVariant KTools::Options::getParam(const QString &name)
{
    QVariant result;
    for (int i = 0; i < defaultStream->size(); i += 19)
    {
        defaultStream->seek(i);
        QByteArray entity = defaultStream->read(19);
        if (entity[0] != '\0')
        {
            QByteArray readedName = getDataFromPointer(entity.mid(1, 9));
            if (readedName == name)
            {
                result = getDataFromPointer(entity.mid(10));
                break;
            }
        }
    }
    return result;
}

void KTools::Options::addInt8Variable(const QByteArray &name, const qint8 data)
{
    QByteArray content;
    content.append(static_cast<quint8>(ParamType::Int8));

    qint64 address = strs->add(name, Kff::VariableTypes::Type::String);
    QByteArray pointer = makePointer(PointerType::VariableTypes, address);
    content.append(pointer);

    address = numbers->add(data, Kff::FixedTypes::Type::Int8);
    pointer = makePointer(PointerType::FixedTypes, address);
    content.append(pointer);

    for (int i = 0; i < defaultStream->size(); i += content.size())
    {
        defaultStream->seek(i);
        if (*defaultStream->read(1) == '\0')
        {
            defaultStream->seek(i);
            defaultStream->write(content);
            break;
        }
    }
}

void KTools::Options::addStringVariable(const QByteArray &name, const QByteArray &data)
{
    QByteArray content;
    content.append(static_cast<quint8>(ParamType::Int8));

    qint64 address = strs->add(name, Kff::VariableTypes::Type::String);
    QByteArray pointer = makePointer(PointerType::VariableTypes, address);
    content.append(pointer);

    address = strs->add(data, Kff::VariableTypes::Type::String);
    pointer = makePointer(PointerType::VariableTypes, address);
    content.append(pointer);

    for (int i = 0; i < defaultStream->size() + 19; i += content.size())
    {
        defaultStream->seek(i);
        if (*defaultStream->read(1) == '\0')
        {
            defaultStream->seek(i);
            defaultStream->write(content);
            break;
        }
    }
}
