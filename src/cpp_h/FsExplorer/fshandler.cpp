#include "fshandler.h"

#include "../KTools/log.h"

FsHandler::FsHandler()
{
    currentDirs = new QMap<QVariant, QString>();
}

QJsonObject FsHandler::fileInfoToJsonObject(const QFileInfo &file)
{
    QJsonObject result;
    result["whenModified"] = file.lastModified().toString("yyyy.MM.dd hh:ss:mm:zzz");
    result["whenReaded"] = file.lastRead().toString("yyyy.MM.dd hh:ss:mm:zzz");
    result["whenMetadataChanged"] = file.metadataChangeTime().toString("yyyy.MM.dd hh:ss:mm:zzz");
    result["owner"] = file.owner();
    result["ownerId"] = QString::number(file.ownerId());
    result["size"] = file.size();
    result["extension"] = file.suffix();
    result["path"] = file.absolutePath();
    result["filePath"] = file.filePath();
    result["fileName"] = file.fileName();
    if (file.isDir())
    {
        result["isDir"] = true;
        result["iconPath"] = "qrc:/resources/FSExplorer/img/fileTypeIcons/folder.png";
    }
    else
    {
        result["isDir"] = false;
        QStringList strList = KTools::FileIdentifier::identifyFileFromFileInfo(file);
        result["iconPath"] = strList[2];
        result["fileType"] = strList[0];
    }
    return result;
}

QJsonObject FsHandler::fileInfoListToJsonObject(const QFileInfoList &files, const QVariant &uuid)
{
    QJsonObject tmp;
    QJsonArray fResult;
    QJsonArray dResult;
    for (int i = 0; i < files.length(); i++)
    {
        tmp = fileInfoToJsonObject(files[i]);
        if (tmp["isDir"] == true)
            dResult.push_back(tmp);
        else
            fResult.push_back(tmp);
    }
    tmp = QJsonObject();
    tmp["dir"] = dResult;
    tmp["file"] = fResult;
    tmp["currentDir"] = getDir(currentDirs->value(uuid)).path();
    return tmp;
}

bool FsHandler::cd(QDir &dir, const QString &file, const QVariant &uuid)
{
    bool res = dir.cd(file);
    KTools::Options::setParam("/fsExplorer/lastOpenedDirectory", dir.path());
    currentDirs->operator[](uuid) = dir.absolutePath();
    return res;
}

void FsHandler::init(const QVariant uuid)
{
    QDir tmpDir = getDir(KTools::Options::getParam("/fsExplorer/lastOpenedDirectory").toString());
    currentDirs->insert(uuid, tmpDir.absolutePath());
    emit dirInfo(fileInfoListToJsonObject(tmpDir.entryInfoList(), uuid), uuid);
}

void FsHandler::slotCd(const QString file, const QVariant uuid)
{
    QDir tmpDir = getDir(currentDirs->value(uuid));
    if (cd(tmpDir, file, uuid))
        emit dirInfo(fileInfoListToJsonObject(tmpDir.entryInfoList(), uuid), uuid);
    else
        KTools::Log::writeError("Directory does not exist. file: " + file, "FsHandler::slotSd()");
}

void FsHandler::slotOpenInDefaultApp(const QString path, const QString fileType, const QByteArray uuid)
{
    if ((fileType == ".jpg") || (fileType == ".png") || (fileType == ".gif") || (fileType == ".png") || (fileType == ".tiff") || (fileType == ".jpeg") || (fileType == ".webp"))
    {
        PixmapContainer *pix = new PixmapContainer();
        pix->pixmap.load(path);
        QQmlEngine::setObjectOwnership(pix, QQmlEngine::JavaScriptOwnership);
        emit openImage(pix);
    }
    else
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void FsHandler::slotCdUp(const QVariant uuid)
{
    QDir tmpDir = getDir(currentDirs->value(uuid));
    if (cd(tmpDir, "..", uuid))
        emit dirInfo(fileInfoListToJsonObject(tmpDir.entryInfoList(), uuid), uuid);
}

void FsHandler::slotShowDrivesList(const QVariant uuid)
{
    emit drivesList(fileInfoListToJsonObject(getDir(currentDirs->value(uuid)).drives(), uuid), uuid);
}

void FsHandler::slotRemoveFile(const QVariantList arr, const QVariant uuid)
{
    for (int i = 0; i < arr.size(); i++)
    {
        if (KTools::File::isFile(arr[i].toString()))
            QFile(arr[i].toString()).remove();
        else
            QDir(arr[i].toString()).removeRecursively();
    }
    QDir tmpDir = getDir(currentDirs->value(uuid));
    tmpDir.refresh();
    emit dirInfo(fileInfoListToJsonObject(tmpDir.entryInfoList(), uuid), uuid);
}

QDir FsHandler::getDir(const QString &path)
{
    QDir newDir(path);

    for (int i = 0; !newDir.exists(); i++)
    {
        newDir.cdUp();
        if (i > 30)
        {
            newDir.setPath("C:/");
            break;
        }
    }
    newDir.setFilter(QDir::AllDirs | QDir::AllEntries | QDir::Writable | QDir::Executable | QDir::Readable | QDir::Hidden | QDir::System | QDir::NoDot);
    return newDir;
}
