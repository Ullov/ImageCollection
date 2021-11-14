#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QUuid>
#include <QQmlEngine>
#include "FsExplorer/fshandler.h"
#include "KTools/uuidslist.h"
#include "KTools/options.h"
#include "ImageViewer/pixmapimage.h"
#include "ImageStorage/imagestorage.h"


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
    qmlRegisterType<ImageStorage>("QmlImageStorage", 1, 0, "ImageStorage");

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "db");
    db.setDatabaseName("settings.sqlite");
    static KTools::Options options = KTools::Options(db);
    QSqlDatabase imgDb = QSqlDatabase::addDatabase("QSQLITE", "imgDb");
    imgDb.setDatabaseName(options.getParam("/Path/Data").toString() + "/images.sqlite");
    ImageStorage imgStor(imgDb);
    FsHandler *fsExplorerHandle = new FsHandler();
    fsExplorerHandle->optionsObj = options;

    UuidsList::ids.init(&UuidsList::ids);
    UuidsList::ids.createItems(20);

    engine.rootContext()->setContextProperty("uuidsList", &UuidsList::ids);
    engine.rootContext()->setContextProperty("fsExplorerHandle", fsExplorerHandle);
    engine.rootContext()->setContextProperty("imageStorage", &imgStor);
    engine.load(url);

    return app.exec();
}
