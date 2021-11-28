#ifndef FSHANDLER_H
#define FSHANDLER_H

#include <QObject>
#include <QFileInfoList>
#include <QJsonObject>
#include <QDateTime>
#include <QJsonArray>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#include <QVariant>
#include <QPixmap>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include "../KTools/fileidentifier.h"
#include "../KTools/options.h"

class FsHandler : public QObject
{
    Q_OBJECT
public:
    FsHandler();
    static KTools::Options *optionsObj;

public slots:
    void init(const QVariant uuid);
    void slotCd(const QString file, const QVariant uuid);
    void slotOpenInDefaultApp(const QString path, const QString fileType, const QByteArray uuid);
    void slotCdUp(const QVariant uuid);
    void slotShowDrivesList(const QVariant uuid);
    void slotRemoveFile(const QVariantList arr, const QVariant uuid);

signals:
    void dirInfo(const QJsonObject dirInfo, const QVariant uuid);
    void drivesList(const QJsonObject drives, const QVariant uuid);
    void openImage(QString path);

private:
    QJsonObject fileInfoToJsonObject(const QFileInfo &file);
    QJsonObject fileInfoListToJsonObject(const QFileInfoList &files, const QVariant &uuid);
    bool cd(QDir &dir, const QString &file, const QVariant &uuid);
    QDir getDir(const QString &path);
    void asyncSendDirInfo(const QFileInfoList files, const QVariant uuid);

    // variables
    QMap<QVariant, QString> *currentDirs;
    QFuture<void> thr;
    bool itsTimeToStopIt = false;
};

#endif // FSHANDLER_H
