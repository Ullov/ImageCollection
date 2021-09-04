#include "options.h"

#include <QJsonArray>
#include <QJsonObject>
#include "file.h"
#include "curl.h"
#include "converter.h"
#include "log.h"

KTools::Options::Options()
{
    bool wtf = KTools::File::fileExist(configPath + configFile);
    if (wtf)
    {
        rootProgramPath = KTools::File::readFile<QString>(configPath, configFile, QIODevice::ReadOnly | QIODevice::Text);
    }
    else
    {
        rootProgramPath = configPath;
        KTools::File::writeFile(configPath.toUtf8(), configPath, configFile, QIODevice::WriteOnly | QIODevice::Text);
    }
    logRootPath = Options::rootProgramPath + "/log/";
    KTools::Curl::cookiePath = Options::rootProgramPath + "/Cookie/";


    if (!KTools::File::fileExist(rootProgramPath + "/Settings/configs.json"))
    {
        KTools::File::copyFile(":/resources/sampleFiles/configs.json", rootProgramPath + "/Settings", "configs.json");
    }
    QString fileContent = KTools::File::readFile<QString>(rootProgramPath + "/Settings", "configs.json");
    configsObj = KTools::Converter::convert<QString, QJsonObject*>(fileContent);
}

bool KTools::Options::save()
{
    return KTools::File::writeFile(KTools::Converter::convert<QJsonObject, QByteArray>(*configsObj), rootProgramPath + "/Settings", "configs.json");
}

void KTools::Options::setRootProgramPath(const QString &path)
{
    rootProgramPath = path;
    KTools::File::writeFile(rootProgramPath.toUtf8(), configPath, configFile, QIODevice::WriteOnly | QIODevice::Text);
    save();
}

QString KTools::Options::getRootProgramPath()
{
    return rootProgramPath;
}

QJsonValue KTools::Options::privateSetParam(QList<QString> pathToParam, QJsonValue currLevel, const QVariant param)
{
    QString name = pathToParam[0];
    pathToParam.pop_front();
    QJsonValue::Type jsValType = currLevel.type();
    if (jsValType == QJsonValue::Type::Undefined)
    {
        KTools::Log::writeError("Attempt access to non existent parameter. name: " + name, "OptionsHandler::setParam()");
        return QJsonValue();
    }
    if (pathToParam.size() > 1)
    {
        if (jsValType == QJsonValue::Type::Array)
        {
            QJsonArray jsVal = currLevel.toArray();
            QJsonValueRef tmp = jsVal[name.toInt()];
            jsVal[name.toInt()] = privateSetParam(pathToParam, tmp, param);
            return jsVal;
        }
        else if (jsValType == QJsonValue::Type::Object)
        {
            QJsonObject jsVal = currLevel.toObject();
            QJsonValueRef tmp = jsVal[name];
            jsVal[name] = privateSetParam(pathToParam, tmp, param);
            return jsVal;
        }
        else
        {
            KTools::Log::writeError("Wrong type. jsValType: " + static_cast<QString>(jsValType) + ", name: " + name, "OptionsHandler::setParam");
        }
    }
    else
    {
        if (jsValType != QJsonValue::Type::Array)
        {
            QJsonObject jsVal = currLevel.toObject();
            jsVal[name] = QJsonValue::fromVariant(param);
            return jsVal;
        }
        else if (jsValType == QJsonValue::Type::Array)
        {
            QJsonArray jsVal = currLevel.toArray();
            jsVal[name.toInt()] = QJsonValue::fromVariant(param);
            return jsVal;
        }
    }
    KTools::Log::writeError("Something went wrong. name: " + name, "OptionsHandler::privateSetParam()");
    return QJsonValue();
}

QJsonValue KTools::Options::privateGetParam(QList<QString> pathToParam, const QJsonValue &previousLevel)
{
    QString name = pathToParam[0];
    pathToParam.pop_front();
    QJsonValue::Type jsValType = previousLevel.type();
    if (jsValType == QJsonValue::Type::Undefined)
    {
        KTools::Log::writeError("Attempt access to non existent parameter. name: " + name, "OptionsHandler::getParam()");
        return QJsonValue();
    }

    if (pathToParam.size() > 1)
    {
        if (jsValType == QJsonValue::Type::Array)
        {
            QJsonArray jsVal = previousLevel.toArray();
            QJsonValueRef tmp = jsVal[name.toInt()];
            return privateGetParam(pathToParam, tmp);
        }
        else if (jsValType == QJsonValue::Type::Object)
        {
            QJsonObject jsVal = previousLevel.toObject();
            QJsonValueRef tmp = jsVal[name];
            return privateGetParam(pathToParam, tmp);
        }
        else
        {
            KTools::Log::writeError("Wrong type. jsValType: " + static_cast<QString>(jsValType) + ", name: " + name, "OptionsHandler::getParam");
            return QJsonValue();
        }
    }
    else
    {
        if (jsValType != QJsonValue::Type::Array)
        {
            QJsonObject jsVal = previousLevel.toObject();
            return jsVal[name];
        }
        else if (jsValType == QJsonValue::Type::Array)
        {
            QJsonArray jsVal = previousLevel.toArray();
            return jsVal[name.toInt()];
        }
    }
    KTools::Log::writeError("Something went wrong. name: " + name, "OptionsHandler::privateGetParam()");
    return QJsonValue();
}

void KTools::Options::setParam(const QString &pathToParam, const QString &param)
{
    QList<QString> list = pathToParam.split("/", Qt::SkipEmptyParts);
    QString name = list[0];
    list.pop_front();
    QJsonValueRef jsVal = configsObj->operator[](name);
    configsObj->operator[](name) = privateSetParam(list, jsVal, param);
    save();
}

QVariant KTools::Options::getParam(const QString &pathToParam)
{
    QList<QString> list = pathToParam.split("/", Qt::SkipEmptyParts);
    QString name = list[0];
    list.pop_front();
    QJsonValue jsVal = configsObj->operator[](name);
    return privateGetParam(list, jsVal);
}
