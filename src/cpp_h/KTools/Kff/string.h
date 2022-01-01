#ifndef KTOOLS_KFF_STRING_H
#define KTOOLS_KFF_STRING_H

#include <QByteArray>

namespace KTools::Kff {
    class Pointer;
    class Manager;
    class String
    {
    public:
        String(Manager *man);
        String(Manager *man, const qint64 pos);
        String(const Pointer *poi);
        ~String();

        QByteArray mid(const qint64 pos, const qint64 len = -1);
        QByteArray readAll();
        void insert(const qint64 pos, const QByteArray content);
        char at(const qint64 pos);

        const QByteRef operator[](const uint pos);
        QByteArray& operator=(const QByteArray &other);
        bool operator==(const QByteArray &other);
        bool operator==(const String &other);
        QByteArray& operator+=(const QByteArray &other);


    private:
        void flush();

        Pointer *pointer;
        QByteArray data;
    };
}

#endif // KTOOLS_KFF_STRING_H
