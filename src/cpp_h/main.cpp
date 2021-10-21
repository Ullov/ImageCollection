#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QUuid>
#include <QQmlEngine>
#include "FsExplorer/fshandler.h"
#include "KTools/uuidslist.h"
#include "ImageViewer/pixmapcontainer.h"
#include "ImageViewer/pixmapimage.h"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    qmlRegisterType<UuidsList>("QmlUuidsList", 1, 0, "UuidsList");
    qmlRegisterType<PixmapContainer>("QmlPixmapContainer", 1, 0, "PixmapContainer");
    qmlRegisterType<PixmapImage>("QmlPixmapImage", 1, 0, "PixmapImage");
    static FsHandler *fsExplorerHandle = new FsHandler();
    static UuidsList ids;
    ids.init(&ids);
    ids.createItems(20);

    engine.rootContext()->setContextProperty("uuidsList", &ids);
    engine.rootContext()->setContextProperty("fsExplorerHandle", fsExplorerHandle);
    engine.load(url);

    return app.exec();
}
