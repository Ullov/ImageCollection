#include "archiverw.h"

ArchiveRW::ArchiveRW() {}

void ArchiveRW::extractArchive(const QString &pathToArchive, const QString &whereExtract)
{
    KZip archive(pathToArchive);
    archive.open(QIODevice::ReadOnly);
    const KArchiveDirectory *root = archive.directory();
    root->copyTo(whereExtract, true);
    archive.close();
}
