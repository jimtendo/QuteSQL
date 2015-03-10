#ifndef EXTENSION_H
#define EXTENSION_H

#include <QList>
#include <QSqlDatabase>
#include <QWidget>
#include <QIcon>
#include <QToolBar>

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

class Extension : public QObject
{
Q_OBJECT

public:
    explicit Extension(QObject *parent = 0);
    ~Extension();

    void setDatabase(QSqlDatabase *database);

    virtual QList<ExtensionTab*> getTabs();
    virtual QList<QToolBar*> getToolBars();

protected:
    QSqlDatabase *m_database;

    QList<ExtensionTab*> m_tabs;
    QList<QToolBar*> m_toolBars;
};

#endif // EXTENSION_H
