#include "SchemaWidget.h"
#include "ui_SchemaWidget.h"

#include <QDebug>

SchemaWidget::SchemaWidget(QWidget *parent, QSqlDatabase *database) :
    QWidget(parent),
    ui(new Ui::SchemaWidget)
{
    ui->setupUi(this);

    // Set the database
    m_database = database;
}

SchemaWidget::~SchemaWidget()
{
    delete ui;
}

void SchemaWidget::init()
{
    // Setup the model
    m_model = new QSqlQueryModel(this);
}

bool SchemaWidget::setTable(QString table)
{
    // Set current table name
    m_tableName = table;

    // Set the query from the query input box
    if (m_database->driverName() == "QSQLITE") {
        m_model->setQuery("PRAGMA table_info(" + table + ")", *m_database);
    }

    else {
        m_model->setQuery("DESC " + table, *m_database);
    }

    // Attach the result table to the model
    ui->tableView->setEnabled(true);
    ui->tableView->setModel(m_model);

    return true;
}
