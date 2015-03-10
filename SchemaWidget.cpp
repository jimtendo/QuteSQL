#include "SchemaWidget.h"
#include "ui_SchemaWidget.h"

SchemaWidget::SchemaWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SchemaWidget)
{
    ui->setupUi(this);
}

SchemaWidget::~SchemaWidget()
{
    delete ui;
}

void SchemaWidget::setDatabase(QSqlDatabase *database)
{
    // Set the database
    m_database = database;

    // Setup the model
    m_model = new QSqlQueryModel(this);
}

bool SchemaWidget::setTable(QString table)
{
    // Set the query from the query input box
    m_model->setQuery("DESC " + table, *m_database);

    // Attach the result table to the model
    ui->tableView->setModel(m_model);

    return true;
}
