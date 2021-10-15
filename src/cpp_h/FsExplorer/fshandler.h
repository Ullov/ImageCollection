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
#include "../KTools/fileidentifier.h"
#include "../KTools/options.h"

class FsHandler : public QObject
{
    Q_OBJECT
public:
    FsHandler();

public slots:
    void init(const QVariant uuid);
    void slotCd(const QString file, const QVariant uuid);
    void slotOpenInDefaultApp(const QString path);
    void slotCdUp(const QVariant uuid);
    void slotShowDrivesList(const QVariant uuid);
    void slotRemoveFile(const QVariantList arr, const QVariant uuid);

signals:
    void dirInfo(const QJsonObject dirInfo, const QVariant uuid);
    void drivesList(const QJsonObject drives, const QVariant uuid);

private:
    QJsonObject fileInfoToJsonObject(const QFileInfo &file);
    QJsonObject fileInfoListToJsonObject(const QFileInfoList &files, const QVariant &uuid);
    bool cd(const QString &dir, const QVariant &uuid);
    QDir createDirectory(const QString &path);


    // variables
    QMap<QVariant, QDir*> *currentDirs;
};

#endif // FSHANDLER_H
