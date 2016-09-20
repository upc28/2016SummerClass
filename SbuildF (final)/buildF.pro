#-------------------------------------------------
#
# Project created by QtCreator 2016-02-19T10:14:23
#
#-------------------------------------------------

QT       += core gui
QT         += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = buildF
TEMPLATE = app


SOURCES += main.cpp\
    startscreen.cpp \
    privatescreen.cpp \
    minheap.cpp \
    hafdisplay.cpp \
    historydisplay.cpp

HEADERS  += \
    startscreen.h \
    privatescreen.h \
    minheap.h \
    hafdisplay.h \
    historydisplay.h \
    msgtype.h

FORMS    += widget.ui \
    chatprivate.ui \
    compile.ui \
    startscreen.ui \
    privatescreen.ui \
    hafdisplay.ui \
    historydisplay.ui
