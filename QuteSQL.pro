#-------------------------------------------------
#
# Project created by QtCreator 2014-09-22T22:53:36
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QuteSQL
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    DatabaseConnectionWidget.cpp \
    OpenConnectionDialog.cpp \
    QueryWidget.cpp \
    SqlWidget.cpp \
    ExplorerWidget.cpp \
    BrowseWidget.cpp \
    SchemaWidget.cpp \
    Extension/MySQLExtension.cpp \
    Extension/Extension.cpp \
    Extension/MySQLTools.cpp

HEADERS  += MainWindow.h \
    DatabaseConnectionWidget.h \
    OpenConnectionDialog.h \
    QueryWidget.h \
    SqlWidget.h \
    ExplorerWidget.h \
    BrowseWidget.h \
    SchemaWidget.h \
    Extension/Extension.h \
    Extension/MySQLExtension.h \
    Extension/MySQLTools.h

FORMS    += MainWindow.ui \
    DatabaseConnectionWidget.ui \
    OpenConnectionDialog.ui \
    QueryWidget.ui \
    SqlWidget.ui \
    ExplorerWidget.ui \
    BrowseWidget.ui \
    SchemaWidget.ui \
    Extension/MySQLTools.ui
