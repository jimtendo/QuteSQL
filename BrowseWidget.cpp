#include "BrowseWidget.h"
#include "ui_BrowseWidget.h"

#include <QDebug>

#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlError>
#include <QSqlQuery>

#include <QMessageBox>

#include "ExportSelectionDialog.h"

BrowseWidget::BrowseWidget(QWidget *parent) :
    QWidget(parent),
    m_model(NULL),
    m_schemaWidget(NULL),
    ui(new Ui::BrowseWidget)
{
    ui->setupUi(this);
}

BrowseWidget::~BrowseWidget()
{
    // Delete the model
    if (m_model) delete m_model;

    // Delete the user interface
    delete ui;
}

void BrowseWidget::init(QSqlDatabase *database, Extension *extension)
{
    // Set the database and extension
    m_database = database;
    m_extension = extension;

    // Setup the model
    m_model = new QSqlTableModel(this, (*m_database));

    // Make sure we save changes when moving to a new row (this is necessary so "Add" works)
    connect(ui->tableView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(currentChanged(QModelIndex,QModelIndex)));

    // Setup schema widget (if supported by extension)
    if (m_extension && m_extension->hasCapability(VIEW_SCHEMA)) {
        m_schemaWidget = new SchemaWidget(this, m_database, m_extension);
        m_schemaWidget->init();
        ui->tabWidget->addTab(m_schemaWidget, QIcon::fromTheme("document-edit"), "Schema");
        //connect(m_schemaWidget, SIGNAL(refreshNeeded()), this, SLOT(refresh())); // @TODO make this refresh
    }
}

bool BrowseWidget::setTable(QString table)
{
    // Call commit changes to save our new row if we have one
    if (m_model->isDirty()) {
        commitChanges();
    }

    // Set new table
    m_table = table;

    // Create model
    m_model->setTable(table);
    m_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_model->select();

    // Connect view to model
    ui->tableView->setModel(m_model);
    ui->tableView->setEnabled(true);
    ui->tableView->show();

    // Enable filter
    ui->filterEdit->setEnabled(true);
    ui->filterButton->setEnabled(true);

    // Enable Add/Remove/Clear/SaveAs
    ui->addButton->setEnabled(true);
    ui->removeButton->setEnabled(true);
    ui->clearButton->setEnabled(true);
    ui->saveAsButton->setEnabled(true);

    // Neaten it up
    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(0, Qt::AscendingOrder);
    ui->tableView->setAlternatingRowColors(true);

    // If there is a schema-editor, then set the table
    if (m_schemaWidget) {
        m_schemaWidget->setTable(table);
    }

    return true;
}

void BrowseWidget::on_filterButton_clicked()
{
    // Set filter and then select (otherwise it doesn't always work)
    m_model->setFilter(ui->filterEdit->text());
    m_model->select();
}

void BrowseWidget::on_removeButton_clicked()
{
    // Get list of selected indexes
    QModelIndexList indexes = ui->tableView->selectionModel()->selectedIndexes();

    // Delete each index
    Q_FOREACH(QModelIndex index, indexes) {
        m_model->removeRow(index.row());
    }

    // Refresh browse widget
    m_model->select();
}

void BrowseWidget::on_addButton_clicked()
{
    // Set the edit model to not save (only change if not already set, otherwise it crashes)
    if (m_model->editStrategy() != QSqlTableModel::OnManualSubmit) {
        m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    }

    // Create a new record
    QSqlRecord newRecord;

    // Insert the row
    m_model->insertRecord(-1, newRecord);

    // Select the newly inserted row
    ui->tableView->selectRow(m_model->rowCount()-1);
}

void BrowseWidget::on_clearButton_clicked()
{
    // Display messagebox for confirmation
    if (QMessageBox::Yes == QMessageBox::question(this, "Clear Table", "Are you sure you want to clear this table?")) {

        // Remove all rows (TODO Will this work across all SQL implementations?)
        m_database->exec("DELETE FROM " + m_table);

        // Refresh browse widget
        m_model->select();
    }
}

void BrowseWidget::commitChanges()
{
    // Submit all previous changes
    if (m_model->isDirty()) {
        m_model->submitAll();

        // Check if there was an error and display it if there was
        if (m_model->lastError().type() != QSqlError::NoError) {
            QMessageBox::critical(this, "Operation failed", m_model->lastError().text());

            // Revert changes and set back to onFieldChange
            m_model->revertAll();
        }
    }
}

void BrowseWidget::currentChanged(QModelIndex previous, QModelIndex current)
{
    // If the row has changed, commit changes
    if (previous.row() != current.row()) {

        // Commit changes
        commitChanges();

        // Revert back to OnFieldChange
        m_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    }
}

void BrowseWidget::on_saveAsButton_clicked()
{
    // Create export selection dialog
    ExportSelectionDialog exportSelectionDialog;

    // Initialise the dialog
    exportSelectionDialog.init(ui->tableView);

    // Execute the dialog
    exportSelectionDialog.exec();
}

void BrowseWidget::refresh()
{
    // If there is a schema widget (extension supports it), refresh it
    /*if (m_schemaWidget) {
        m_schemaWidget->setTable(m_table);
    }*/
    // @TODO this can cause errors if the table no longer exists
}
