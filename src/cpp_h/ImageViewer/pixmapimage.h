#ifndef PIXMAPIMAGE_H
#define PIXMAPIMAGE_H

#include <QQuickPaintedItem>
#include <QPainter>
#include <QDir>

class PixmapImage : public QQuickPaintedItem
{
    Q_OBJECT
public:
    PixmapImage(QQuickItem *parent = Q_NULLPTR);
    Q_INVOKABLE void setImage(QString path);
    Q_INVOKABLE void setViewerDimensions(const int height, const int width);
    Q_INVOKABLE void setCanvasScale(const qreal number);
    Q_INVOKABLE void nextPicture();
    Q_INVOKABLE void previousPicture();

protected:
    virtual void paint(QPainter *painter) Q_DECL_OVERRIDE;

private:
    void loadImage();

    QPixmap mPixmap;
    qreal scaleNumb = 1.0;
    bool isScaleNegative = false;
    int viewerWidth = 620;
    int viewerHeight = 480;
    qreal actualScale;
    QStringList picsInCurrDir;
    int currPicId;
    QString pathToFolder;
};

#endif // PIXMAPIMAGE_H
