# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = app
TARGET = Kactus2
DESTDIR = ./executable
QT += core xml widgets gui printsupport help
CONFIG += release
DEFINES += _WINDOWS QT_DLL QT_XMLPATTERNS_LIB QT_XML_LIB QT_HAVE_MMX QT_HAVE_3DNOW QT_HAVE_SSE  QT_HAVE_MMXEXT QT_HAVE_SSE2 KACTUS2_EXPORTS QT_HELP_LIB QT_PRINTSUPPORT_LIB QT_WIDGETS_LIB
INCLUDEPATH += . \
    ./GeneratedFiles \
    $(QTDIR)/../qttools/include \
    $(QTDIR)/../qttools/include/QtHelp \
    $(QTDIR)/../qtxmlpatterns/include/QtXmlPatterns \
    $(QTDIR)/../qtxmlpatterns/include
LIBS += -L./executable \
    -lIPXACTmodels
DEPENDPATH += .
linux-g++*:QMAKE_CXXFLAGS += -fPIE
linux-g++*:LIBS += -pie -rdynamic
win32-g++*:LIBS += -Wl,--export-all-symbols,--out-implib,executable/libKactus2.a
MOC_DIR += ./GeneratedFiles
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(Kactus2.pri)
win32:RC_FILE = Kactus2.rc
unix:QMAKE_POST_LINK = ln -f -s Kactus2 executable/libKactus2.so; ./createhelp
target.path = $$bin_path
INSTALLS += target
