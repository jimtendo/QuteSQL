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
    explicit MySQLExtension(QObject *parent = 0);
    ~MySQLExtension();

public slots:
    void backupDatabase();
    void restoreDatabase();

private:
    QToolBar m_toolsToolBar;
    MySQLTools m_toolsWidget;

    QAction *m_backupAction;
    QAction *m_restoreAction;
};

#endif // MYSQLEXTENSION_H
