#ifndef KAWAIIMAGEPROVIDER_H
#define KAWAIIMAGEPROVIDER_H

#include <QQuickImageProvider>

class KawaiImageProvider : public QQuickImageProvider
{
public:
    KawaiImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap) {}
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;
};

#endif // KAWAIIMAGEPROVIDER_H
