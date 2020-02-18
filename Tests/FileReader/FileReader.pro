QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    ../../MarkdownViewer/FileReader.h \

SOURCES += \
    ../../MarkdownViewer/FileReader.cpp \
    tst_FileReader.cpp \

INCLUDEPATH += ../../MarkdownViewer
