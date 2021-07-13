#ifndef KTOOLS_LOG_H
#define KTOOLS_LOG_H

#include "enums.h"

namespace KTools
{
    class Log
    {
    public:
        Log();

    private:
        static QString logFileName;
        static QMap<KTools::Enums::LogType, QString> logTypePath;
        static QMap<KTools::Enums::LogType, QString> logTypeNames;

    public:
        static void writeCustomLog(const QString &message, const QString &from, const KTools::Enums::LogType &type = KTools::Enums::LogType::Custom);
        static void writeCustomLog(const QString &message, const QString &from, const KTools::Enums::LogType &type, const QString &path, const QString &fileName);
        static void writeInfo(const QString &message, const QString &from);
        static void writeError(const QString &message, const QString &from);
        static void writeDebug(const QString &message, const QString &from);
    };
}

#endif // KTOOLS_LOG_H
