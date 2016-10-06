#ifndef EXPLORERWIDGET_H
#define EXPLORERWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QListWidgetItem>

#include "Extension/Extension.h"
#include "BrowseWidget.h"

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
    void openInNewTab();
    void addTable();
    void removeTable();
    void renameTable();
    void refresh();

private slots:
    void on_tableListWidget_itemActivated(QListWidgetItem *item);
    void on_tableListWidget_customContextMenuRequested(const QPoint &pos);

    void on_browseWidgets_tabCloseRequested(int index);

private:
    Ui::ExplorerWidget *ui;

    // Database
    QSqlDatabase *m_database;

    // Extension
    Extension *m_extension;

    // Browse Widgets
    QList<BrowseWidget> m_browseWidgets;
};

#endif // EXPLORERWIDGET_H
