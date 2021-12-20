#ifndef KTOOLS_KFF_POINTER_H
#define KTOOLS_KFF_POINTER_H

#include <QByteArray>

namespace KTools::Kff {
    class Manager;
    class Pointer
    {
    public:
        enum class PointerType : quint8 {
            Null = 0,
            FixedTypes = 1,
            VariableTypes = 2,
            File = 3
        };

        Pointer();
        Pointer(Manager *man, const QByteArray &content);
        Pointer(Manager *man, const PointerType type, const qint64 position);

        static QByteArray makePointer(const PointerType type, const qint64 position);
        qint64 getPosition();
        PointerType getType();
        QByteArray getAll() const;
        void setType(const PointerType type);
        void setPos(const qint64 position);
        void setAll(const PointerType type, const qint64 position);
        void setAll(const QByteArray &content);
        /*void writeData(const QByteArray &content);
        void writeData(const QList<Pointer> &content);
        void writeData(const QByteArray &content, const FixedTypes::Type type);*/

        template <typename T>
        bool writeData(const T &content);
        template <typename T>
        T getData();

        bool isNull = false;

    private:
        QByteArray data;
        Manager *manager;

        bool comparePointerType(const PointerType first, const quint8 second);
    };
}

#endif // KTOOLS_KFF_POINTER_H
