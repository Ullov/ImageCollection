#ifndef KTOOLS_KFF_VARIABLETYPES_H
#define KTOOLS_KFF_VARIABLETYPES_H

#include "rawstream.h"
#include "../converter.h"
#include "../log.h"
#include "pointer.h"

namespace KTools::Kff {
    class VariableTypes : private RawStream
    {
    public:
        VariableTypes(Manager *man);
        VariableTypes(Manager *man, const qint64 position);

        enum class Type : qint8 {
            String = 1,
            OccupiedCls = 2 // All clusters aside from first cluster in variable
        };

        QByteArray readString(const qint64 position);
        qint64 rewriteVariable(const QByteArray &data, const qint64 position);
        bool deleteVariable(const qint64 position);
        bool clearVariable(const qint64 position);
        qint64 writeVariable(const QByteArray &data, const qint64 position = -1);

    private:
        struct Sizes {
            static const qint64 type = 1;
            static const qint64 size = 8; // Skipped in Type::OccupiedCls
            static const qint64 nextCls = 8;
            static const qint64 data = 111;
            static const qint64 occupiedData = data + 8;
            static const qint64 all = 128;
        };

        QByteArray readVariable(const qint64 position, const Type type);
    };
}

#endif // KTOOLS_KFF_VARIABLETYPES_H
