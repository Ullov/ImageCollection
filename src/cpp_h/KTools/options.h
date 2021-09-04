#ifndef KTOOLS_OPTIONS_H
#define KTOOLS_OPTIONS_H

#include "curl.h"

class QJsonObject;
class QVariant;
class QJsonValue;

namespace KTools
{
    class Options
    {
    public:
        Options();
        QString getRootProgramPath();

        static QJsonObject *configsObj;
        static QString configPath;
        static QString configFile;
        static QString rootProgramPath;
        static QString logRootPath;

        static void setRootProgramPath(const QString &path);
        static void setParam(const QString &pathToParam, const QString &param); // wrapper for privateSetParam() for C++
        static QVariant getParam(const QString &pathToParam); // wrapper for privateGetParam() for C++

    private:
        static bool save(); // saving current settings in native file system
        static QJsonValue privateSetParam(QList<QString> pathToParam, QJsonValue currLevel, const QVariant param); // private method because for it need wrapper
        static QJsonValue privateGetParam(QList<QString> pathToParam, const QJsonValue &previousLevel); // private method because for it need wrapper
    };
}

#endif // KTOOLS_OPTIONS_H
