QT += quick sql

CONFIG += c++17 object_parallel_to_source

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        cpp_h/FsExplorer/fshandler.cpp \
        cpp_h/ImageStorage/interface.cpp \
        cpp_h/ImageViewer/pixmapimage.cpp \
        cpp_h/KTools/HtmlAstMaker/object.cpp \
        cpp_h/KTools/HtmlAstMaker/tag.cpp \
        cpp_h/KTools/Kff/file.cpp \
        cpp_h/KTools/Kff/list.cpp \
        cpp_h/KTools/Kff/metainfofs.cpp \
        cpp_h/KTools/Kff/fixedtypes.cpp \
        cpp_h/KTools/Kff/manager.cpp \
        cpp_h/KTools/Kff/pointer.cpp \
        cpp_h/KTools/Kff/rawstream.cpp \
        cpp_h/KTools/Kff/string.cpp \
        cpp_h/KTools/Kff/variabletypes.cpp \
        cpp_h/KTools/StaticVarsDeclarations.cpp \
        cpp_h/KTools/archiverw.cpp \
        cpp_h/KTools/converter.cpp \
        cpp_h/KTools/curl.cpp \
        cpp_h/KTools/exforstring.cpp \
        cpp_h/KTools/file.cpp \
        cpp_h/KTools/fileidentifier.cpp \
        cpp_h/KTools/log.cpp \
        cpp_h/KTools/options.cpp \
        cpp_h/KTools/uuidslist.cpp \
        cpp_h/main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    cpp_h/FsExplorer/fshandler.h \
    cpp_h/ImageStorage/interface.h \
    cpp_h/ImageViewer/pixmapimage.h \
    cpp_h/KTools/HtmlAstMaker/object.h \
    cpp_h/KTools/HtmlAstMaker/tag.h \
    cpp_h/KTools/Kff/file.h \
    cpp_h/KTools/Kff/list.h \
    cpp_h/KTools/Kff/nameinfo.h \
    cpp_h/KTools/Kff/metainfofs.h \
    cpp_h/KTools/Kff/fixedtypes.h \
    cpp_h/KTools/Kff/manager.h \
    cpp_h/KTools/Kff/pointer.h \
    cpp_h/KTools/Kff/rawstream.h \
    cpp_h/KTools/Kff/string.h \
    cpp_h/KTools/Kff/variabletypes.h \
    cpp_h/KTools/archiverw.h \
    cpp_h/KTools/converter.h \
    cpp_h/KTools/curl.h \
    cpp_h/KTools/enums.h \
    cpp_h/KTools/exforstring.h \
    cpp_h/KTools/file.h \
    cpp_h/KTools/fileidentifier.h \
    cpp_h/KTools/log.h \
    cpp_h/KTools/options.h \
    cpp_h/KTools/uuidslist.h

#Curl
LIBS += -L$$PWD/libs/curl-x64/lib/dll/ -llibcurl.dll
INCLUDEPATH += $$PWD/libs/curl-x64/include
DEPENDPATH += $$PWD/libs/curl-x64/include

#KArchive
LIBS += -L$$PWD/libs/karchive-5.69.0/lib/ -llibKF5Archive.dll
INCLUDEPATH += $$PWD/libs/karchive-5.69.0/include
DEPENDPATH += $$PWD/libs/karchive-5.69.0/include

QMAKE_CXXFLAGS += -O0
