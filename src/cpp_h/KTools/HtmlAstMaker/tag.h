#ifndef HTMLTAG_H
#define HTMLTAG_H

#include <QString>
#include <QMap>

namespace KTools::HtmlAst
{
    class Tag
    {
    public:
        Tag();
        ~Tag();
        void setSelfclosingness(const bool sc);
        void setName(const QString &newName);
        void addAttribute(const QString &key, const QString &value);
        void setInnerContent(const QString &content);
        void addChildTag(Tag &localTag);
        void setParentTag(Tag &localTag);

        bool isSelfclosing();
        QString& getName();
        QString getAttributeValue(const QString &key);
        QMap<QString, QString>& getAttributes();
        QString& getInnerContent();
        QVector<Tag*>& getChildTags();
        Tag& getParentTag();
        qint32 getChildTagCounter();

        void clearAttributes();
        Tag& find(const qint32 numb); // returns child tag by number
        Tag& find(const QString path); // returns tag by path. example "1/0/2/5"
        bool isExist(qint32 childTagIndex);

        enum class StatusEnum
        {
            TagClosingDetected,
            TagInvalid,
            TagValid
        };
        StatusEnum status;

    private:
        bool selfclosing = false;
        QString name;
        QMap<QString, QString> attributes;
        QString innerContent;
        QVector<Tag*> childTags;
        Tag *parentTag;
        bool parentTagSetted = false;
        qint32 childTagCounter = 0;
    };
}

#endif // HTMLTAG_H
