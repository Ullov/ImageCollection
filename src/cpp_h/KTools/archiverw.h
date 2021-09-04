#ifndef ARCHIVERW_H
#define ARCHIVERW_H

#include <KF5/KArchive/kzip.h>

class ArchiveRW
{
public:
    ArchiveRW();

    static void extractArchive(const QString &pathToArchive, const QString &whereExtract);

};

#endif // ARCHIVERW_H
