#ifndef KTOOLS_OPTIONS_H
#define KTOOLS_OPTIONS_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QJsonArray>
#include <QJsonObject>
#include <QSqlError>
#include <QDir>

namespace KTools
{
    class Options
    {
    public:
        Options();
        Options(QSqlDatabase &db);
        ~Options();

        void updateParam(const QString &pathToParam, const QString &value);
        QVariant getParam(const QString &pathToParam);

    private:
        QSqlDatabase settings;
    };
}

#endif // KTOOLS_OPTIONS_H
