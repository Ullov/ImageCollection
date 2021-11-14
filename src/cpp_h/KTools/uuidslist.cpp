#include "uuidslist.h"

QVector<QByteArray> UuidsList::data;
QVector<bool> UuidsList::dataBool;
QVariant UuidsList::ret;
QVariant UuidsList::varArg;
UuidsList UuidsList::ids;

UuidsList::UuidsList() {}

UuidsList::~UuidsList() {}

QVariant UuidsList::getAndMark()
{
    QVariant retVal = "All ids used up";
    for (int i = 0; i < data.length(); i++)
    {
        if (!dataBool[i])
        {
            dataBool[i] = true;
            retVal = data[i];
            break;
        }
    }
    return retVal;
}

void UuidsList::init(UuidsList *obj)
{
    QMetaObject::invokeMethod(obj, "getAndMark", Qt::AutoConnection, Q_RETURN_ARG(QVariant, ret));
    //QMetaObject::invokeMethod(obj, "unmark", Qt::AutoConnection, Q_ARG(QVariant, varArg));
}

void UuidsList::unmark(const QVariant str)
{
    for (int i = 0; i < data.length(); i++)
    {
        if (data[i] == str.toByteArray())
        {
            dataBool[i] = false;
            break;
        }
    }
}

void UuidsList::createItems(const int i)
{
    for (int li = 0; li < i; li++)
    {
        data.append(QUuid::createUuid().toByteArray());
        dataBool.append(false);
    }
}
