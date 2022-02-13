#include "fshandler.h"

#include "KTools/src/cpp_h/log.h"

KTools::Options *FsHandler::optionsObj;

extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;
//qt_ntfs_permission_lookup++;

FsHandler::FsHandler()
{
    currentDirs = new QMap<QVariant, QString>();
    //qt_ntfs_permission_lookup++;
}

QJsonObject FsHandler::fileInfoToJsonObject(const QFileInfo &file)
{
    QJsonObject result;
    result["whenModified"] = file.lastModified().toString("yyyy.MM.dd hh:ss:mm:zzz");
    result["whenReaded"] = file.lastRead().toString("yyyy.MM.dd hh:ss:mm:zzz");
    result["whenMetadataChanged"] = file.metadataChangeTime().toString("yyyy.MM.dd hh:ss:mm:zzz");
    result["whenCreated"] = file.birthTime().toString("yyyy.MM.dd hh:ss:mm:zzz");
    result["group"] = file.group();
    result["groupId"] = QString::number(file.groupId());
    result["owner"] = file.owner();
    result["ownerId"] = QString::number(file.ownerId());
    result["isHidden"] = file.isHidden();
    result["isJunction"] = file.isJunction();
    result["isReadable"] = file.isReadable();
    result["isRoot"] = file.isRoot();
    result["isShortcut"] = file.isShortcut();
    result["isSymLink"] = file.isSymLink();
    result["isWritable"] = file.isWritable();
    result["symLinkTarget"] = file.symLinkTarget();
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
    optionsObj->updateParam("FSExplorer:LastPath", KTools::Converter::convert<QString, QByteArray>(dir.path()), KTools::Options::ParamType::String);
    currentDirs->operator[](uuid) = dir.absolutePath();
    return res;
}

void FsHandler::init(const QVariant uuid)
{
    QDir tmpDir = getDir(optionsObj->getParam<QString>("FSExplorer:LastPath"));
    currentDirs->insert(uuid, tmpDir.absolutePath());
    asyncSendDirInfo(tmpDir.entryInfoList(), uuid);
}

void FsHandler::slotCd(const QString file, const QVariant uuid)
{
    QDir tmpDir = getDir(currentDirs->value(uuid));
    if (cd(tmpDir, file, uuid))
        asyncSendDirInfo(tmpDir.entryInfoList(), uuid);
    else
        KTools::Log::writeError("Directory does not exist. file: " + file, "FsHandler::slotSd()");
}

void FsHandler::slotOpenInDefaultApp(const QString path, const QString fileType, const QByteArray uuid)
{
    if ((fileType == "jpg") || (fileType == "gif") || (fileType == "png") || (fileType == "tiff") || (fileType == "jpeg") || (fileType == "webp"))
        emit openImage(path);
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

void FsHandler::asyncSendDirInfo(const QFileInfoList files, const QVariant uuid)
{
    while (thr.isRunning())
        itsTimeToStopIt = true;

    itsTimeToStopIt = false;
    thr = QtConcurrent::run([=]()
    {
        bool run = true;
        int i = 0;
        QFileInfoList list;
        while (run && !itsTimeToStopIt)
        {
            int startPos = (100 * i) - 1;
            int len = files.length() - ((100 * i) - 1);
            if (len < 100)
            {
                list = files.mid(startPos, len);
                run = false;
            }
            else
                list = files.mid(startPos, 100);

            QJsonObject tmp = fileInfoListToJsonObject(list, uuid);
            if (i == 0)
                tmp["refresh"] = true;
            else
                tmp["refresh"] = false;

            emit dirInfo(tmp, uuid);
            i++;
        }
    });
}
