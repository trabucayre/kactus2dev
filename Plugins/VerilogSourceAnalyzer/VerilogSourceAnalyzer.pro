# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = lib
TARGET = VerilogSourceAnalyzer
DESTDIR = ../../executable/Plugins
QT += core xml widgets
CONFIG += plugin release
DEFINES += QT_DLL QT_WIDGETS_LIB VERILOGSOURCEANALYZER_LIB QT_XML_LIB
INCLUDEPATH += ./GeneratedFiles \
    ./../.. \
    . \
    ./GeneratedFiles/release
LIBS += -L"./../../executable" \
    -lIPXACTmodels
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/release
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(VerilogSourceAnalyzer.pri)

target.path = $$plugin_path
INSTALLS += target
