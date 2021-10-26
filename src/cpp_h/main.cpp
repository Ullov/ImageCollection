#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QUuid>
#include <QQmlEngine>
#include "FsExplorer/fshandler.h"
#include "KTools/uuidslist.h"
#include "KTools/options.h"
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
    qmlRegisterType<PixmapImage>("QmlPixmapImage", 1, 0, "PixmapImage");

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("settings.sqlite");
    static KTools::Options options = KTools::Options(db);
    static FsHandler *fsExplorerHandle = new FsHandler();
    fsExplorerHandle->optionsObj = options;

    static UuidsList ids;
    ids.init(&ids);
    ids.createItems(20);

    engine.rootContext()->setContextProperty("uuidsList", &ids);
    engine.rootContext()->setContextProperty("fsExplorerHandle", fsExplorerHandle);
    engine.load(url);

    return app.exec();
}
