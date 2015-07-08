#ifndef EXPLORERWIDGET_H
#define EXPLORERWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QListWidgetItem>

#include "Extension/Extension.h"

namespace Ui {
class ExplorerWidget;
}

class ExplorerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExplorerWidget(QWidget *parent = 0);
    ~ExplorerWidget();

    void init(QSqlDatabase *database, Extension *extension = NULL);

public slots:
    void addTable();
    void removeTable();
    void renameTable();
    void refresh();

private slots:
    void on_tableListWidget_itemActivated(QListWidgetItem *item);
    void on_tableListWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::ExplorerWidget *ui;

    // Database
    QSqlDatabase *m_database;

    // Extension
    Extension *m_extension;

    // Schema Widget
    SchemaWidget *m_schemaWidget;
};

#endif // EXPLORERWIDGET_H
