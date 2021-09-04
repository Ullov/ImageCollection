#include "object.h"

#include "../../KTools/converter.h"
#include <QJsonObject>
#include <QJsonArray>

KTools::HtmlAst::Object::Object() {}

bool KTools::HtmlAst::Object::makeAst(const QString &data)
{
    htmlText = &data;
    qint32 pos = 0;

    return setRootTag(readTag(pos, htmlText->size()));
}

KTools::HtmlAst::Tag& KTools::HtmlAst::Object::readTag(qint32 &pos, const qint32 &endPos)
{
    Tag *tagClass = new Tag();
    while (pos < endPos)
    {
        if (htmlText->mid(pos, 2) == "<!")
        {
            ++pos;
            while (htmlText->at(pos) != "<")
                ++pos;
        }
        else if (htmlText->at(pos) == "<")
        {
            bool tagNameReaded = false;
            bool tagClosed = false;
            ++pos; // skip < char
            if (htmlText->at(pos) == "/")
            {
                tagClass->status = Tag::StatusEnum::TagClosingDetected;
                return *tagClass;
            }
            while (pos < endPos)
            {
                if (!tagClosed)
                {
                    if (htmlText->at(pos) == ">")
                    {
                        tagClosed = true; // tag end detected
                        ++pos;
                    }
                    else if (htmlText->at(pos).isSpace() || htmlText->at(pos) == "/")
                    {
                        ++pos; // skip space char after < char
                    }
                    else if ((htmlText->at(pos).isLetter() || htmlText->at(pos) == "-") && !tagNameReaded)
                    {
                        readTagName(pos, *tagClass);
                        tagNameReaded = true;
                    }
                    else if ((htmlText->at(pos).isLetter() || htmlText->at(pos) == "-") && tagNameReaded)
                    {
                        readTagAttributes(pos, *tagClass);
                        tagClosed = true;
                    }
                }
                else
                {
                    qint32 tmpPos = findTagEndingPart(pos, endPos, *tagClass);

                    if (tagClass->getName().toLower() == "script")
                    {
                        if (parseJs)
                            readJs(tagClass->getInnerContent());

                        tagClass->status = Tag::StatusEnum::TagValid;
                        pos += tagClass->getInnerContent().size() + 9;
                        return *tagClass;
                    }

                    if (tagClass->isSelfclosing())
                    {
                        while (pos < tmpPos)
                        {
                            Tag *newTag = &readTag(pos, tmpPos);
                            if (newTag->status == Tag::StatusEnum::TagValid)
                            {
                                tagClass->addChildTag(*newTag);
                            }
                            else if (newTag->status == Tag::StatusEnum::TagInvalid)
                            {
                                delete newTag;
                            }
                            else if(newTag->status == Tag::StatusEnum::TagClosingDetected)
                            {
                                ++pos;
                            }
                        }
                        tagClass->status = Tag::StatusEnum::TagValid;
                        return *tagClass;
                    }
                    else
                    {
                        tagClass->status = Tag::StatusEnum::TagValid;
                        return *tagClass;
                    }
                }
            }
        }
        else
        {
            tagClass->status = Tag::StatusEnum::TagInvalid; // error handling
            pos++;
            return *tagClass;
        }
    }
    tagClass->status = Tag::StatusEnum::TagValid;
    return *tagClass;
}

bool KTools::HtmlAst::Object::readTagName(qint32 &pos, Tag &tagClass)
{
    qint32 start = pos;
    while (htmlText->at(pos).isLetter() || htmlText->at(pos) == "-" || htmlText->at(pos).isDigit())
    {
        ++pos;
    }
    tagClass.setName(htmlText->mid(start, pos - start));
    return true;
}

bool KTools::HtmlAst::Object::readTagAttributes(qint32 &pos, Tag &tagClass)
{
    while (htmlText->at(pos) != ">" || htmlText->mid(pos, 2) == "/>") // while tag not closed
    {
        qint32 startKey = pos;
        while ((htmlText->at(pos).isLetter() || htmlText->at(pos) == "-" || htmlText->at(pos) == ":") && htmlText->at(pos) != "=")
        {
            ++pos;
        }
        qint32 lenKey = pos - startKey; // key lenght
        QChar quiote = htmlText->at(pos + 1);
        if (htmlText->at(pos) == "=" && (quiote == "\"" || quiote == "'"))
        {
            pos += 2;
            qint32 startValue = pos;
            while (htmlText->at(pos) != quiote)
            {
                ++pos;
            }
            tagClass.addAttribute(htmlText->mid(startKey, lenKey), htmlText->mid(startValue, pos - startValue));
        }
        else
        {
            return false; // here should be error handling
        }
        ++pos; // skip " char
        while (htmlText->at(pos) == " " || htmlText->at(pos) == "\n" || htmlText->at(pos) == "\r" || htmlText->at(pos) == "\t") // skip spaces between attributes
            ++pos;
    }
    ++pos;
    return true;
}

bool KTools::HtmlAst::Object::setRootTag(Tag &tag)
{
    if (tag.status == Tag::StatusEnum::TagValid)
    {
        rootTag = &tag;
        return true;
    }
    else
        return false;
}

