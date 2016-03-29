#-------------------------------------------------
#
# Project created by QtCreator 2016-03-04T18:25:19
#
#-------------------------------------------------

QT       += core gui
CONFIG   += C++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sst
TEMPLATE = app

HEADERS  += \
    aboutdialog.h \
    hotkeyeventfilter.h \
    mainwidget.h \
    settings.h \
    settingsdialog.h

SOURCES += main.cpp\
    aboutdialog.cpp \
    hotkeyeventfilter.cpp \
    mainwidget.cpp \
    settings.cpp \
    settingsdialog.cpp

FORMS += \
    aboutdialog.ui \
    settingsdialog.ui

RESOURCES += \
    res.qrc

TRANSLATIONS += \
    sst_ru.ts

debug {
    OBJECTS_DIR = "debug_tmp"
    MOC_DIR     = "debug_tmp"
    RCC_DIR     = "debug_tmp"
  }

release {
    OBJECTS_DIR = "release_tmp"
    MOC_DIR     = "release_tmp"
    RCC_DIR     = "release_tmp"
  }

win32 {
    DEFINES += HOTKEY_ENABLED
    RC_FILE = RC_FILE.rc
    DISTFILES += RC_FILE.rc
  }
