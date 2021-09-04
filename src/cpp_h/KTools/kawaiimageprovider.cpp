#include "kawaiimageprovider.h"

//KawaiImageProvider::KawaiImageProvider() {}

QPixmap KawaiImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    QStringList list = id.split("/", Qt::SkipEmptyParts);
    int width = list[0].toInt();
    int height = list[1].toInt();
    if (size)
        *size = QSize(width, height);
    QPixmap pixmap(width, height);
    //pixmap
    return pixmap;
}
