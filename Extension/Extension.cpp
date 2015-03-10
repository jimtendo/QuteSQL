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

QList<ExtensionTab*> Extension::getTabs()
{
    return m_tabs;
}

QList<QToolBar*> Extension::getToolBars()
{
    return m_toolBars;
}
