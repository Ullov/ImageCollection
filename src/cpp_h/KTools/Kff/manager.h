#ifndef KTOOLS_KFF_MANAGER_H
#define KTOOLS_KFF_MANAGER_H

#include "../file.h"

namespace KTools::Kff {
    class RawStream;
    class FixedTypes;
    class VariableTypes;
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

        RawStream* getDefaultStream();
        qint64 allocCluster();
        void freeCluster(const qint64 cls);
        void writeInode(const qint64 clust);
        FixedTypes* getNumbers();
        VariableTypes* getStrings();
        QByteArray makePointer(const PointerType type, const qint64 position);
        void addClusterPos(const qint64 position);
        RawStream getNewStream();
        QByteArray getDataFromPointer(const QByteArray &pointer);

        KTools::File file;

    protected:
        OpenMode mode;
        FixedTypes *numbers;
        VariableTypes *strs;
        RawStream *defaultStream;

    private:
        void constructFs();

        static constexpr char signature[] = "KFFS0000";

        struct Sizes {
            static const qint64 inode = 8;
            static const qint64 cluster = 4096;
            static constexpr qint64 signature = sizeof(signature) - 1;
            static const qint64 allInodes = 160;
        };
        struct Offsets {
            const qint64 inodes = Sizes::signature;
            const qint64 data = inodes + Sizes::allInodes;
        };

        Sizes sizes;
        Offsets offsets;

        QList<QPair<qint64, bool>> clusters;
    };
}

#endif // KTOOLS_KFF_MANAGER_H
