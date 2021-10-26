#include "options.h"

KTools::Options::Options() {}

KTools::Options::Options(QSqlDatabase &db)
{
    settings = db;
    settings.open();
    QSqlQuery res = settings.exec("SELECT * FROM General");
    if (!res.next())
    {
        settings.exec("CREATE TABLE General (Param TEXT, Value TEXT)");
        settings.exec("INSERT INTO General (Param, Value) VALUES ('DataPath', '')"); // Path to directory where almost al app's data saved. If '' it's app's directory

        settings.exec("CREATE TABLE FSExplorer (Param TEXT, Value TEXT)");
        QString err = settings.lastError().text();
        settings.exec("INSERT INTO FSExplorer (Param, Value) VALUES ('LastPath', 'C:/')");
    }
    /*res = settings.exec("SELECT * FROM FSExplorer");
    QString tmp = "";
    while (res.next())
    {
        tmp += res.value(0).toString() + ":" + res.value(1).toString() + "  ";
    }
    settings.close();*/
}

KTools::Options::~Options()
{
    settings.close();
    QString connName = settings.connectionName();
    settings = QSqlDatabase();
    QSqlDatabase::removeDatabase(connName);
}

void KTools::Options::updateParam(const QString &pathToParam, const QString &value)
{
    settings.open();
    QList<QString> list = pathToParam.split("/", Qt::SkipEmptyParts);
    QString query = "UPDATE " + list[0] + " SET Value = '" + value + "' WHERE Param = '" + list[1] + "'";
    QSqlQuery resp = settings.exec(query);
    QSqlError err = resp.lastError();
    QSqlError::ErrorType errType = err.type();
    QString strErr = err.text();
}

QVariant KTools::Options::getParam(const QString &pathToParam)
{
    QList<QString> list = pathToParam.split("/", Qt::SkipEmptyParts);
    QSqlQuery resp;
    QString res;
    settings.open();
    if (list[0] == "Path")
    {
        resp = settings.exec("SELECT Value FROM General WHERE Param = 'DataPath'");
        resp.next();
        QString dataPath = resp.value(0).toString();
        if (dataPath == "")
            dataPath = QDir().path();
        if (list[1] == "Log")
            res = dataPath + "/Log";
        else if (list[1] == "Data")
            res = dataPath;
    }
    else
    {
        resp = settings.exec("SELECT Value FROM " + list[0] + " WHERE Param = '" + list[1] + "'");
        resp.next();
        res = resp.value(0).toString();
    }
    return res;
}
