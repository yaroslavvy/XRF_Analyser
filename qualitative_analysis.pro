#-------------------------------------------------
#
# Project created by QtCreator 2019-05-11T13:01:57
#
#-------------------------------------------------

QT       += core gui
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qualitative_analysis
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    singlewindow.cpp \
    spectrumspm.cpp \
    service.cpp \
    barclock.cpp \
    workareaview.cpp \
    charttest.cpp \
    spectrumchart.cpp \
    tabspecwindow.cpp \
    spectrumlistmodel.cpp

HEADERS += \
        mainwindow.h \
    singlewindow.h \
    spectrumspm.h \
    service.h \
    barclock.h \
    workareaview.h \
    charttest.h \
    spectrumchart.h \
    tabspecwindow.h \
    spectrumlistmodel.h \
    spectrumpenstruct.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
