# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = lib
TARGET = VHDLimport
DESTDIR = ../../executable/Plugins
QT += core xml widgets gui
CONFIG += debug
DEFINES += QT_DLL QT_WIDGETS_LIB QT_XML_LIB VHDLIMPORT_LIB
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/Debug \
    ./../..
LIBS += -L"./../../executable" \
    -lIPXACTmodels
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(VHDLimport.pri)

target.path = $$plugin_path
INSTALLS += target
