#ifndef KTOOLS_KFF_DIRECTORY_H
#define KTOOLS_KFF_DIRECTORY_H

#include "rawstream.h"
#include "../converter.h"

namespace KTools::Kff {
    class FixedTypes : private RawStream
    {
    public:
        FixedTypes(Manager *man);

        enum class Type : quint8 {
            Int8 = 1,
            Int16 = 2,
            Int32 = 3,
            Int64 = 4,
            UInt8 = 5,
            UInt16 = 6,
            UInt32 = 7,
            UInt64 = 8,
            Float = 9,
            Bool = 10,
            Pointer = 11,
            Void8 = 12,
            Void16 = 13,
            Void32 = 14,
            Void64 = 15,
            Void65 = 16
        };

    private:
        struct Size {
        private:
            static const qint8 Int8 = 1;
            static const qint8 Int16 = 2;
            static const qint8 Int32 = 4;
            static const qint8 Int64 = 8;
            static const qint8 UInt8 = 1;
            static const qint8 UInt16 = 2;
            static const qint8 UInt32 = 4;
            static const qint8 UInt64 = 8;
            static const qint8 Float = 8;
            static const qint8 Bool = 1;
            static const qint8 Pointer = 9;
            static const qint8 Void8 = 1;
            static const qint8 Void16 = 2;
            static const qint8 Void32 = 4;
            static const qint8 Void64 = 8;
            static const qint8 Void65 = 9;
        public:
            static constexpr qint8 get(const Type type)
            {
                switch (type)
                {
                    case Type::Int8 : return Int8;
                    case Type::Int16 : return Int16;
                    case Type::Int32 : return Int32;
                    case Type::Int64 : return Int64;
                    case Type::UInt8 : return UInt8;
                    case Type::UInt16 : return UInt16;
                    case Type::UInt32 : return UInt32;
                    case Type::UInt64 : return UInt64;
                    case Type::Float : return Float;
                    case Type::Bool : return Bool;
                    case Type::Pointer : return Pointer;
                    case Type::Void8 : return Void8;
                    case Type::Void16 : return Void16;
                    case Type::Void32 : return Void32;
                    case Type::Void64 : return Void64;
                    case Type::Void65 : return Void65;
                }
                return 0; // To silence warning
            }
            static constexpr qint8 getVoidTypeNumb(const Type type)
            {
                qint8 typeSize = get(type);
                switch (typeSize)
                {
                    case 1 : return static_cast<qint8>(Type::Void8);
                    case 2 : return static_cast<qint8>(Type::Void16);
                    case 4 : return static_cast<qint8>(Type::Void32);
                    case 8 : return static_cast<qint8>(Type::Void64);
                    case 9 : return static_cast<qint8>(Type::Void65);
                }
                return 0; // To silence warning
            }
        };

    public:
        template <typename T>
        qint64 add(const T data, const Type type = Type::Int64);

        template <typename T>
        T get(const qint64 position);

        template <typename T>
        bool change(const T data, const qint64 position);

        bool remove(const qint64 position);
    };
}

#endif // KTOOLS_KFF_DIRECTORY_H
