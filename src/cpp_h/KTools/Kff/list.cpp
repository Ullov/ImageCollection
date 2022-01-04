#include "list.h"
#include "../converter.h"
#include "pointer.h"

template <typename T>
KTools::Kff::List<T>::List(KTools::Kff::Manager *man) : KTools::Kff::String(man) {}

template <typename T>
KTools::Kff::List<T>::List(KTools::Kff::Manager *man, const qint64 pos) : KTools::Kff::String(man, pos)
{
    for (qint64 i = 0 ; i < data.length(); i += sizeof(T))
    {
        list.append(KTools::Converter::byteArrayToT<T>(data.mid(i, sizeof(T))));
    }
}

template <>
KTools::Kff::List<KTools::Kff::String>::List(KTools::Kff::Manager *man, const qint64 pos) : KTools::Kff::String(man, pos)
{
    for (qint64 i = 0 ; i < data.length(); i += 9)
    {
        Pointer pointer(man, data.mid(i, 9));
        list.append(String(&pointer));
    }
}

template <>
KTools::Kff::List<KTools::Kff::Pointer>::List(KTools::Kff::Manager *man, const qint64 pos) : KTools::Kff::String(man, pos)
{
    for (qint64 i = 0 ; i < data.length(); i += 9)
    {
        Pointer pointer(man, data.mid(i, 9));
        list.append(pointer);
    }
}

template <typename T>
KTools::Kff::List<T>::List(Pointer *poi) : KTools::Kff::String(poi)
{
    for (qint64 i = 0 ; i < data.length(); i += sizeof(T))
    {
        list.append(KTools::Converter::byteArrayToT<T>(data.mid(i, sizeof(T))));
    }
}

template <>
KTools::Kff::List<KTools::Kff::String>::List(Pointer *poi) : KTools::Kff::String(poi)
{
    for (qint64 i = 0 ; i < data.length(); i += 9)
    {
        Pointer pointer(poi->getManager(), data.mid(i, 9));
        list.append(String(&pointer));
    }
}

template <>
KTools::Kff::List<KTools::Kff::Pointer>::List(Pointer *poi) : KTools::Kff::String(poi)
{
    for (qint64 i = 0 ; i < data.length(); i += 9)
    {
        Pointer pointer(poi->getManager(), data.mid(i, 9));
        list.append(pointer);
    }
}

template <typename T>
KTools::Kff::List<T>::~List()
{
    data = "";
    for (int i = 0; i < list.size(); i++)
    {
        data += KTools::Converter::toByteArray<T>(list[i]);
    }
}

template <>
KTools::Kff::List<KTools::Kff::String>::~List()
{
    data = "";
    for (int i = 0; i < list.size(); i++)
    {
        data += list[i].pointer->getAll();
    }
}

template <>
KTools::Kff::List<KTools::Kff::Pointer>::~List()
{
    data = "";
    for (int i = 0; i < list.size(); i++)
    {
        data += list[i].getAll();
    }
}

template <typename T>
T& KTools::Kff::List<T>::operator[](const qint64 i)
{
    return list[i];
}

template <typename T>
KTools::Kff::List<T>& KTools::Kff::List<T>::operator+=(const T other)
{
    list.append(other);
    return list;
}

template <typename T>
void KTools::Kff::List<T>::remove(const qint64 i)
{
    list.removeAt(i);
}

template <typename T>
void KTools::Kff::List<T>::safeRemove(const qint64 i)
{
    list.removeAt(i);
}

template <>
void KTools::Kff::List<KTools::Kff::String>::safeRemove(const qint64 i)
{
    list[i].deleteVariable();
    list.removeAt(i);
}

template <>
void KTools::Kff::List<KTools::Kff::Pointer>::safeRemove(const qint64 i)
{
    list[i].deleteData();
    list.removeAt(i);
}

template void KTools::Kff::List<qint8>::safeRemove(const qint64);
template void KTools::Kff::List<qint16>::safeRemove(const qint64);
template void KTools::Kff::List<qint32>::safeRemove(const qint64);
template void KTools::Kff::List<qint64>::safeRemove(const qint64);
template void KTools::Kff::List<quint8>::safeRemove(const qint64);
template void KTools::Kff::List<quint16>::safeRemove(const qint64);
template void KTools::Kff::List<quint32>::safeRemove(const qint64);
template void KTools::Kff::List<quint64>::safeRemove(const qint64);
template void KTools::Kff::List<double>::safeRemove(const qint64);
template void KTools::Kff::List<bool>::safeRemove(const qint64);
