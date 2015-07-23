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
    m_extension(NULL),
    m_schemaWidget(NULL)
{
    ui->setupUi(this);

    // Connect actions
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
    // Set the database
    m_database = database;

    // Set the extension
    m_extension = extension;

    // Hide add button if it's not supported
    if (!m_extension || !m_extension->hasCapability(ADD_TABLE)) {
        ui->addButton->setVisible(false);
    }

    // Hide remove button if it's not supported
    if (!m_extension || !m_extension->hasCapability(REMOVE_TABLE)) {
        ui->removeButton->setVisible(false);
    }

    // Set the database for child widgets
    ui->browseTab->setDatabase(m_database);

    // Setup schema widget (if supported by extension)
    if (m_extension && m_extension->hasCapability(VIEW_SCHEMA)) {
        m_schemaWidget = new SchemaWidget(this, m_database, m_extension);
        m_schemaWidget->init();
        ui->tabWidget->addTab(m_schemaWidget, QIcon::fromTheme("document-edit"), "Schema");
        connect(m_schemaWidget, SIGNAL(refreshNeeded()), this, SLOT(refresh()));
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
}

void ExplorerWidget::on_tableListWidget_itemActivated(QListWidgetItem *item)
{
    // Set the browse tab to this table
    ui->browseTab->setTable(item->text());

    // If schema is supported, then set schema to this table
    if (m_schemaWidget) {
        m_schemaWidget->setTable(item->text());
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
    if (QMessageBox::Yes == QMessageBox::question(this, "Drop Table", "Are you sure you want to drop this table?")) {

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
