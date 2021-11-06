#ifndef IMAGESTORAGE_H
#define IMAGESTORAGE_H

#include <QSqlDatabase>
#include <QSqlQuery>
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
    enum getTagParams {
        All,
        Id,
    };
    QSqlQuery exec(const QString &query);
    QSqlQuery execWithBind(const QByteArray &query, const QString &bindedValue, const QSql::ParamType bindFlags);
    QList<QSqlQuery> getTag(const QString &tags, const getTagParams &param = All);
    bool openDb();

    QSqlDatabase data;
};

#endif // IMAGESTORAGE_H
