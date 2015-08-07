#include "Extension.h"

#include <QDebug>

ExtensionTab::ExtensionTab(QWidget *widget, QIcon icon, QString label)
{
    m_widget = widget;
    m_icon = icon;
    m_label = label;
}

ExtensionTab::~ExtensionTab()
{
}

QWidget *ExtensionTab::getWidget()
{
    return m_widget;
}

QIcon ExtensionTab::getIcon()
{
    return m_icon;
}

QString ExtensionTab::getLabel()
{
    return m_label;
}

Extension::Extension(QObject *parent, QSqlDatabase *database) :
    QObject(parent)
{
    m_database = database;
}

Extension::~Extension()
{

}

int Extension::hasCapability(Capability capability)
{
    return false;
}

int Extension::importDatabase()
{
    return false;
}

int Extension::exportDatabase()
{
    return false;
}

int Extension::clearDatabase()
{
    return false;
}

int Extension::createTable(QString table)
{
    return false;
}

int Extension::removeTable(QString table)
{
    return false;
}

int Extension::renameTable(QString from, QString to)
{
    return false;
}

QString Extension::viewSchemaQuery(QString table)
{
    return QString();
}

int Extension::getSchemaColumn(SchemaColumn column)
{
    return 0;
}

QMap<QString, int> Extension::getDataTypes()
{
    QMap<QString, int> types;

    return types;
}

int Extension::addColumn(QString table, QString name, QString type, int length, bool nullable, QString defaultValue)
{
    return false;
}

int Extension::removeColumn(QString table, QString column)
{
    return false;
}

int Extension::alterColumn(QString table, QString oldName, QString newName, QString type, int length, bool nullable, QString defaultValue)
{
    qDebug() << "Alter column not implemented.";

    return false;
}

void Extension::addRelations(QSqlRelationalTableModel *model)
{

}

QList<ExtensionTab*> Extension::getTabs()
{
    return m_tabs;
}

QList<QToolBar*> Extension::getToolBars()
{
    return m_toolBars;
}
