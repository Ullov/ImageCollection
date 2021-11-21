#include "log.h"

#include <QDateTime>
#include <QIODevice>
#include "file.h"
#include "options.h"

QString KTools::Log::logFileName = QDateTime::currentDateTime().toString("yyyy.MM.dd") + ".log";

QMap<KTools::Enums::LogType, QString> KTools::Log::logTypePath = {
    {KTools::Enums::LogType::Info, "/info/"},
    {KTools::Enums::LogType::Debug, "/debug/"},
    {KTools::Enums::LogType::Error, "/error/"},
    {KTools::Enums::LogType::Custom, "/custom/"}
};
QMap<KTools::Enums::LogType, QString> KTools::Log::logTypeNames = {
    {KTools::Enums::LogType::Info, "Info"},
    {KTools::Enums::LogType::Debug, "Debug"},
    {KTools::Enums::LogType::Error, "Error"},
    {KTools::Enums::LogType::Custom, "Custom"}
};
KTools::Options KTools::Log::optionsObj;

KTools::Log::Log() {}

void KTools::Log::writeCustomLog(const QString &message, const QString &from, const KTools::Enums::LogType &type)
{
    QString dateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss:zzz");
    QString dt = '[' + from + "]\t" + dateTime + ' ' + message + '\n';
    File::writeFile(dt.toUtf8(), optionsObj.getParam("/Path/Log").toString() + logTypePath[type], logFileName, QIODevice::Append | QIODevice::WriteOnly);
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
