#ifndef IMAGESTORAGE_H
#define IMAGESTORAGE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDateTime>
#include "../KTools/log.h"

class ImageStorage : public QObject
{
    Q_OBJECT
public:
    ImageStorage();
    ImageStorage(QSqlDatabase &db);

    Q_INVOKABLE QString addTag(const QVariantMap info);
    Q_INVOKABLE QVariantList getAllTags();

private:
    enum class attributeAssociation {
        Image,
        Tag,
        Gallery
    };
    enum class attributeDataType {
        Int,
        Real,
        Text,
        Bool,
        Blob,
        List,
        Null // Attribute with this data type must change data type before writing
    };
    enum class getAttributesChainParam {
        All,
        Parents
    };

    QSqlQuery exec(const QString &query);
    QSqlQuery execWithBind(const QByteArray &query, const QString &bindedValue, const QSql::ParamType bindFlags);
    QList<QSqlQuery> getTag(const QString &tags);
    bool openDb();
    QVariantMap writeAttribute(const QString &name, const QByteArray &value, const attributeDataType dataType, const attributeAssociation association, const int associateId);
    QString getDataTypeId(const attributeDataType type);
    QList<QSqlQuery> getAttributesChain(const QString &attributes, const getAttributesChainParam param = getAttributesChainParam::All);
    QVariantMap createAttributeName(const QString &name, const attributeDataType dataType);
    QVariantMap findTagByName(const QString name, const int level);

    QSqlDatabase data;
};

#endif // IMAGESTORAGE_H
