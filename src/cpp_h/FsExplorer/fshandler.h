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
    void init();
    void slotCd(const QString file);
    void slotOpenInDefaultApp(const QString path);
    void slotCdUp();
    void slotShowDrivesList();
    void slotRemoveFile(const QVariantList arr);

signals:
    void dirInfo(const QJsonObject dirInfo);
    void drivesList(const QJsonObject drives);

private:
    QJsonObject fileInfoToJsonObject(const QFileInfo &file);
    QJsonObject fileInfoListToJsonObject(const QFileInfoList &files);
    bool cd(const QString &dir);
    bool setPath(const QString &dir);


    // variabnles
    QDir *currentDir;
};

#endif // FSHANDLER_H
