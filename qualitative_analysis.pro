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
    service.cpp \
    bar_clock.cpp \
    main_window.cpp \
    single_window.cpp \
    spectrum_chart.cpp \
    spectrum_list_model.cpp \
    spectrum_spm.cpp \
    tab_spec_window.cpp \
    work_area_view.cpp \
    exception.cpp \
    spectrum_list_view.cpp \
    spectrum_list_mime_data.cpp \
    tab_bar_spec_window.cpp

HEADERS += \
    service.h \
    bar_clock.h \
    main_window.h \
    single_window.h \
    spectrum_chart.h \
    spectrum_chart_axis_modes.h \
    spectrum_list_model.h \
    spectrum_pen_struct.h \
    spectrum_spm.h \
    tab_spec_window.h \
    work_area_view.h \
    spectrum_attribute_struct.h \
    exception.h \
    spectrum_list_view.h \
    spectrum_list_mime_data.h \
    tab_bar_spec_window.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
