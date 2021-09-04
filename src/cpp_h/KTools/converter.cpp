#include "converter.h"

#include <QDataStream>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "exforstring.h"
#include <QTextCodec>

KTools::Converter::Converter() {}

template<typename T>
T KTools::Converter::byteArrayToT(const QByteArray &bArr)
{
    QDataStream str(bArr);
    T result;
    str >> result;
    return result;
}
template<>
const char* KTools::Converter::byteArrayToT<const char*>(const QByteArray &bArr)
{
    return bArr.data();
}
template<>
QString KTools::Converter::byteArrayToT<QString>(const QByteArray &bArr)
{
    return bArr;
}
template<>
QByteArray KTools::Converter::byteArrayToT<QByteArray>(const QByteArray &bArr)
{
    return bArr;
}



template<typename QByteArray, typename E>
E KTools::Converter::convert(const QByteArray &data)
{
    return byteArrayToT<E>(data);
}
template<>
QJsonObject KTools::Converter::convert<QString, QJsonObject>(const QString &data)
{
    QJsonObject obj;
    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    if (!doc.isNull())
    {
        if (doc.isObject())
            obj = doc.object();
        else
            return {}; // document is not an object
    }
    else
        return {}; // invalid JSON
    return obj;
}
template<>
QJsonArray KTools::Converter::convert<QString, QJsonArray>(const QString &data)
{
    QJsonArray arr;
    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    if (!doc.isNull())
    {
        if (doc.isArray())
            arr = doc.array();
        else
            return {}; // document is not an object
    }
    else
        return {}; // invalid JSON
    return arr;
}
template<>
QByteArray KTools::Converter::convert<qint8, QByteArray>(const qint8 &data)
{
    return toByteArray<qint8>(data);
}
template<>
QByteArray KTools::Converter::convert<quint8, QByteArray>(const quint8 &data)
{
    return toByteArray<quint8>(data);
}
template<>
QByteArray KTools::Converter::convert<qint16, QByteArray>(const qint16 &data)
{
    return toByteArray<qint16>(data);
}
template<>
QByteArray KTools::Converter::convert<quint16, QByteArray>(const quint16 &data)
{
    return toByteArray<quint16>(data);
}
template<>
QByteArray KTools::Converter::convert<qint32, QByteArray>(const qint32 &data)
{
    return toByteArray<qint32>(data);
}
template<>
QByteArray KTools::Converter::convert<quint32, QByteArray>(const quint32 &data)
{
    return toByteArray<quint32>(data);
}
template<>
QByteArray KTools::Converter::convert<qint64, QByteArray>(const qint64 &data)
{
    return toByteArray<qint64>(data);
}
template<>
QByteArray KTools::Converter::convert<quint64, QByteArray>(const quint64 &data)
{
    return toByteArray<quint64>(data);
}
template<>
QByteArray KTools::Converter::convert<float, QByteArray>(const float &data)
{
    return toByteArray<float>(data);
}
template<>
QByteArray KTools::Converter::convert<double, QByteArray>(const double &data)
{
    return toByteArray<double>(data);
}
template<>
unsigned char KTools::Converter::convert<char, unsigned char>(const char &data)
{
    return static_cast<unsigned char>(data);
}
template<>
QByteArray KTools::Converter::convert<QString, QByteArray>(const QString &data)
{
    return data.toUtf8();
}
template<>
unsigned char KTools::Converter::convert<QString, unsigned char>(const QString &data)
{
    const char *tmp = convert<QString, QByteArray>(data).constData();
    return convert<char, unsigned char>(*tmp);
}
template<>
QString KTools::Converter::convert<QJsonObject, QString>(const QJsonObject &data)
{
    QJsonDocument jDoc(data);
    return jDoc.toJson();
}
template<>
QByteArray KTools::Converter::convert<QJsonObject, QByteArray>(const QJsonObject &data)
{
    return convert<QJsonObject, QString>(data).toUtf8();
}
template<>
QJsonObject* KTools::Converter::convert<QString, QJsonObject*>(const QString &data)
{
    QJsonObject *obj;
    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    if (!doc.isNull())
    {
        if (doc.isObject())
            obj = new QJsonObject(doc.object());
        else
            return {}; // document is not an object
    }
    else
        return {}; // invalid JSON
    return obj;
}


template<typename T>
QByteArray KTools::Converter::toByteArray(const T &data)
{   
    QByteArray result;
    QDataStream str(&result, QIODevice::WriteOnly);
    str << data;
    return result;
}

