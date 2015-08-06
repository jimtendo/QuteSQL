#ifndef MYSQLEXTENSION_H
#define MYSQLEXTENSION_H

#include <QList>
#include <QSqlDatabase>
#include <QWidget>

#include "Extension.h"
#include "MySQLTools.h"

class MySQLExtension : public Extension
{
Q_OBJECT

public:
    explicit MySQLExtension(QObject *parent = 0, QSqlDatabase *database = new QSqlDatabase());
    ~MySQLExtension();

    SchemaWidget* createSchemaWidget(QWidget *parent);

    int hasCapability(Capability capability);

    int exportDatabase();
    int clearDatabase();

    int createTable(QString table);
    int removeTable(QString table);
    int renameTable(QString from, QString to);

    QString viewSchemaQuery(QString table);
    int getSchemaColumn(SchemaColumn column);
    QMap<QString, int> getDataTypes();
    int addColumn(QString table, QString name, QString type, int length = 0, bool nullable = false, QString defaultValue = "");
    int removeColumn(QString table, QString column);

private:
    ExtensionTab *m_toolsTab;
    QToolBar m_toolsToolBar;
    MySQLTools m_toolsWidget;
};

#endif // MYSQLEXTENSION_H
