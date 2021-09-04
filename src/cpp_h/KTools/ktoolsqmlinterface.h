#ifndef KTOOLSQMLINTERFACE_H
#define KTOOLSQMLINTERFACE_H

#include <QObject>
#include <QVariant>
//#include "../Parsers/parserclass.h"

class KToolsQmlInterface : public QObject
{
    Q_OBJECT
public:
    KToolsQmlInterface();

public slots:
    // Start OptionsHandler
    void optionsHandlerSetRootProgramPath(const QString path);
    void optionsHandlerEmitRootProgramPath();
    void optionsHandlerSlotSetParam(const QString pathToParam, const QString param);
    void optionsHandlerSlotGetParam(const QString pathToParam);
    // End OptionsHandler

public:
    // Start KTools::Curl
    //static void curlSendProgress(const QList<double> list, const qint64 millisecondsFromStart, const Parsers::Parsers downloaderType);
    // End KTools::Curl

    static KToolsQmlInterface obj;

signals:
    // Start OptionsHandler
    void optionsHandlerSendRootProgramPath(const QString path);
    void optionsHandlerSignalParam(const QString pathToParam, const QVariant param);
    // End OptionsHandler
    // Start KTools::Curl
    //void curlProgressSignal(const QList<double> list, const qint64 millisecondsFromStart, const Parsers::Parsers downloaderType);
    // End KTools::Curl
};

#endif // KTOOLSQMLINTERFACE_H
