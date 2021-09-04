#include "fshandler.h"

#include "../KTools/log.h"

FsHandler::FsHandler()
{
    currentDir = new QDir(KTools::Options::getParam("/fsExplorer/lastOpenedDirectory").toString());

    for (int i = 0; !currentDir->exists(); i++)
    {
        currentDir->cdUp();
        if (i > 30)
        {
            currentDir->setPath("C:/");
            break;
        }
    }
    currentDir->setFilter(QDir::AllDirs | QDir::AllEntries | QDir::Writable | QDir::Executable | QDir::Readable | QDir::Hidden | QDir::System | QDir::NoDot);
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

QJsonObject FsHandler::fileInfoListToJsonObject(const QFileInfoList &files)
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
    tmp["currentDir"] = currentDir->path();
    return tmp;
}

bool FsHandler::cd(const QString &dir)
{
    bool res = currentDir->cd(dir);
    KTools::Options::setParam("/fsExplorer/lastOpenedDirectory", currentDir->path());
    return res;
}

bool FsHandler::setPath(const QString &dir)
{
    QString oldPath = currentDir->path();
    currentDir->setPath(dir);
    if (currentDir->exists())
        return true;
    else
    {
        currentDir->setPath(oldPath);
        return false;
    }
}

void FsHandler::init()
{
    emit dirInfo(fileInfoListToJsonObject(currentDir->entryInfoList()));
}

void FsHandler::slotCd(const QString file)
{
    if (cd(file))
        emit dirInfo(fileInfoListToJsonObject(currentDir->entryInfoList()));
    else
        KTools::Log::writeError("Directory does not exist. file: " + file, "FsHandler::slotSd()");
}

void FsHandler::slotOpenInDefaultApp(const QString path)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void FsHandler::slotCdUp()
{
    if (cd(".."))
        emit dirInfo(fileInfoListToJsonObject(currentDir->entryInfoList()));
}

void FsHandler::slotShowDrivesList()
{
    emit drivesList(fileInfoListToJsonObject(currentDir->drives()));
}

void FsHandler::slotRemoveFile(const QVariantList arr)
{
    for (int i = 0; i < arr.size(); i++)
        QDir(arr[i].toString()).removeRecursively();
    emit dirInfo(fileInfoListToJsonObject(currentDir->entryInfoList()));
}
