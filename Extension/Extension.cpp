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

int Extension::addColumn(QString table)
{
    return false;
}

int Extension::removeColumn(QString table, QString column)
{
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

SchemaWidget * Extension::createSchemaWidget(QWidget *parent)
{
    return NULL;
}
