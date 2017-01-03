#-------------------------------------------------
#
# Project created by QtCreator 2016-09-18T21:26:15
#
#-------------------------------------------------

QT       += core gui
QT += sql
QT += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GeneralSql
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog_add_rule.cpp \
    databasesqlite.cpp \
    dialog_modi_rule.cpp \
    uidata.cpp \
    g_def.cpp

HEADERS  += mainwindow.h \
    dialog_add_rule.h \
    databasesqlite.h \
    dialog_modi_rule.h \
    uidata.h \
    g_def.h

FORMS    += mainwindow.ui \
    dialog_add_rule.ui \
    dialog_modi_rule.ui

RESOURCES += \
    start.qrc
