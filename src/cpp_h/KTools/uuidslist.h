#ifndef UUIDSLIST_H
#define UUIDSLIST_H

#include <QByteArray>
#include <QVector>
#include <QObject>
#include <QVariant>
#include <QUuid>

class UuidsList : public QObject
{
    Q_OBJECT
public:
    UuidsList();
    ~UuidsList();

    Q_INVOKABLE QVariant getAndMark();
    Q_INVOKABLE void unmark(const QVariant i);
    static void createItems(const int i);
    static void init(UuidsList *obj);

    static QVariant ret;
    static QVariant varArg;

private:
    static QVector<QByteArray> data;
    static QVector<bool> dataBool;
};

#endif // UUIDSLIST_H
