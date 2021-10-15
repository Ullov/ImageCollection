#include "fshandler.h"

#include "../KTools/log.h"

FsHandler::FsHandler()
{
    currentDirs = new QMap<QVariant, QDir*>();
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
        QStringList strList = KTools::FileIdentifier::identifyFileFromFileSystem(file.filePath());
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
    tmp["currentDir"] = currentDirs->value(uuid)->path();
    return tmp;
}

bool FsHandler::cd(const QString &dir, const QVariant &uuid)
{
    bool res = currentDirs->value(uuid)->cd(dir);
    KTools::Options::setParam("/fsExplorer/lastOpenedDirectory", currentDirs->value(uuid)->path());
    return res;
}

void FsHandler::init(const QVariant uuid)
{
    QDir *newDir = new QDir(KTools::Options::getParam("/fsExplorer/lastOpenedDirectory").toString());

    for (int i = 0; !newDir->exists(); i++)
    {
        newDir->cdUp();
        if (i > 30)
        {
            newDir->setPath("C:/");
            break;
        }
    }
    newDir->setFilter(QDir::AllDirs | QDir::AllEntries | QDir::Writable | QDir::Executable | QDir::Readable | QDir::Hidden | QDir::System | QDir::NoDot);
    currentDirs->insert(uuid, newDir);
    emit dirInfo(fileInfoListToJsonObject(newDir->entryInfoList(), uuid), uuid);
}

void FsHandler::slotCd(const QString file, const QVariant uuid)
{
    if (cd(file, uuid))
        emit dirInfo(fileInfoListToJsonObject(currentDirs->value(uuid)->entryInfoList(), uuid), uuid);
    else
        KTools::Log::writeError("Directory does not exist. file: " + file, "FsHandler::slotSd()");
}

void FsHandler::slotOpenInDefaultApp(const QString path)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void FsHandler::slotCdUp(const QVariant uuid)
{
    if (cd("..", uuid))
        emit dirInfo(fileInfoListToJsonObject(currentDirs->value(uuid)->entryInfoList(), uuid), uuid);
}

void FsHandler::slotShowDrivesList(const QVariant uuid)
{
    emit drivesList(fileInfoListToJsonObject(currentDirs->value(uuid)->drives(), uuid), uuid);
}

void FsHandler::slotRemoveFile(const QVariantList arr, const QVariant uuid)
{
    for (int i = 0; i < arr.size(); i++)
        QDir(arr[i].toString()).removeRecursively();
    emit dirInfo(fileInfoListToJsonObject(currentDirs->value(uuid)->entryInfoList(), uuid), uuid);
}
