######################################################################
# Automatically generated by qmake (3.1) Mon Feb 12 14:35:35 2024
######################################################################

TEMPLATE = app
TARGET = venice-qt
INCLUDEPATH += .

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -lprotobuf

# Input
HEADERS += \
    include/channel/datachannel.h \
    include/datachannelevents.h \
    include/exception/errorstartingfiletransferserviceveniceexception.h \
    external/protobuf/cpp_proto/venice.pb.h \
    include/service/filetransferservice.h \
    include/ui/mainwindow.h \
    include/exception/notavailableportfoundveniceexception.h \
    include/exception/notbluetoothadapterfoundveniceexception.h \
    include/exception/notsuitablewifiadapterfoundveniceexception.h \
    include/service/venicebluetoothuuid.h \
    include/exception/veniceexception.h \
    include/network/venicemessage.h \
    include/service/veniceservice.h \
    include/event/venicetimer.h \
    include/channel/wifidatachannel.h
FORMS += \
    ui/mainwindow.ui
SOURCES += src/main.cpp \
    src/channel/datachannel.cpp \
    src/exception/errorstartingfiletransferserviceveniceexception.cpp \
    external/protobuf/cpp_proto/venice.pb.cc \
    src/service/filetransferservice.cpp \
    src/ui/mainwindow.cpp \
    src/exception/notavailableportfoundveniceexception.cpp \
    src/exception/notbluetoothadapterfoundveniceexception.cpp \
    src/exception/notsuitablewifiadapterfoundveniceexception.cpp \
    src/service/venicebluetoothuuid.cpp \
    src/exception/veniceexception.cpp \
    src/network/venicemessage.cpp \
    src/service/veniceservice.cpp \
    src/event/venicetimer.cpp \
    src/channel/wifidatachannel.cpp
TRANSLATIONS += venice-qt_en_US.ts
QT += widgets bluetooth core network
CONFIG += c++23
# DEFINES += QT_NO_DEBUG_OUTPUT
