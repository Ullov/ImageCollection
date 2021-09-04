#ifndef KTOOLS_HTMLAST_OBJECT_H
#define KTOOLS_HTMLAST_OBJECT_H

#include <QString>
#include "tag.h"
#include "../log.h"

namespace KTools::HtmlAst
{
    class Object
    {
    public:
        Object();

        bool makeAst(const QString &data);
        Tag *rootTag;
        bool parseJs = false;

    private:

        bool escapedChar = false;
        const QString *htmlText;
        qint32 tagStartPos;

        Tag& readTag(qint32 &pos, const qint32 &endPos);
        bool readTagName(qint32 &pos, Tag &tagClass);
        bool readTagAttributes(qint32 &pos, Tag &tagClass);
        bool setRootTag(Tag &tag);
        qint32 findTagEndingPart(const qint32 &pos, const qint32 &endPos, Tag &tagClass, qint32 differenceEdAndOp = 0);
        //qint32 findTagEndingPartSecondPass(qint32 pos, qint32 endPos, HtmlTag &tagClass, qint32 differenceEdAndOp);

        struct JsArrsAndObjects
        {
            QVector<QJsonObject> objects;
            QVector<QJsonArray> arrs;
        };

        enum class JsReadStatus
        {
            Success,
            InvalidString,
            NotFound
        };
        JsReadStatus readJs(const QString &inner);

    public:
        JsArrsAndObjects arrsAndObjs;
    };
}

#endif // KTOOLS_HTMLAST_OBJECT_H
