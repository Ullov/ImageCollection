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
ImageData        BLOB NOT NULL)");

        exec("\
CREATE TABLE ImageTag (\
Id      INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,\
ImageId INT NOT NULL,\
TagId   INT NOT NULL)");

        exec("\
CREATE TABLE Tag (\
Id               INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,\
ParentTagId      INT NULL)");

        exec("\
CREATE TABLE ParentChildTag (\
Id          INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,\
ParentTagId INT NOT NULL,\
ChildTagId  INT NOT NULL)");

        exec("\
CREATE TABLE Gallery (\
Id               INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE)");

        exec("\
CREATE TABLE GalleryImage (\
Id        INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,\
GalleryId Id INT NOT NULL,\
ImageId   INT NOT NULL)");

        exec("\
CREATE TABLE ExtensionEnum (\
Id      INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,\
Value   TEXT NOT NULL)");
        exec("INSERT INTO ExtensionEnum (Value) VALUES ('jpg')");
        exec("INSERT INTO ExtensionEnum (Value) VALUES ('jpeg')");
        exec("INSERT INTO ExtensionEnum (Value) VALUES ('webp')");
        exec("INSERT INTO ExtensionEnum (Value) VALUES ('png')");
        exec("INSERT INTO ExtensionEnum (Value) VALUES ('tiff')");
        exec("INSERT INTO ExtensionEnum (Value) VALUES ('gif')");

        exec("\
CREATE TABLE Attribute (\
Id                  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,\
Name                INT NOT NULL,\
Value               BLOB NULL)");

        exec("\
CREATE TABLE AttributeName (\
Id      INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,\
Name    TEXT NOT NULL)");

        exec("\
CREATE TABLE ParentChildAttribute (\
Id                  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,\
ParentAttributeId   INT NOT NULL,\
ChildAttributeId    INT NOT NULL)");

        exec("\
CREATE TABLE List (\
Id       INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,\
DataType INT NOT NULL)");

        exec("\
CREATE TABLE ListItems (\
Id      INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,\
ListId  INT NOT NULL,\
Value   BLOB NULL)");

        exec("\
CREATE TABLE DataTypeEnum (\
Id      INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,\
Value   TEXT NOT NULL)");
        exec("INSERT INTO DataTypeEnum (Value) VALUES ('INT')");
        exec("INSERT INTO DataTypeEnum (Value) VALUES ('REAL')");
        exec("INSERT INTO DataTypeEnum (Value) VALUES ('TEXT')");
        exec("INSERT INTO DataTypeEnum (Value) VALUES ('BOOL')");
        exec("INSERT INTO DataTypeEnum (Value) VALUES ('BLOB')");
        exec("INSERT INTO DataTypeEnum (Value) VALUES ('LIST')");
        exec("INSERT INTO DataTypeEnum (Value) VALUES ('NULL')");

        exec("\
CREATE TABLE ImageAttribute (\
Id          INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,\
ImageId     INT NOT NULL,\
AttributeId INT NOT NULL,\
DataType    INT NOT NULL)");

        exec("\
CREATE TABLE TagAttribute (\
Id          INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,\
TagId       INT NOT NULL,\
AttributeId INT NOT NULL,\
DataType    INT NOT NULL)");

        exec("\
CREATE TABLE GalleryAttribute (\
Id          INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,\
GalleryId   INT NOT NULL,\
AttributeId INT NOT NULL,\
DataType    INT NOT NULL)");

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
    if (info["name"].toString().contains(':'))
        return "Tag name contains colon (:)";
    QString currTime = QString::number(QDateTime::currentDateTime().toSecsSinceEpoch());
    QString query;
    if (info["parentTag"] == "")
    {
        QString values = "'" + info["name"].toString() + "', '" + info["summary"].toString() + "', " + currTime + ", " + currTime;
        query = "INSERT INTO Tag VALUES ()";
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

QList<QSqlQuery> ImageStorage::getTag(const QString &tags)
{
    QString query;
    QList<QSqlQuery> result;
    QList<QString> list;

    if (tags.contains(':'))
        list = tags.split(":", Qt::SkipEmptyParts);
    else
        list.append(tags);

    query = "SELECT * FROM AttributeName WHERE (Name = 'Name' AND ParentAttributeId IS NULL AND DataType = " + getDataTypeId(attributeDataType::Text);
    QSqlQuery res = exec(query);
    if (!res.next())
    {
        KTools::Log::writeError("System attribute with name 'Name' does not exist. Probably not even one tag created. tags: " + tags + " | query: " + query , "ImageStorage::getTag()");
        return result;
    }
    QString tagNameAttrId = res.value(0).toString();

    query = "SELECT * FROM Attribute WHERE (Name = " + tagNameAttrId + " AND Value = x'" + list[0] + "')";
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
            query = "SELECT * FROM Tag WHERE (Name = '" + list[i] + "' AND ParentTagId = " + parentId + ")";
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

QVariantMap ImageStorage::writeAttribute(const QString &name, const QByteArray &value, const attributeDataType dataType, const attributeAssociation association, const int associateId)
{
    QVariantMap result;
    QString query;
    QSqlQuery reply;

    QString dataTypeId = getDataTypeId(dataType);
    QVariantMap creationResult = createAttributeName(name, dataType);
    if (creationResult["error"] != "")
    {
        result["error"] = "Error while creating attribute name. name: " + name + " || value: " + value;
        KTools::Log::writeError(result["error"].toString(), "ImageStorage::writeAttribute()");
        return result;
    }

    query = "INSERT INTO Attribute (Name, Value) VALUES (" + creationResult["nameId"].toString() + ", x'" + value + "')";
    reply = exec(query);
    if (reply.lastError().isValid())
    {
        result["error"] = "Error while inserting attribute data. query: " + query;
        KTools::Log::writeError(result["error"].toString(), "ImageStorage::writeAttribute()");
        return result;
    }

    query = "SELECT * FROM Attribute WHERE Id = (SELECT MAX(Id) FROM Attribute)";
    reply = exec(query);
    QString associateName;
    if (association == attributeAssociation::Tag)
        associateName = "Tag";
    else if (association == attributeAssociation::Image)
        associateName = "Image";
    else if (association == attributeAssociation::Gallery)
        associateName = "Gallery";
    else
    {
        result["error"] = "Unexpected association value.";
        KTools::Log::writeError(result["error"].toString(), "ImageStorage::writeAttribute()");
        return result;
    }
    query = "INSERT INTO " + associateName + "Attribute (" + associateName + "Id, AttributeId) VALUES (" + QString::number(associateId) + ", " + reply.value(0).toString() + ")";
    reply = exec(query);
    if (reply.lastError().isValid())
    {
        result["error"] = "Error while connecting " + associateName + " and attribute. query: " + query;
        KTools::Log::writeError(result["error"].toString(), "ImageStorage::writeAttribute()");
        exec("DELETE FROM Attribute WHERE Id = (SELECT MAX(Id) FROM Attribute)");
        return result;
    }

    result["error"] = "";
    return result;
}

QString ImageStorage::getDataTypeId(const attributeDataType type)
{
    QString query = "SELECT Id FROM DataTypeEnum WHERE Value = ";
    if (type == attributeDataType::Blob)
        query += "'BLOB'";
    else if (type == attributeDataType::Bool)
        query += "'BOOL'";
    else if (type == attributeDataType::Int)
        query += "'INT'";
    else if (type == attributeDataType::List)
        query += "'LIST'";
    else if (type == attributeDataType::Real)
        query += "'REAL'";
    else if (type == attributeDataType::Text)
        query += "'TEXT'";
    else if (type == attributeDataType::Null)
        query += "'NULL'";

    QSqlQuery reply = exec(query);
    if (!reply.next())
    {
        KTools::Log::writeError("Can not retrieve DataType integer value. query:" + query, "ImageStorage::getDataTypeId()");
        return "-1";
    }

    return reply.value(0).toString();
}

QList<QSqlQuery> ImageStorage::getAttributesChain(const QString &attributes, const getAttributesChainParam param)
{
    QList<QSqlQuery> result;
    QList<QString> list;
    if (attributes.contains(':'))
        list = attributes.split(':', Qt::SkipEmptyParts);
    else
    {
        KTools::Log::writeError("Passed root level attribute. attributes: " + attributes, "ImageStorage::getParentAttributes()");
        return result;
    }

    QString query = "SELECT * FROM AttributeName WHERE Name = '" + list[0] + "'";
    result.append(exec(query));
    if (!result.last().next())
    {
        KTools::Log::writeError("Probably wrong parent attribute name. query: " + query, "ImageStorage::getParentAttributes()");
        return result;
    }

    int iterationLimit = 0;
    if (param == getAttributesChainParam::All)
        iterationLimit = list.length();
    if (param == getAttributesChainParam::Parents)
        iterationLimit = list.length() - 1;

    for (int i = 1; i < iterationLimit; i++)
    {
        query = "SELECT * FROM AttributeName WHERE (Name = '" + list[i] + "' AND ParentAttributeId = " + result.last().value(0).toString() + ")";
        result.append(exec(query));
        if (!result.last().next())
        {
            KTools::Log::writeError("Probably wrong parent attribute name or id. query: " + query, "ImageStorage::getParentAttributes()");
            return result;
        }
    }
    return result;
}

QVariantMap ImageStorage::createAttributeName(const QString &name, const attributeDataType dataType)
{

    bool parentNowCreated = false;
    QString query;
    QVariantMap result;
    QList<QString> list;
    QSqlQuery baseSelectReply;
    QSqlQuery reply;
    if (dataType == attributeDataType::Null)
    {
        result["error"] = "Attempt create attribute with NULL data type.";
        KTools::Log::writeError(result["error"].toString(), "ImageStorage::createAttributeName()");
        return result;
    }

    if (name.contains(':'))
        list = name.split(':', Qt::SkipEmptyParts);
    else
        list.append(name);

    query = "SELECT * FROM AttributeName WHERE (Name = '" + list[0] + "' AND ParentAttributeId IS NULL";
    baseSelectReply = exec(query);
    if (baseSelectReply.lastError().isValid())
    {
        result["error"] = "Error in query on retreiving root level tag. query:" + query;
        KTools::Log::writeError(result["error"].toString(), "ImageStorage::createAttributeName()");
        return result;
    }

    if (!baseSelectReply.next())
    {
        if (list.length() > 1)
            query = "INSERT INTO AttributeName (Name, DataType) VALUES ('" + list[0] + "' ," + getDataTypeId(attributeDataType::Null) + ")";
        else
            query = "INSERT INTO AttributeName (Name, DataType) VALUES ('" + list[0] + "' ," + getDataTypeId(dataType) + ")";
        reply = exec(query);
        if (reply.lastError().isValid())
        {
            result["error"] = "Error in query on adding root level tag. query:" + query;
            KTools::Log::writeError(result["error"].toString(), "ImageStorage::createAttributeName()");
            return result;
        }
        parentNowCreated = true;
        query = "SELECT * FROM AttributeName WHERE (Name = '" + list[0] + "' AND ParentAttributeId IS NULL";
        baseSelectReply = exec(query);
    }

    int parentId = -1;
    for (int i = 1; i < list.length(); i++)
    {
        parentId = baseSelectReply.value(0).toInt();
        query = "SELECT * FROM AttributeName WHERE (Name = '" + list[i] + "' AND ParentAttributeId = " + QString::number(parentId) + ")";
        baseSelectReply = exec(query);
        if (parentNowCreated || !baseSelectReply.next())
        {
            if (list.length() > (i + 1))
                query = "INSERT INTO AttributeName (Name, ParentAttributeId, DataType) VALUES ('" + list[0] + "' , " + QString::number(parentId) + "," + getDataTypeId(attributeDataType::Null) + ")";
            else
                query = "INSERT INTO AttributeName (Name, ParentAttributeId, DataType) VALUES ('" + list[0] + "' , " + QString::number(parentId) + "," + getDataTypeId(dataType) + ")";
            reply = exec(query);
            if (reply.lastError().isValid())
            {
                result["error"] = "Error in query on adding " + QString::number(i + 1) + " level tag. query:" + query;
                KTools::Log::writeError(result["error"].toString(), "ImageStorage::createAttributeName()");
                return result;
            }
            parentNowCreated = true;
            query = "SELECT * FROM AttributeName WHERE (Name = '" + list[i] + "' AND ParentAttributeId = " + QString::number(parentId) + ")";
            baseSelectReply = exec(query);
        }
        else
        {
            if (list.length() == (i + 1) && baseSelectReply.value(2) != getDataTypeId(dataType))
            {
                if (baseSelectReply.value(2) == getDataTypeId(attributeDataType::Null))
                {
                    query = "UPDATE AttributeName SET DataType = " + getDataTypeId(dataType) + " WHERE Id = " + reply.value(0).toString();
                    reply = exec(query);
                    if (reply.lastError().isValid())
                    {
                        result["error"] = "Error on DataType updating. query:" + query;
                        KTools::Log::writeError(result["error"].toString(), "ImageStorage::createAttributeName()");
                        return result;
                    }
                }
                else
                {
                    result["error"] = "Attribute exist but passed wrong data type in function. name: " + name;
                    KTools::Log::writeError(result["error"].toString(), "ImageStorage::createAttributeName()");
                    return result;
                }
            }
        }
        parentId = baseSelectReply.value(0).toInt();
    }
    result["error"] = "";
    result["nameId"] = parentId;
    return result;
}

QVariantMap ImageStorage::findTagByName(const QString name, const int level)
{

}

QSqlQuery ImageStorage::execWithBind(const QByteArray &query, const QString &bindedValue, const QSql::ParamType bindFlags)
{

}
