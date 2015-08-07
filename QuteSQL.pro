#-------------------------------------------------
#
# Project created by QtCreator 2014-09-22T22:53:36
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qutesql
TEMPLATE = app

LIBS += -lz

VERSION = 0.1
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

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
    Extension/MySQLTools.cpp \
    SQLHighlighter.cpp \
    ExportSelectionDialog.cpp \
    Extension/MySQLSchemaWidget.cpp \
    QueryHistoryWidget.cpp \
    SavedQueryWidget.cpp \
    ColumnDialog.cpp \
    Database.cpp \
    Utilities/SQLSplitter.cpp \
    Tools/SQLSplitterDialog.cpp \
    Utilities/QCompressor.cpp

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
    Extension/MySQLTools.h \
    SQLHighlighter.h \
    ExportSelectionDialog.h \
    Extension/MySQLSchemaWidget.h \
    QueryHistoryWidget.h \
    SavedQueryWidget.h \
    ColumnDialog.h \
    Database.h \
    Utilities/SQLSplitter.h \
    Tools/SQLSplitterDialog.h \
    Utilities/QCompressor.h

FORMS    += MainWindow.ui \
    DatabaseConnectionWidget.ui \
    OpenConnectionDialog.ui \
    QueryWidget.ui \
    SqlWidget.ui \
    ExplorerWidget.ui \
    BrowseWidget.ui \
    SchemaWidget.ui \
    Extension/MySQLTools.ui \
    ExportSelectionDialog.ui \
    QueryHistoryWidget.ui \
    SavedQueryWidget.ui \
    ColumnDialog.ui \
    Tools/SQLSplitterDialog.ui

target.path = /usr/bin

desktop.path = /usr/share/applications/
desktop.files += $${TARGET}.desktop

INSTALLS += target desktop

OTHER_FILES += \
    README.md
