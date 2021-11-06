#include "imagestorage.h"

ImageStorage::ImageStorage() {}

ImageStorage::ImageStorage(QSqlDatabase &db)
{
    data = db;
    openDb();
    QSqlQuery resp = exec("SELECT * FROM Image");
    if (resp.lastError().isValid())
    {
        exec(\
"CREATE TABLE Image (\
Id               INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,\
Name             TEXT NOT NULL,\
Hash             TEXT NOT NULL,\
Size             INT NOT NULL,\
Height           INT NOT NULL,\
Width            INT NOT NULL,\
GalleryId        INT NULL,\
Extension        INT NOT NULL,\
FormerExtension  INT NOT NULL,\
CreationTime     INT NOT NULL,\
ModificationTime INT NOT NULL,\
AdditionTime     INT NOT NULL,\
ImageData        BLOB NOT NULL\
)");
        exec("\
CREATE TABLE ImageTag (\
Id      INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,\
ImageId INT NOT NULL,\
TagId   INT NOT NULL\
)");
        exec("\
CREATE TABLE Tag (\
Id               INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,\
Name             TEXT NOT NULL,\
ParentTagId      INT NULL,\
Summary          TEXT NULL,\
CreationTime     INT NOT NULL,\
ModificationTime INT NOT NULL\
)");
        exec("\
CREATE TABLE ParentChildTag (\
Id          INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,\
ParentTagId INT NOT NULL,\
ChildTagId  INT NOT NULL\
)");
        exec("\
CREATE TABLE Gallery (\
Id               INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,\
Name             TEXT NOT NULL,\
Summary          TEXT NULL,\
CreationTime     INT NOT NULL,\
ModificationTime INT NOT NULL\
)");
        exec("\
CREATE TABLE GalleryImage (\
Id      INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,\
Gallery Id INT NOT NULL,\
ImageId INT NOT NULL\
)");
        exec("\
CREATE TABLE ExtensionEnum (\
Id      INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,\
Value   TEXT NOT NULL\
)");
        exec("INSERT INTO ExtensionEnum (Value) VALUES ('jpg')");
        exec("INSERT INTO ExtensionEnum (Value) VALUES ('jpeg')");
        exec("INSERT INTO ExtensionEnum (Value) VALUES ('webp')");
        exec("INSERT INTO ExtensionEnum (Value) VALUES ('png')");
        exec("INSERT INTO ExtensionEnum (Value) VALUES ('tiff')");
        exec("INSERT INTO ExtensionEnum (Value) VALUES ('gif')");
    }
}

QSqlQuery ImageStorage::exec(const QString &query)
{
    if (!data.isOpen())
        openDb();
    QSqlQuery resp(data);
    resp.exec(query);
    if (resp.lastError().isValid())
        KTools::Log::writeError("Error in query. Error text: " + resp.lastError().text() + " || Query: " + query, "ImageStorage::exec()");
    return resp;
}

bool ImageStorage::openDb()
{
    bool res = data.open();
    if (!res)
        KTools::Log::writeError("Database not opened. Error text: " + data.lastError().text(), "ImageStorage::open()");
    return res;
}

QString ImageStorage::addTag(const QVariantMap info)
{
    QString currTime = QString::number(QDateTime::currentDateTime().toSecsSinceEpoch());
    QString query;
    if (info["parentTag"] == "")
    {
        QString values = "'" + info["name"].toString() + "', '" + info["summary"].toString() + "', " + currTime + ", " + currTime;
        query = "INSERT INTO Tag (Name, Summary, CreationTime, ModificationTime) VALUES (" + values + ")";
    }
    else
    {
        QList<QSqlQuery> parentTags = getTag(info["parentTag"].toString());
        if (!parentTags.last().isValid())
        {
            KTools::Log::writeError("Parent tag not found. info.parentTag: " + info["parentTag"].toString(), "ImageStorage::addTag()");
            return "Parent tag not found";
        }
        QString values = "'" + info["name"].toString() + "', " + QString::number(parentTags.last().value(0).toInt()) + ", '" + info["summary"].toString() + "', " + currTime + ", " + currTime;
        query = "INSERT INTO Tag (Name, ParentTagId, Summary, CreationTime, ModificationTime) VALUES (" + values + ")";
    }
    QSqlQuery resp = exec(query);
    if (resp.lastError().isValid())
    {
        KTools::Log::writeError("Error while adding tag. query: " + query, "ImageStorage::addTag()");
        return "Parent tag not found";
    }
    return "Tag added";
}

QList<QSqlQuery> ImageStorage::getTag(const QString &tags, const getTagParams &param)
{
    QString values;
    QString query;
    QList<QSqlQuery> result;
    QList<QString> list;
    if (param == getTagParams::All)
        values = "Id, Name, ParentTagId, Summary, CreationTime, ModificationTime";
    else if (param == getTagParams::Id)
        values = "Id";

    if (tags.contains(':'))
        list = tags.split(":", Qt::SkipEmptyParts);
    else
        list.append(tags);

    query = "SELECT " + values + " FROM Tag WHERE Name = '" + list[0] + "'";
    QSqlQuery res = exec(query);
    result.append(res);
    if (!res.next())
    {
        KTools::Log::writeError("Tag does not exist. tags: " + tags + " | query: " + query , "ImageStorage::getTag()");
        return result;
    }

    if (list.length() > 1)
    {
        for (int i = 1; i < list.length(); i++)
        {
            QString parentId = QString::number(result.last().value(0).toInt());
            query = "SELECT " + values + " FROM Tag WHERE (Name = '" + list[i] + "' AND ParentTagId = " + parentId + ")";
            result.append(exec(query));
            if (!result.last().next())
            {
                KTools::Log::writeError("Tag does not exist. tags: " + tags + " | query: " + query, "ImageStorage::getTag()");
                return result;
            }
        }
    }
    return result;
}

QVariantList ImageStorage::getAllTags()
{
    QVariantList result;
    QVariantMap item;
    QString query = "SELECT * FROM Tag";
    QSqlQuery reply = exec(query);
    while (reply.next())
    {
        QString tagName = reply.value(1).toString();
        if (!reply.value(2).isNull())
        {
            QList<QVariant> namesList;
            QString parentId = reply.value(2).toString();
            bool run = true;
            while (run)
            {
                QString query = "SELECT * FROM Tag WHERE Id = " + parentId;
                QSqlQuery localReply = exec(query);
                localReply.next();
                namesList.append(localReply.value(1).toString());
                if (localReply.value(2).isNull())
                    run = false;
                else
                    parentId = localReply.value(2).toString();
            }
            tagName = "";
            for (int i = namesList.length() - 1; i  >= 0; i--)
            {
                tagName += namesList[i].toString() + ":";
            }
            tagName += reply.value(1).toString();
        }

        item.insert("id", reply.value(0));
        item.insert("name", tagName);
        item.insert("parentTagId", reply.value(2).toString());
        item.insert("summary", reply.value(3));
        item.insert("creationTime", reply.value(4));
        item.insert("modificationTime", reply.value(5));
        result.append(item);
        item = QVariantMap();
    }
    return result;
}

QSqlQuery ImageStorage::execWithBind(const QByteArray &query, const QString &bindedValue, const QSql::ParamType bindFlags)
{

}
