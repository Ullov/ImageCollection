#ifndef IMAGESTORAGE_INTERFACE_H
#define IMAGESTORAGE_INTERFACE_H

#include <QVariantMap>
#include "nameinfo.h"

namespace ImageStorage {
    class Db;
    class Interface : public QObject
    {
        Q_OBJECT
    public:
        Interface(Db *ldb);

        Q_INVOKABLE void addTag(const QByteArray name, const QByteArray description);
        Q_INVOKABLE void addAttribute(const QByteArray name, const QByteArray description, const ImageStorage::DataType dataType);
        Q_INVOKABLE QVariantList getNames();

    public slots:
        void addPhoto(const QByteArray path, const QVariantMap attributes, const QVariantList tags, const QByteArray convertTo);

    private:
        Db *db;
    };
}
#endif // IMAGESTORAGE_INTERFACE_H
