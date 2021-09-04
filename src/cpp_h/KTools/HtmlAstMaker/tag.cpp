#include "tag.h"

KTools::HtmlAst::Tag::Tag()
{
    childTags = QVector<Tag*>();
}

KTools::HtmlAst::Tag::~Tag()
{
    if (parentTagSetted)
        delete parentTag;
    for (qint32 i = 0; i < childTags.size(); i++)
        delete childTags.at(i);
}

bool KTools::HtmlAst::Tag::isSelfclosing()
{
    return selfclosing;
}

void KTools::HtmlAst::Tag::setSelfclosingness(bool sc)
{
    selfclosing = sc;
}

void KTools::HtmlAst::Tag::setName(const QString &newName)
{
    name = newName;
}

QString& KTools::HtmlAst::Tag::getName()
{
    return name;
}

void KTools::HtmlAst::Tag::addAttribute(const QString &key, const QString &value)
{
    attributes.insert(key, value);
}

QString KTools::HtmlAst::Tag::getAttributeValue(const QString &key)
{
    return attributes.value(key);
}

QMap<QString, QString>& KTools::HtmlAst::Tag::getAttributes()
{
    return attributes;
}

void KTools::HtmlAst::Tag::clearAttributes()
{
    attributes.clear();
}

void KTools::HtmlAst::Tag::setInnerContent(const QString &content)
{
    innerContent = content;
}

QString& KTools::HtmlAst::Tag::getInnerContent()
{
    return innerContent;
}

void KTools::HtmlAst::Tag::addChildTag(Tag &localTag)
{
    childTags.append(&localTag);
    localTag.setParentTag(*this);
    ++childTagCounter;
}

QVector<KTools::HtmlAst::Tag*>& KTools::HtmlAst::Tag::getChildTags()
{
    return childTags;
}

void KTools::HtmlAst::Tag::setParentTag(Tag &localTag)
{
    parentTag = &localTag;
    parentTagSetted = true;
}

KTools::HtmlAst::Tag& KTools::HtmlAst::Tag::getParentTag()
{
    return *parentTag;
}

KTools::HtmlAst::Tag& KTools::HtmlAst::Tag::find(const qint32 numb)
{
    return *childTags[numb];
}

qint32 KTools::HtmlAst::Tag::getChildTagCounter()
{
    return childTagCounter;
}

bool KTools::HtmlAst::Tag::isExist(qint32 childTagIndex)
{
    if (childTagIndex < childTags.size())
        return true;
    else
        return false;
}

KTools::HtmlAst::Tag& KTools::HtmlAst::Tag::find(const QString path)
{
    QList<QString> list = path.split("/", Qt::SkipEmptyParts);
    Tag *currTag = this;
    for (qint32 i = 0; i < list.size(); i++)
    {
        if (list[i] == "..")
        {
            currTag = &currTag->getParentTag();
        }
        else
        {
            currTag = &currTag->find(list[i].toInt());
        }
    }
    return *currTag;
}
