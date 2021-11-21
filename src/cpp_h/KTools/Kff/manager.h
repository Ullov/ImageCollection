#ifndef KTOOLS_KFF_MANAGER_H
#define KTOOLS_KFF_MANAGER_H

#include "../file.h"
#include "rawstream.h"
#include "fixedtypes.h"
#include "variabletypes.h"

namespace KTools::Kff {
    class Manager
    {
    public:
        enum class OpenMode {
            Clear,
            Keep
        };
        enum class PointerType : quint8 {
            FixedTypes = 1,
            VariableTypes = 2,
            File = 3
        };

        ~Manager();
        Manager(const QString &path, const OpenMode lMode);

        RawStream getStream();
        qint64 allocCluster();
        void freeCluster(const qint64 cls);
        void writeInode(const qint64 clust, const qint64 size = -1);
        FixedTypes* getNumbers();
        VariableTypes* getStrings();
        QByteArray makePointer(const PointerType type, const qint64 position);

        KTools::File file;

    private:
        void constructFs();

        static constexpr char signature[] = "KFFS0000";

        struct Offsets {
            const qint64 inodes = sizeof(signature) - 1;
            qint64 data = 0;
        };
        struct Sizes {
            static const qint64 inode = 16;
            static const qint64 cluster = 4096;
            static const qint64 signature = sizeof(signature) - 1;
        };

        Sizes sizes;
        Offsets offsets;

        OpenMode mode;
        QList<QPair<qint64, bool>> clusters;
        FixedTypes *numbers;
        VariableTypes *strs;
    };
}

#endif // KTOOLS_KFF_MANAGER_H
