#ifndef IMAGESTORAGE_INTERFACE_H
#define IMAGESTORAGE_INTERFACE_H

#include <QVariantMap>
#include "../KTools/Kff/nameinfo.h"

namespace KTools::Kff {
    class MetainfoFs;
}

namespace ImageStorage {
    class Interface : public QObject
    {
        Q_OBJECT
    public:
        Interface(KTools::Kff::MetainfoFs *ldb);

        Q_INVOKABLE void addTag(const QByteArray name, const QByteArray description);
        Q_INVOKABLE void addAttribute(const QByteArray name, const QByteArray description, const KTools::Kff::DataType dataType);
        Q_INVOKABLE QVariantList getNames();

    public slots:
        void addPhoto(const QByteArray path, const QVariantMap attributes, const QVariantList tags, const QByteArray convertTo);

    private:
        KTools::Kff::MetainfoFs *db;
    };
}
#endif // IMAGESTORAGE_INTERFACE_H