qint32 KTools::HtmlAst::Object::findTagEndingPart(const qint32 &pos, const qint32 &endPos, Tag &tagClass, qint32 differenceEdAndOp)
{
    qint32 tmpPos = pos; // this also ABSOLUTE end position
    ++tmpPos;
    QString tagOpeningPart = "<" + tagClass.getName();
    qint32 tagOpSize = tagOpeningPart.size();
    QString tagEndPart = "</" + tagClass.getName() + ">"; // thank to this variable ast maker now works in 6 times faster
    qint32 tagEdSize = tagEndPart.size();
    qint32 sameTagCounterEd = 0;
    qint32 sameTagCounterOp = 0;
    if (htmlText->mid(pos, tagEdSize) == tagEndPart)
    {
        tagClass.setSelfclosingness(true);
        return pos;
    }
    if (tagClass.getName() == "tr")
    {
        QVector<qint32> closingTagsPos;
        QVector<qint32> openingTagsPos;
        while (tmpPos < endPos + 1)
        {
            if (htmlText->mid(tmpPos, tagEdSize) == tagEndPart) // tag ending part detected
            {
                if (sameTagCounterOp == 0) // tag ending part for this tag
                {
                    tagClass.setInnerContent(htmlText->mid(pos, tmpPos - pos));
                    tagClass.setSelfclosingness(true);
                    //Logging::writeDebug(tagClass.getName() + "\t" + tagClass.getInnerContent().replace("\n", "").replace("\r", ""), "HtmlObject");
                    break;
                }
                else // tag ending part not for this tag
                {
                    --sameTagCounterOp;
                    ++sameTagCounterEd;
                    ++tmpPos;
                }
            }
            else // not ending part, something else
            {
                if (htmlText->mid(tmpPos - 1, tagOpSize) == tagOpeningPart) // another opening tag part detected
                {
                    ++sameTagCounterOp;
                    --sameTagCounterEd;
                }
                ++tmpPos;
            }
        }
    }
    else
    {
        while (tmpPos < endPos + 1)
        {
            if (htmlText->mid(tmpPos, tagEdSize) == tagEndPart) // tag ending part detected
            {
                if (sameTagCounterOp == 0) // tag ending part for this tag
                {
                    tagClass.setInnerContent(htmlText->mid(pos, tmpPos - pos));
                    tagClass.setSelfclosingness(true);
                    //Logging::writeDebug(tagClass.getName() + "\t" + tagClass.getInnerContent().replace("\n", "").replace("\r", ""), "HtmlObject");
                    break;
                }
                else // tag ending part not for this tag
                {
                    --sameTagCounterOp;
                    ++sameTagCounterEd;
                    ++tmpPos;
                }
            }
            else // not ending part, something else
            {
                if (htmlText->mid(tmpPos - 1, tagOpSize) == tagOpeningPart) // another opening tag part detected
                {
                    ++sameTagCounterOp;
                    --sameTagCounterEd;
                }
                ++tmpPos;
            }
        }
    }
    /*if (!tagClass.isSelfclosing() && sameTagCounterOp + sameTagCounterEd != 0)  // opening parts more than ending parts
    {                                                                           //this occurs whrn optionally selfclosing tags writed as selfclosing
        findTagEndingPart(pos, endPos, tagClass, sameTagCounterEd);
    }*/
    return tmpPos;
}

KTools::HtmlAst::Object::JsReadStatus KTools::HtmlAst::Object::readJs(const QString &inner)
{
    //QString inner = tagClass.getInnerContent();
    /*if (tagClass.getAttributeValue("src") != QString())
    {
        inner
    }*/
    if (inner.size() == 0)
        return JsReadStatus::NotFound;

    qint32 bracesCount = 0;
    qint32 startPos = 0;
    bool inObject = false;
    bool inArray = false;
    for (qint32 i = 0; i < inner.size(); i++)
    {
        if (inner[i] == '[')
        {
            ++bracesCount;
            if (!inObject && !inArray)
            {
                inArray = true;
                startPos = i;
            }
        }
        else if (inner[i] == '{')
        {
            ++bracesCount;
            if (!inObject && !inArray)
            {
                inObject = true;
                startPos = i;
            }
        }
        else if (inner[i] == "]")
        {
            --bracesCount;
            if (bracesCount == 0 && inArray)
            {
                QJsonArray jsArr = KTools::Converter::convert<QString, QJsonArray>(inner.mid(startPos, i - startPos + 1));
                if (!jsArr.isEmpty())
                {
                    arrsAndObjs.arrs.append(jsArr);
                }
                else
                {
                    i = startPos + 1;
                }
                inArray = false;
            }
        }
        else if (inner[i] == "}")
        {
            --bracesCount;
            if (bracesCount == 0 && inObject)
            {
                QJsonObject jsObj = KTools::Converter::convert<QString, QJsonObject>(inner.mid(startPos, i - startPos + 1));
                if (!jsObj.isEmpty())
                {
                    arrsAndObjs.objects.append(jsObj);
                }
                else
                {
                    i = startPos + 1;
                }
                inObject = false;
            }
        }
    }
}
