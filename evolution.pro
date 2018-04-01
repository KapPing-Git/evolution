#-------------------------------------------------
#
# Project created by QtCreator 2017-06-04T15:19:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = evolution
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    aliveobject.cpp \
    acommand.cpp \
    alivearena.cpp \
    aabstructliveobject.cpp \
    alivemanager.cpp \
    alivearenawidget.cpp \
    abestsurvivedwiget.cpp \
    AChart/achartseriescollection.cpp \
    AChart/achartseries.cpp \
    AChart/achartpolyline.cpp \
    AChart/achartpoint.cpp \
    AChart/achartmousephysicalcoord.cpp \
    AChart/achartaxiscollection.cpp \
    AChart/achartaxis.cpp \
    AChart/achart.cpp \
    AChart/AChartSeriesOptimisers/preseriesoptimiser.cpp \
    AChart/AChartSeriesOptimisers/aabstractseriesoptimiser.cpp \
    AChart/ACHartDevice/achartseriesdecorator.cpp \
    AChart/ACHartDevice/achartdecoratorcontainer.cpp \
    AChart/ACHartDevice/achartdecorator.cpp \
    AChart/ACHartDevice/universalDecorators/achartseriesmarker.cpp \
    AChart/ACHartDevice/universalDecorators/achartrectselector.cpp \
    AChart/AChartAxisPainter/achartaxispaintersimple.cpp \
    AChart/AChartAxisPainter/achartaxispainter.cpp \
    asourcecodeform.cpp \
    ASettings/amysettings.cpp \
    ASettings/amysettingsform.cpp \
    ASettings/acommandssettingswidget.cpp \
    ASettings/acommandsettingsform.cpp

HEADERS  += mainwindow.h \
    public.h \
    aliveobject.h \
    acommand.h \
    alivearena.h \
    aabstructliveobject.h \
    alivemanager.h \
    alivearenawidget.h \
    abestsurvivedwiget.h \
    AChart/achartseriescollection.h \
    AChart/achartseries.h \
    AChart/achartpolyline.h \
    AChart/achartpoint.h \
    AChart/achartmousephysicalcoord.h \
    AChart/achartaxiscollection.h \
    AChart/achartaxis.h \
    AChart/achart.h \
    AChart/AChartSeriesOptimisers/preseriesoptimiser.h \
    AChart/AChartSeriesOptimisers/aabstractseriesoptimiser.h \
    AChart/ACHartDevice/achartseriesdecorator.h \
    AChart/ACHartDevice/achartdecoratorcontainer.h \
    AChart/ACHartDevice/achartdecorator.h \
    AChart/ACHartDevice/universalDecorators/achartseriesmarker.h \
    AChart/ACHartDevice/universalDecorators/achartrectselector.h \
    AChart/AChartAxisPainter/achartaxispaintersimple.h \
    AChart/AChartAxisPainter/achartaxispainter.h \
    asourcecodeform.h \
    ASettings/amysettings.h \
    ASettings/amysettingsform.h \
    ASettings/acommandssettingswidget.h \
    ASettings/acommandsettingsform.h

FORMS    += mainwindow.ui \
    asourcecodeform.ui \
    ASettings/amysettingsform.ui \
    ASettings/acommandsettingsform.ui

RESOURCES += \
    res.qrc
