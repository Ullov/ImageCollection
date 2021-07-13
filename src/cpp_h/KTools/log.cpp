#include "log.h"

#include <QDateTime>
#include <QIODevice>
#include "file.h"
#include "options.h"

#ifdef KAWAI_DEBUG
#include <iostream>
#endif

KTools::Log::Log() {}

void KTools::Log::writeCustomLog(const QString &message, const QString &from, const KTools::Enums::LogType &type)
{
    QString dateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss:zzz");
    QString dt = '[' + from + ']' + dateTime + ' ' + message + '\n';
    File::writeFile(dt.toUtf8(), Options::logRootPath + logTypePath[type], logFileName, QIODevice::Append | QIODevice::WriteOnly);
#ifdef KAWAI_DEBUG
    if (type == Enums::LogType::Info)
        std::clog << "Info: " << dt.data();
    else if (type == Enums::LogType::Debug)
        std::clog << "Debug: " << dt.data();
    else if (type == Enums::LogType::Error)
        std::cerr << "Error: " << dt.data();
    else if (type == Enums::LogType::Custom)
        std::clog << "Custom: " << dt.data();
    else
        std::clog << "Undefined log type: " << dt.data();
#endif
}

void KTools::Log::writeCustomLog(const QString &message, const QString &from, const KTools::Enums::LogType &type, const QString &path, const QString &fileName)
{
    QString dateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss:zzz");
    QString dt = '[' + logTypeNames[type] + "][" + from + ']' + dateTime + ' ' + message + '\n';
    File::writeFile(dt.toUtf8(), path, fileName, QIODevice::Append | QIODevice::WriteOnly);
}

void KTools::Log::writeInfo(const QString &message, const QString &from)
{
    writeCustomLog(message, from, KTools::Enums::LogType::Info);
}

void KTools::Log::writeError(const QString &message, const QString &from)
{
    writeCustomLog(message, from, KTools::Enums::LogType::Error);
}

void KTools::Log::writeDebug(const QString &message, const QString &from)
{
    writeCustomLog(message, from, KTools::Enums::LogType::Debug);
}
