#include "ExplorerWidget.h"
#include "ui_ExplorerWidget.h"

#include <QListWidgetItem>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>
#include <QMenu>

ExplorerWidget::ExplorerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExplorerWidget),
    m_extension(NULL)
{
    ui->setupUi(this);

    // Connect actions
    connect(ui->actionOpen_In_New_Tab, SIGNAL(triggered(bool)), this, SLOT(openInNewTab()));
    connect(ui->actionAdd_Table, SIGNAL(triggered()), this, SLOT(addTable()));
    connect(ui->actionRemove_Table, SIGNAL(triggered()), this, SLOT(removeTable()));
    connect(ui->actionRename_Table, SIGNAL(triggered()), this, SLOT(renameTable()));

    // Connect buttons
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addTable()));
    connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(removeTable()));
}

ExplorerWidget::~ExplorerWidget()
{
    delete ui;
}

void ExplorerWidget::init(QSqlDatabase *database, Extension *extension)
{
    // Set the database and extension
    m_database = database;
    m_extension = extension;

    // Hide add button if it's not supported
    if (!m_extension || !m_extension->hasCapability(ADD_TABLE)) {
        ui->addButton->setVisible(false);
    }

    // Hide remove button if it's not supported
    if (!m_extension || !m_extension->hasCapability(REMOVE_TABLE)) {
        ui->removeButton->setVisible(false);
    }

    // Refresh table list
    refresh();
}

void ExplorerWidget::refresh()
{
    // Clear list of tables
    ui->tableListWidget->clear();

    // Repopulate list of tables
    foreach (QString table, m_database->tables()) {
        QListWidgetItem *tableItem = new QListWidgetItem(QIcon::fromTheme("folder"), table);
        ui->tableListWidget->addItem(tableItem);
    }

    // Remove Browse Tabs that no longer have corresponding tables
    // @TODO
}

void ExplorerWidget::on_tableListWidget_itemActivated(QListWidgetItem *item)
{
    // Get the currently active browse widget
    BrowseWidget *activeWidget = (BrowseWidget*)ui->browseWidgets->currentWidget();

    // If there is none, create it
    if (!activeWidget) {
        // Create default browse widget and add it to browse widgets
        activeWidget = new BrowseWidget;
        activeWidget->init(m_database, m_extension);
        ui->browseWidgets->addTab(activeWidget, QIcon::fromTheme("server-database"), "Default");
        ui->browseWidgets->setCurrentWidget(activeWidget);
    }

    // Set the table and tabb text
    activeWidget->setTable(item->text());
    ui->browseWidgets->setTabText(ui->browseWidgets->currentIndex(), item->text());
}

void ExplorerWidget::openInNewTab()
{
    // If there is a table selected...
    if (ui->tableListWidget->currentItem()) {

        // Get table name
        QString table = ui->tableListWidget->currentItem()->text();

        // Create browse widget
        BrowseWidget *browseWidget = new BrowseWidget;
        browseWidget->init(m_database, m_extension);
        browseWidget->setTable(table);

        // Create icon and this to the list of browse widget tabs
        ui->browseWidgets->addTab(browseWidget, QIcon::fromTheme("server-database"), table);
        ui->browseWidgets->setCurrentWidget(browseWidget);
    }
}

void ExplorerWidget::addTable()
{
    // Get table name
    QString table = QInputDialog::getText(this, "Create Table", "Enter table name");

    // Create table if extension supports it
    m_extension->createTable(table);

    // Refresh explorer
    refresh();
}

void ExplorerWidget::removeTable()
{
    // Display messagebox for confirmation
    if (ui->tableListWidget->currentItem() && QMessageBox::Yes == QMessageBox::question(this, "Drop Table", "Are you sure you want to drop this table?")) {

        // Get table name
        QString table = ui->tableListWidget->currentItem()->text();

        // Drop table
        m_extension->removeTable(table);

        // Refresh explorer
        refresh();
    }
}

void ExplorerWidget::renameTable()
{
    // Get old table name
    QString from = ui->tableListWidget->currentItem()->text();

    // Get new table name
    QString to = QInputDialog::getText(this, "Rename table", "Enter new table name");

    // Rename table
    m_extension->renameTable(from, to);

    // Refresh explorer
    refresh();
}

void ExplorerWidget::on_tableListWidget_customContextMenuRequested(const QPoint &pos)
{
    // Setup context menu
    QMenu *menu = new QMenu(this);

    // Add button to open in new tab
    menu->addAction(ui->actionOpen_In_New_Tab);

    // If our extension can add tables
    if (m_extension && m_extension->hasCapability(ADD_TABLE)) {
        menu->addAction(ui->actionAdd_Table);
    }

    // If our extension can remove tables
    if (m_extension && m_extension->hasCapability(REMOVE_TABLE)) {
        menu->addAction(ui->actionRemove_Table);
    }

    // If our extension can rename tables
    if (m_extension && m_extension->hasCapability(RENAME_TABLE)) {
        menu->addAction(ui->actionRename_Table);
    }

    // Show the menu
    menu->popup(ui->tableListWidget->viewport()->mapToGlobal(pos));
}

void ExplorerWidget::on_browseWidgets_tabCloseRequested(int index)
{
    // Get a pointer to the tab instance so we can delete it later (at the end of this function)
    BrowseWidget *widget = (BrowseWidget*)ui->browseWidgets->widget(index);

    // Remove the tab
    ui->browseWidgets->removeTab(index);

    // Actually delete the widget
    delete widget;
}
