#include "BrowseWidget.h"
#include "ui_BrowseWidget.h"

#include <QSqlTableModel>

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
