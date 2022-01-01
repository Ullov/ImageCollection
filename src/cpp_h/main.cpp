#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QUuid>
#include <QQmlEngine>
#include "FsExplorer/fshandler.h"
#include "KTools/uuidslist.h"
#include "KTools/options.h"
#include "ImageViewer/pixmapimage.h"
#include "KTools/Kff/metainfofs.h"
#include "KTools/Kff/manager.h"
#include "KTools/Kff/rawstream.h"
#include "KTools/Kff/fixedtypes.h"
#include "KTools/Kff/variabletypes.h"
#include "ImageStorage/interface.h"

void testFunction ()
{
    /*QImage img = QImage(R"#(E:/Win7アプリ/downloads/images/test/62p8Qle.gif)#");
    QString path = "E:/Win7アプリ/downloads/images/test";
    for (int i = 1; i < 101; i++)
        img.save(path + "/" + QString::number(i) + ".webp", "WEBP", i);*/

    /*KTools::Kff::Manager manger = KTools::Kff::Manager("E:/Win7アプリ/downloads/kff.kff", KTools::Kff::Manager::OpenMode::Keep);
    KTools::Kff::RawStream *stream = manger.getDefaultStream();
    stream->write(KTools::File::readFile<QByteArray>("E:/Win7アプリ/downloads", "prod.keys"));
    QByteArray content;
    KTools::Kff::FixedTypes *numbers = manger.getNumbers();
    QList<qint64> addresses;
    addresses.append(numbers->add(67548ll, KTools::Kff::FixedTypes::Type::Int64));
    addresses.append(numbers->add(908798ll));
    addresses.append(numbers->add(50, KTools::Kff::FixedTypes::Type::Int32));
    addresses.append(numbers->add(13, KTools::Kff::FixedTypes::Type::Int32));
    addresses.append(numbers->add(true, KTools::Kff::FixedTypes::Type::Bool));
    addresses.append(numbers->add<double>(1.986543456, KTools::Kff::FixedTypes::Type::Float));
    content.append(QByteArray::number(numbers->get<qint64>(addresses[0])) + '\n');
    numbers->change(5436657ll, addresses[0]);
    content.append(QByteArray::number(numbers->get<qint64>(addresses[0])) + '\n');
    content.append(QByteArray::number(numbers->get<double>(addresses[5]), 'g', 9) + '\n');
    content.append(QByteArray::number(numbers->get<bool>(addresses[4])) + '\n');
    KTools::Kff::VariableTypes *strs = manger.getStrings();
    QByteArray str;
    QByteArray str2;
    str.append(522, '1');
    str2.append(22, '2');
    qint64 addr = strs->add(str, KTools::Kff::VariableTypes::Type::String);
    qint64 addr2 = strs->add(str, KTools::Kff::VariableTypes::Type::String);
    content.append(strs->readString(addr) + '\n');
    qint64 addr3 = strs->rewriteVariable(str2, addr2, KTools::Kff::VariableTypes::Type::String);
    content.append(strs->readString(addr2) + '\n');
    content.append(strs->readString(addr3) + '\n');
    QList<QByteArray> pointerList;
    for (int i = 0; i < 10; i++)
        pointerList.append(manger.makePointer(KTools::Kff::Manager::PointerType::VariableTypes, i * -128));
    qint64 addr4 = strs->appendPointers(pointerList);
    pointerList.clear();
    pointerList.append(manger.makePointer(KTools::Kff::Manager::PointerType::FixedTypes, 47));
    addr4 = strs->appendPointers(pointerList, addr4);
    pointerList = strs->getPointers(addr4);
    strs->deleteVariable(addr4);

    KTools::File::writeFile(content, "E:/Win7アプリ/downloads", "test.txt", QIODevice::Truncate | QIODevice::WriteOnly);*/
}

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
    //qmlRegisterType<ImageStorage::ImageStorage>("QmlImageStorage", 1, 0, "ImageStorage");
    KTools::Kff::registerTypesForQml();

    static KTools::Options *options = new KTools::Options();
    FsHandler *fsExplorerHandle = new FsHandler();
    KTools::Kff::MetainfoFs db(options);
    ImageStorage::Interface imageStorage(&db);

    UuidsList::ids.init(&UuidsList::ids);
    UuidsList::ids.createItems(20);

    engine.rootContext()->setContextProperty("uuidsList", &UuidsList::ids);
    engine.rootContext()->setContextProperty("fsExplorerHandle", fsExplorerHandle);
    engine.rootContext()->setContextProperty("imageStorage", &imageStorage);
    engine.load(url);

    testFunction();

    return app.exec();
}


Q_DECLARE_METATYPE(KTools::Kff::NameInfo)
Q_DECLARE_METATYPE(KTools::Kff::NameInfoList)
