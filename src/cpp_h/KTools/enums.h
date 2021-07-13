#ifndef KTOOLS_ENUMS_H
#define KTOOLS_ENUMS_H

#include <QObject>
#include <QQmlEngine>

namespace KTools::Enums
{
    Q_NAMESPACE
    Q_CLASSINFO("RegisterEnumClassesUnscoped", "false")
    enum class LogType {
        Custom,
        Info,
        Error,
        Debug
    };
    Q_ENUM_NS(LogType)
    inline void registerTypesForQml()
    {
        qmlRegisterUncreatableMetaObject(staticMetaObject, "KEnums", 1, 0, "KEnums", "Is enum");
        qRegisterMetaType<LogType>("KEnumLogType");
    }
};

#endif // KTOOLS_ENUMS_H
