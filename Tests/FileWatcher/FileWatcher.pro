QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    ../../MarkdownViewer/FileWatcher.h \

SOURCES += \
    ../../MarkdownViewer/FileWatcher.cpp \
    tst_FileWatcher.cpp \

INCLUDEPATH += ../../MarkdownViewer
