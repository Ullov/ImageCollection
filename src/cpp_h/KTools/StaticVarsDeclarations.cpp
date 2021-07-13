#include "options.h"
#include "log.h"
#include "enums.h"
#include "fileidentifier.h"

#include <QStandardPaths>
#include <QDir>
#include <QDateTime>

// Start OptionsHandler
QString KTools::Options::configFile = "config.txt";
QString KTools::Options::configPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/KawaiClient/";
QString KTools::Options::rootProgramPath = "";
QString KTools::Options::logRootPath = "";

QJsonObject *KTools::Options::configsObj = new QJsonObject();
// End OptionsHandler
// Start Log
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
// End Log
