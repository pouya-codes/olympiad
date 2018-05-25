#-------------------------------------------------
#
# Project created by QtCreator 2017-04-21T11:26:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = olmpiad
TEMPLATE = app
CONFIG += c++11

unix {
unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv
LIBS +=   -lboost_system \
-lboost_filesystem

}
win32 {
INCLUDEPATH += include

LIBS += D:\Develop\QT\OMR\lib\libopencv_core2413.dll
LIBS += D:\Develop\QT\OMR\lib\libopencv_highgui2413.dll
LIBS += D:\Develop\QT\OMR\lib\libopencv_imgproc2413.dll
}


SOURCES += main.cpp\
        mainwindow.cpp \
    setting.cpp \
    roi_selecter.cpp \
    barcodereader.cpp \
    barcodedetector.cpp \
    filecollector.cpp \
    omitemptys.cpp

HEADERS  += mainwindow.h \
            asmOpenCV.h \
    setting.h \
    roi_selecter.h \
    barcodereader.h \
    barcodedetector.h \
    filecollector.h \
    omitemptys.h

FORMS    += mainwindow.ui



include(QZXing/QZXing.pri)