QString KTools::Converter::numberToUtf8(const quint16 &code)
{
    QString resultChar = QString::fromUtf16(&code);
    resultChar.resize(1);
    return resultChar;
}

void KTools::Converter::percentEncodingToString(QString &encodedString)
{
    bool forWhat;
    for (int i = 0; i < encodedString.size();)
    {
        QChar currChar = encodedString[i];
        if (currChar == "%")
        {
            QString code = encodedString.mid(i + 1, 2);
            QString repl = QString((char)code.toInt(&forWhat, 16));
            encodedString.replace("%" + code, repl);
            if (i > 2)
                i = i - 2;
        }
        else
        {
            ++i;
        }
    }
}

void KTools::Converter::percentEncodingToString(QByteArray &encodedString)
{
    QString str = encodedString;
    bool forWhat;
    for (int i = 0; i < str.size();)
    {
        QChar currChar = str[i];
        if (currChar == "%")
        {
            QString code = str.mid(i + 1, 2);
            QString repl = QString((char)code.toInt(&forWhat, 16));
            str.replace("%" + code, repl);
            if (i > 2)
                i = i - 2;
        }
        else
        {
            ++i;
        }
    }
    encodedString = str.toUtf8();
}

void KTools::Converter::toNtfsCompatibleString(QString &data)
{
    QVector<QString> wrongChars = {"\"", "|", "/", ":", "*", "?", ">", "<"};
    QVector<QString> voids;
    for (int i = 0; i < wrongChars.size(); i++)
        voids.push_back("");
    ExForString::deleteChars(data, wrongChars);
}

void KTools::Converter::convertHtmlEntities(QString &inp)
{
    QVector<QString> htmlEntities = {"&amp;", "&quot;", "&apos;", "&it;", "&gt;", "&nbsp;"};
    QVector<QString> rightSumbols = {"&", "\"", "'", "<", ">", " "};
    ExForString::replace(inp, htmlEntities, rightSumbols);
}

void KTools::Converter::convertHtmlEntities(QByteArray &inp)
{
    QVector<QByteArray> htmlEntities = {"&amp;", "&quot;", "&apos;", "&it;", "&gt;", "&nbsp;"};
    QVector<QByteArray> rightSumbols = {"&", "\"", "'", "<", ">", " "};
    ExForString::replace(inp, htmlEntities, rightSumbols);
}

void KTools::Converter::convertHtmlHexCodes(QString &data)
{
    QVector<QVector<QVector<QString>>> regexResult;
    QVector<QString> patterns;
    patterns.push_back("&#(\\d+);");
    ExForString::executeRegex(data, patterns, regexResult);
    for (int i = 0; i < regexResult[0].size(); i++)
    {
        regexResult[0][i][1] = numberToUtf8(regexResult[0][i][1].toUShort());
        data.replace(regexResult[0][i][0], regexResult[0][i][1]);
    }
    convertHtmlEntities(data);
}

QString KTools::Converter::nationalEncodingToUtf8(const QByteArray &inputEncoding, const QByteArray &data)
{
    QTextCodec *codec = QTextCodec::codecForName(inputEncoding);
    return codec->toUnicode(data);
}

template qint8 KTools::Converter::byteArrayToT<qint8>(const QByteArray&);
template quint8 KTools::Converter::byteArrayToT<quint8>(const QByteArray&);
template qint16 KTools::Converter::byteArrayToT<qint16>(const QByteArray&);
template quint16 KTools::Converter::byteArrayToT<quint16>(const QByteArray&);
template qint32 KTools::Converter::byteArrayToT<qint32>(const QByteArray&);
template quint32 KTools::Converter::byteArrayToT<quint32>(const QByteArray&);
template qint64 KTools::Converter::byteArrayToT<qint64>(const QByteArray&);
template quint64 KTools::Converter::byteArrayToT<quint64>(const QByteArray&);

template QByteArray KTools::Converter::toByteArray<qint8>(const qint8&);
template QByteArray KTools::Converter::toByteArray<quint8>(const quint8&);
template QByteArray KTools::Converter::toByteArray<qint16>(const qint16&);
template QByteArray KTools::Converter::toByteArray<quint16>(const quint16&);
template QByteArray KTools::Converter::toByteArray<qint32>(const qint32&);
template QByteArray KTools::Converter::toByteArray<quint32>(const quint32&);
template QByteArray KTools::Converter::toByteArray<qint64>(const qint64&);
template QByteArray KTools::Converter::toByteArray<quint64>(const quint64&);
