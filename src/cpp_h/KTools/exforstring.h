#ifndef KTOOLS_EXFORSTRING_H
#define KTOOLS_EXFORSTRING_H

#include <QtGlobal>

namespace KTools
{
    class ExForString
    {
    public:
        ExForString();

        static void replace(QString &inp, const QVector<QString> &whatReplace, const QVector<QString> &onWhatReplace);
        static void replace(QByteArray &inp, const QVector<QByteArray> &whatReplace, const QVector<QByteArray> &onWhatReplace);
        static void deleteChars(QString &inp, const QVector<QString> &whatDelete);
        static void executeRegex(const QString &data, const QVector<QString> &pattens, QVector<QVector<QVector<QString>>> &regexResult);
        static void executeRegex(const QString &data, const QString &pattern, QVector<QString> &result);
        static void executeRegex(const QString &data, const QString &pattern, QVector<double> &result);
        static void executeRegex(const QString &data, const QString &pattern, QVector<qint64> &result);
        static QVector<qint64> getIntegerNumberFromString(const QString &str);
        static QVector<double> getDoubleNumberFromString(const QString &str);
    };
}

#endif // KTOOLS_EXFORSTRING_H
