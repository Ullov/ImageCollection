#ifndef KTOOLS_KFF_LIST_H
#define KTOOLS_KFF_LIST_H

#include "string.h"
#include <QList>

namespace KTools::Kff {
    template<typename T>
    class List : private String
    {
    public:
        List(Manager *man);
        List(Manager *man, const qint64 pos);
        List(Pointer *poi);
        ~List();

        void remove(const qint64 i);
        void safeRemove(const qint64 i);
        qint64 size();

        T& operator[](const qint64 i);
        List<T>& operator+=(const T other);

    private:
        QList<T> list;
    };
}

#endif // KTOOLS_KFF_LIST_H
