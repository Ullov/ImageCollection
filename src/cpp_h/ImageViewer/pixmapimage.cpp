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
    // If scale number bigger than 1 (picture same size or smaller) scale coefficent equal scaleNumb.
    // But if smaller than 1 (picture smaller size) scale coefficent will be somewhere beetween 1 and 0
    // i.e. 0 < x < 1. This because scale coefficent qual to 0 gives picture with dimensions 0x0 and
    // coefficent smaller than 0 gives picture with negative dimensions. For pirpose of right scaling
    // implemented simplified linear interpolation:
    // x = (t - 0.1) - ((n * -1) / 100)
    // Or more simple form of this formula:
    // x = t - (n / 100)
    // Here:
    //
    // (t - 0.1): t it's top of the iterval in my case equal to 1. -0.1 need because in some cases without
    // it formula can give 0. Without this part formula will be giving numbers from the other end of interval.
    //
    // (n * -1): n it's number that i want convert to some number in interval. -1 need because my n negative.
    // Yes, i can aftervards * formula result on -1 but computer work slower with negative nubers.
    //
    // If give to formula n bigger than 1 it will give x bigger than 1.
    actualScale = scaleNumb;
    if (scaleNumb < 1.0f)
    {
        actualScale = 0.99f - ((scaleNumb * -1.0f) / 100.0f);
    }
    else if (scaleNumb == 0.0f)
    {
        actualScale = 0.1f;
    }
    else if (scaleNumb < -100.0f)
    {
        scaleNumb = -100.0f;
        actualScale = 0.99f - ((scaleNumb * -1.0f) / 100.0f);
    }

    setHeight(mPixmapContainer.pixmap.height() * actualScale);
    setWidth(mPixmapContainer.pixmap.width() * actualScale);

    painter->setWindow(0, 0, mPixmapContainer.pixmap.width(), mPixmapContainer.pixmap.height());
    painter->drawPixmap(0, 0, mPixmapContainer.pixmap.width(), mPixmapContainer.pixmap.height(), mPixmapContainer.pixmap);
}

void PixmapImage::setViewerHeight(const int dimension)
{
    viewerHeight = dimension;
}

void PixmapImage::setViewerWidth(const int dimension)
{
    viewerWidth = dimension;
}

void PixmapImage::setCanvasScale(const qreal number)
{
    if (scaleNumb + number >= 1)
        scaleNumb += number / 20;
    else
        scaleNumb += number;
    update();
}
