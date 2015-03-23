#include "BrowseWidget.h"
#include "ui_BrowseWidget.h"

#include <QDebug>

#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlError>

#include <QMessageBox>

BrowseWidget::BrowseWidget(QWidget *parent) :
    QWidget(parent),
    m_model(NULL),
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

void BrowseWidget::setDatabase(QSqlDatabase *database)
{
    // Set the database
    m_database = database;

    // Setup the model
    m_model = new QSqlTableModel(this, (*m_database));
}

bool BrowseWidget::setTable(QString table)
{
    // Call commit changes to save our new row if we have one
    if (m_model->isDirty()) {
        commitChanges();
    }

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

    return true;
}

void BrowseWidget::on_filterButton_clicked()
{
    m_model->setFilter(ui->filterEdit->text());
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
    // Set the edit model to not save
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    // Create a new record
    QSqlRecord newRecord;

    // Insert the row
    m_model->insertRecord(-1, newRecord);

    // Select the newly inserted row
    ui->tableView->selectRow(m_model->rowCount()-1);

    // Make sure we save changes when moving to a new row (this is necessary so "Add" works)
    connect(ui->tableView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(currentChanged(QModelIndex,QModelIndex)));
}

void BrowseWidget::on_clearButton_clicked()
{
    // Display messagebox for confirmation
    if (QMessageBox::Yes == QMessageBox::question(this, "Clear Table", "Are you sure you want to clear this table?\n\nThis action cannot be undone.")) {

        // Remove all rows TODO make more efficient
        for (int i = 0; i < m_model->rowCount(); i++) {
            m_model->removeRow(i);
        }

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
        }
    }
}

void BrowseWidget::currentChanged(QModelIndex previous, QModelIndex current)
{
    // If the row has changed, commit changes
    if (previous.row() != current.row()) {

        // Commit changes
        commitChanges();

        // There's no reason to call this anymore, we don't need to know if the row has changed
        disconnect(ui->tableView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(currentChanged(QModelIndex,QModelIndex)));

        // Revert back to OnFieldChange
        m_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    }
}
