#ifndef PIXMAPIMAGE_H
#define PIXMAPIMAGE_H

#include <QQuickPaintedItem>
#include <QPainter>
#include "pixmapcontainer.h"

class PixmapImage : public QQuickPaintedItem
{
    Q_OBJECT
public:
    PixmapImage(QQuickItem *parent = Q_NULLPTR);
    Q_INVOKABLE void setImage(QObject *pixContainer);
    Q_INVOKABLE void setViewerHeight(const int dimension);
    Q_INVOKABLE void setViewerWidth(const int dimension);
    Q_INVOKABLE void setCanvasScale(const qreal number);

protected:
    virtual void paint(QPainter *painter) Q_DECL_OVERRIDE;

private:
    PixmapContainer mPixmapContainer;
    qreal scaleNumb = 1.0;
    bool isScaleNegative = false;
    int viewerWidth = 620;
    int viewerHeight = 480;
    qreal actualScale;
};

#endif // PIXMAPIMAGE_H
