QT += quick

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        cpp_h/KTools/StaticVarsDeclarations.cpp \
        cpp_h/KTools/converter.cpp \
        cpp_h/KTools/exforstring.cpp \
        cpp_h/KTools/file.cpp \
        cpp_h/KTools/fileidentifier.cpp \
        cpp_h/KTools/log.cpp \
        cpp_h/KTools/options.cpp \
        cpp_h/main.cpp

RESOURCES += resources/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    cpp_h/KTools/converter.h \
    cpp_h/KTools/enums.h \
    cpp_h/KTools/exforstring.h \
    cpp_h/KTools/file.h \
    cpp_h/KTools/fileidentifier.h \
    cpp_h/KTools/log.h \
    cpp_h/KTools/options.h
