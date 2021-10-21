#include "pixmapimage.h"

PixmapImage::PixmapImage(QQuickItem *parent) : QQuickPaintedItem(parent) {}

void PixmapImage::setImage(QObject *pixContainer)
{
    PixmapContainer *container = qobject_cast<PixmapContainer*>(pixContainer);
    mPixmapContainer.pixmap = container->pixmap;
    update();
}

void PixmapImage::paint(QPainter *painter)
{
    setHeight(mPixmapContainer.pixmap.height());
    setWidth(mPixmapContainer.pixmap.width());
    painter->drawPixmap(0, 0, mPixmapContainer.pixmap.width(), mPixmapContainer.pixmap.height(), mPixmapContainer.pixmap);
}
