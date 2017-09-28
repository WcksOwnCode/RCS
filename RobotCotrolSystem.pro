#-------------------------------------------------
#
# Project created by QtCreator 2017-08-30T21:04:10
#
#-------------------------------------------------

QT       += core gui
QT +=axcontainer
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RobotCotrolSystem
TEMPLATE = app
QT       += serialport
QT += multimedia
QT += multimediawidgets

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
    curvefit.cpp \
    funcitons.cpp \
    matrix_4.cpp

HEADERS  += mainwindow.h \
    curvefit.h \
    funcitons.h \
    matrix_4.h

FORMS    += mainwindow.ui
SUBDIRS += \
    qextserialport-1.2win-alpha/qextserialport.pro
INCLUDEPATH += D:\opencv\opencv\build\include

CONFIG(debug, debug|release): {
LIBS += -LD:\opencv\opencv\build\x86\vc12\lib\
-lopencv_core2413d \
-lopencv_imgproc2413d \
-lopencv_highgui2413d \
-lopencv_ml2413d \
-lopencv_video2413d \
-lopencv_features2d2413d \
-lopencv_calib3d2413d \
-lopencv_objdetect2413d \
-lopencv_contrib2413d \
-lopencv_legacy2413d \
-lopencv_flann2413d
} else:CONFIG(release, debug|release): {
LIBS += -LD:\opencv\opencv\build\x86\vc12\lib\
-lopencv_core2413 \
-lopencv_imgproc2413 \
-lopencv_highgui2413 \
-lopencv_ml2413 \
-lopencv_video2413 \
-lopencv_features2d2413 \
-lopencv_calib3d2413 \
-lopencv_objdetect2413 \
-lopencv_contrib2413 \
-lopencv_legacy2413 \
-lopencv_flann2413
}
