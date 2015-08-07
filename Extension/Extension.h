#ifndef EXTENSION_H
#define EXTENSION_H

#include <QList>
#include <QSqlDatabase>
#include <QWidget>
#include <QIcon>
#include <QToolBar>
#include <QSqlRelationalTableModel>

#include "SchemaWidget.h"

class ExtensionTab
{
public:
    ExtensionTab(QWidget *widget, QIcon icon, QString label);
    ~ExtensionTab();

    QIcon getIcon();
    QString getLabel();
    QWidget *getWidget();

private:
    QIcon m_icon;
    QString m_label;
    QWidget *m_widget;
};

enum Capability
{
    IMPORT_DATABASE = 1,
    EXPORT_DATABASE = 2,
    CLEAR_DATABASE = 3,

    ADD_TABLE = 11,
    REMOVE_TABLE = 12,
    RENAME_TABLE = 13,

    VIEW_SCHEMA = 21,
    ADD_COLUMN = 22,
    REMOVE_COLUMN = 23,
    ALTER_COLUMN = 24
};

enum SchemaColumn
{
    NONE,
    NAME,
    TYPE,
    NULLABLE,
    DEFAULT_VALUE
};

enum ColumnFlags
{
    NO_PROPERTIES = 0,
    HAS_LENGTH = 1,
    HAS_NULLABLE = 2,
    HAS_DEFAULT = 4,
    HAS_ENUM = 5
};

class Extension : public QObject
{
Q_OBJECT

public:
    explicit Extension(QObject *parent = 0, QSqlDatabase *database = new QSqlDatabase());
    ~Extension();

    // Defines capabilities
    virtual int hasCapability(Capability capability);

    // Database functions
    virtual int importDatabase();
    virtual int exportDatabase();
    virtual int clearDatabase();

    // Table functions
    virtual int createTable(QString table);
    virtual int removeTable(QString table);
    virtual int renameTable(QString from, QString to);

    // Schema functions
    virtual QString viewSchemaQuery(QString table);
    virtual int getSchemaColumn(SchemaColumn column);
    virtual QMap<QString, int> getDataTypes();
    virtual int addColumn(QString table, QString name, QString type, int length = 0, bool nullable = false, QString defaultValue = "");
    virtual int removeColumn(QString table, QString column);
    virtual int alterColumn(QString table, QString oldName, QString newName, QString type, int length = 0, bool nullable = false, QString defaultValue = "");

    virtual void addRelations(QSqlRelationalTableModel *model);

    virtual QList<ExtensionTab*> getTabs();
    virtual QList<QToolBar*> getToolBars();

signals:
    void refreshNeeded();

protected:
    QSqlDatabase *m_database;

    QList<ExtensionTab*> m_tabs;
    QList<QToolBar*> m_toolBars;
};

#endif // EXTENSION_H
