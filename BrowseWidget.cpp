#include "BrowseWidget.h"
#include "ui_BrowseWidget.h"

#include <QDebug>

#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlError>

#include "InsertRowDialog.h"

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
    if (m_model) {
        delete m_model;
    }

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

    // Make sure we save changes when moving to a new row (this is necessary so "Add" works)
    connect(ui->tableView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(commitChanges()));

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
}

void BrowseWidget::on_clearButton_clicked()
{
    // Remove all rows
    m_model->removeRows(1, m_model->rowCount()-1);

    // Refresh browse widget
    m_model->select();
}

void BrowseWidget::commitChanges()
{
    // Submit all previous changes
    if (m_model->isDirty()) {
        m_model->submitAll();
    }

    // Revert back to OnFieldChange
    //m_model->setEditStrategy(QSqlTableModel::OnFieldChange);
}
