#ifndef NAMEINFO_H

#include <QByteArray>
#include <QObject>
#include <QQmlEngine>

namespace ImageStorage {
    Q_NAMESPACE
    Q_CLASSINFO("RegisterEnumClassesUnscoped", "false")
    enum class NameAffinity {
        Attribute,
        Tag
    };
    enum class DataType : qint8 {
        Int8 = 1,
        Int16 = 2,
        Int32 = 3,
        Int64 = 4,
        UInt8 = 5,
        UInt16 = 6,
        UInt32 = 7,
        UInt64 = 8,
        Float = 9,
        Bool = 10,
        String = 11,
        List = 12
    };
    Q_ENUM_NS(DataType)

    struct NameInfo {
        Q_GADGET // Can not have signals or slots
    public:
        NameAffinity affinity;
        qint64 addr;
        DataType type;
        QByteArray name;
        QByteArray data;
        QByteArray description;
    };
    struct NameInfoList {
        Q_GADGET // Can not have signals or slots
    public:
        QList<NameInfo> items;
        Q_INVOKABLE void add(const NameInfo name) {items.append(name);}
        Q_INVOKABLE NameInfo at(const int i) {return items.at(i);}
    };

    inline void registerTypesForQml()
    {
        qmlRegisterUncreatableMetaObject(staticMetaObject, "ImageStorageNs", 1, 0, "ImageStorageNs", "It's enum");
        qRegisterMetaType<DataType>();
        /*qRegisterMetaType<NameInfo>();
        qRegisterMetaType<NameInfoList>();*/
    }
}

#define NAMEINFO_H

#endif // NAMEINFO_H
