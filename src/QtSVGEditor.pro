TEMPLATE = app
TARGET = QtSVGEditor
DESTDIR = ./debug
QT += core gui widgets printsupport
CONFIG += debug
DEFINES += QT_QML_DEBUG _USING_V140_SDK71_ _WINDOWS _ATL_XP_TARGETING QT_PRINTSUPPORT_LIB QT_WIDGETS_LIB
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./debug \
    ./GeneratedFiles/Debug
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/Debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(QtSVGEditor.pri)
