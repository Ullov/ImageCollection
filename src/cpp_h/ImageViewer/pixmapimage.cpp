#include "pixmapimage.h"

PixmapImage::PixmapImage(QQuickItem *parent) : QQuickPaintedItem(parent) {}

void PixmapImage::setImage(QString path)
{
    setAntialiasing(true);

    QFileInfo currPicInfo = QFileInfo(path);
    pathToFolder = currPicInfo.absolutePath();
    QString currPicName = currPicInfo.fileName();
    QDir picDir = currPicInfo.absoluteDir();
    QStringList filters;
    filters << "*.jpg" << "*.jpeg" << "*.png" << "*.webp" << "*.tiff" << "*.gif";
    picDir.setNameFilters(filters);
    picsInCurrDir = picDir.entryList();
    currPicId = picsInCurrDir.indexOf(currPicName);

    loadImage();
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

    setHeight(mPixmap.height() * actualScale);
    setWidth(mPixmap.width() * actualScale);

    painter->setWindow(0, 0, mPixmap.width(), mPixmap.height());
    painter->drawPixmap(0, 0, mPixmap.width(), mPixmap.height(), mPixmap);
}

void PixmapImage::setViewerDimensions(const int height, const int width)
{
    viewerHeight = height;
    viewerWidth = width;
}

void PixmapImage::setCanvasScale(const qreal number)
{
    if (scaleNumb + number >= 1)
        scaleNumb += number / 20;
    else
        scaleNumb += number;
    update();
}

void PixmapImage::nextPicture()
{
    if (currPicId + 1 < picsInCurrDir.length())
        currPicId++;

    loadImage();
}

void PixmapImage::previousPicture()
{
    if (currPicId > 0)
        currPicId--;

    loadImage();
}

void PixmapImage::loadImage()
{
    QPixmap pix = QPixmap();
    pix.load(pathToFolder + "/" + picsInCurrDir[currPicId]);
    mPixmap = pix;

    if (viewerHeight < mPixmap.height())
    {
        scaleNumb = (viewerHeight - mPixmap.height()) / (mPixmap.height() / 100.0f);
    }
    else if (viewerWidth < mPixmap.width())
    {
        scaleNumb = (viewerWidth - mPixmap.width()) / (mPixmap.width() / 100.0f);
    }
    else
    {
        scaleNumb = 1;
    }
    update();
}
