#include "string.h"
#include "manager.h"
#include "pointer.h"
#include "variabletypes.h"
#include "nameinfo.h"

KTools::Kff::String::String(Manager *man)
{
    pointer = new Pointer(man, Pointer::PointerType::VariableTypes, man->getStrings()->add("", VariableTypes::Type::String));
    data = "";
}

KTools::Kff::String::String(Manager *man, const qint64 pos)
{
    pointer = new Pointer(man, Pointer::PointerType::VariableTypes, pos);
    data = pointer->getData<QByteArray>();
}

KTools::Kff::String::String(const Pointer *poi)
{
    pointer = new Pointer(*poi);
    data = pointer->getData<QByteArray>();
}

KTools::Kff::String::~String()
{
    delete pointer;
}

QByteArray KTools::Kff::String::mid(const qint64 pos, const qint64 len)
{
    return data.mid(pos, len);
}

QByteArray KTools::Kff::String::readAll()
{
    return data;
}

void KTools::Kff::String::insert(const qint64 pos, const QByteArray content)
{
    data.insert(pos, content);
    flush();
}

void KTools::Kff::String::flush()
{
    pointer->writeData(data);
}

char KTools::Kff::String::at(const qint64 pos)
{
    return data.at(pos);
}

const QByteRef KTools::Kff::String::operator[](const uint pos)
{
    return data[pos];
}

QByteArray& KTools::Kff::String::operator=(const QByteArray &other)
{
    data = other;
    flush();
    return data;
}

bool KTools::Kff::String::operator==(const QByteArray &other)
{
    return data == other;
}

QByteArray& KTools::Kff::String::operator+=(const QByteArray &other)
{
    data.append(other);
    flush();
    return data;
}
