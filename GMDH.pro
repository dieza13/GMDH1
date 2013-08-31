#-------------------------------------------------
#
# Project created by QtCreator 2013-01-30T20:19:50
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widget

TARGET = GMDH
TEMPLATE = app

CONFIG += qwt
LIBS += -lqwt -lm

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
    sample.cpp \
    calculatedrange.cpp

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
    sample.h \
    EnterRange.h \
    calculatedrange.h

FORMS    += mainwindow.ui \
    neirosetdialog.ui \
    getexmpledialog.ui

RESOURCES += \
    resources.qrc
INCLUDEPATH += /usr/include/qwt
LIBS += -L/usr/lib -lqwt
