#-------------------------------------------------
#
# Project created by QtCreator 2013-01-30T20:19:50
#
#-------------------------------------------------

QT       += core gui

TARGET = GMDH
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    body.cpp \
    filereader.cpp \
    firstlayernets.cpp \
    getexmpledialog.cpp \
    itembody.cpp \
    layerbody.cpp \
    neiron.cpp \
    neironbody.cpp \
    neirosetdialog.cpp \
    netslayer.cpp \
    perceptron.cpp \
    perceptronbody.cpp \
    perceptroncontext.cpp \
    sample.cpp

HEADERS  += mainwindow.h \
    body.h \
    filereader.h \
    firstlayernets.h \
    getexmpledialog.h \
    itembody.h \
    layerbody.h \
    neiron.h \
    neironbody.h \
    neirosetdialog.h \
    netslayer.h \
    perceptron.h \
    perceptronbody.h \
    perceptroncontext.h \
    sample.h

FORMS    += mainwindow.ui \
    neirosetdialog.ui \
    getexmpledialog.ui

RESOURCES += \
    resources.qrc
