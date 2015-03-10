#include "BrowseWidget.h"
#include "ui_BrowseWidget.h"

#include <QSqlTableModel>

BrowseWidget::BrowseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BrowseWidget)
{
    ui->setupUi(this);
}

BrowseWidget::~BrowseWidget()
{
    // Delete the model
    delete m_model;

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
    // Create model
    m_model->setTable(table);
    m_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_model->select();

    // Connect view to model
    ui->tableView->setModel(m_model);
    ui->tableView->show();

    return true;
}
