#-------------------------------------------------
#
# Project created by QtCreator 2018-04-02T09:34:00
#
#-------------------------------------------------

QT       += core gui sql  network axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SMDM
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


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    bdata.cpp \
    micran_gen.cpp \
    n9000a.cpp \
    powersourse.cpp \
    tp_smdm.cpp \
    mainwindow_log.cpp \
    qcustomplot.cpp \
    mainwindow_imagetp.cpp \
    moydialog.cpp \
    regylirovka.cpp \
    mainwindow_pdf.cpp \
    thread_sm.cpp \
    thread_dm.cpp \
    thread_smdm.cpp \
    mainwindow_start.cpp \
    log.cpp \
    mainwindow_updategraph.cpp \
    mainwindow_updategraphregylirovka.cpp \
    mainwindow_loadresult.cpp

HEADERS += \
        mainwindow.h \
    bdata.h \
    micran_gen.h \
    n9000a.h \
    powersourse.h \
    tp_smdm.h \
    qcustomplot.h \
    moydialog.h \
    regylirovka.h \
    thread_sm.h \
    thread_dm.h \
    thread_smdm.h \
    log.h

FORMS += \
        mainwindow.ui \
    moydialog.ui \
    regylirovka.ui \
    power.ui \
    electricalpower.ui \
    frequency.ui \
    switchingsignaldm.ui \
    switchingsignalsm.ui \
    switchingsignalsmdm.ui \
    transfercoefficient.ui

LIBS += -L$$PWD/ -lMiVISA32

RESOURCES += \
    resources.qrc
