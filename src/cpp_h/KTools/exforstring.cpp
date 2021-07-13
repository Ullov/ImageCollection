#include "exforstring.h"

#include <string>
#include <QString>
#include <QVector>
#include <QRegularExpression>

KTools::ExForString::ExForString() {}

void KTools::ExForString::replace(QString &inp, const QVector<QString> &whatReplace, const QVector<QString> &onWhatReplace)
{
    std::string input = inp.toStdString();
    for (int i = 0; i < whatReplace.size(); i++)
    {
        size_t startPos = 0;
        while ((startPos = input.find(whatReplace[i].toStdString(), startPos)) != std::string::npos)
        {
            input.replace(startPos, whatReplace[i].length(), onWhatReplace[i].toStdString());
            startPos += onWhatReplace[i].length();
        }
    }
    inp = QString::fromStdString(input);
}

void KTools::ExForString::replace(QByteArray &inp, const QVector<QByteArray> &whatReplace, const QVector<QByteArray> &onWhatReplace)
{
    std::string input = inp.toStdString();
    for (int i = 0; i < whatReplace.size(); i++)
    {
        size_t startPos = 0;
        while ((startPos = input.find(whatReplace[i].toStdString(), startPos)) != std::string::npos)
        {
            input.replace(startPos, whatReplace[i].length(), onWhatReplace[i].toStdString());
            startPos += onWhatReplace[i].length();
        }
    }
    inp = QByteArray::fromStdString(input);
}

void KTools::ExForString::deleteChars(QString &inp, const QVector<QString> &whatDelete)
{
    QVector<QString> voids;
    for (int i = 0; i < whatDelete.size(); i++)
        voids.push_back("");
    replace(inp, whatDelete, voids);
}

void KTools::ExForString::executeRegex(const QString &data, const QVector<QString> &pattens, QVector<QVector<QVector<QString>>> &regexResult)
{
    regexResult.resize(pattens.size());
    for (int i = 0; i < pattens.size(); i++)
    {
        QRegularExpression re(pattens[i]);
        QRegularExpressionMatchIterator reI = re.globalMatch(data);
        for (int j = 0; reI.hasNext(); j++)
        {
            regexResult[i].resize(regexResult[i].size() + 1);
            QRegularExpressionMatch match = reI.next();
            for (int jj = 0; jj <= match.lastCapturedIndex(); jj++)
                regexResult[i][j].push_back(match.captured(jj));
        }
    }
}

void KTools::ExForString::executeRegex(const QString &data, const QString &pattern, QVector<QString> &result)
{
    result.clear();
    QRegularExpression re(pattern);
    QRegularExpressionMatchIterator i = re.globalMatch(data);
    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        result.append(match.captured(1));
    }
}

void KTools::ExForString::executeRegex(const QString &data, const QString &pattern, QVector<double> &result)
{
    result.clear();
    QRegularExpression re(pattern);
    QRegularExpressionMatchIterator i = re.globalMatch(data);
    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        result.append(match.captured(1).toDouble());
    }
}

void KTools::ExForString::executeRegex(const QString &data, const QString &pattern, QVector<qint64> &result)
{
    result.clear();
    QRegularExpression re(pattern);
    QRegularExpressionMatchIterator i = re.globalMatch(data);
    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        result.append(match.captured(1).toLong());
    }
}

QVector<qint64> KTools::ExForString::getIntegerNumberFromString(const QString &str)
{
    QVector<qint64> result;
    executeRegex(str, "(\\d+.\\d+)", result);
    return result;
}

QVector<double> KTools::ExForString::getDoubleNumberFromString(const QString &str)
{
    QVector<double> result;
    executeRegex(str, "(\\d+[.,]\\d+|\\d+)", result);
    return result;
}
