#include "QueryWidget.h"
#include "ui_QueryWidget.h"

#include <QSqlQueryModel>

QueryWidget::QueryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QueryWidget)
{
    // Setup UI
    ui->setupUi(this);
}

QueryWidget::~QueryWidget()
{
    // Delete UI
    delete ui;
}

void QueryWidget::setDatabase(QSqlDatabase database)
{
    // Set the database
    m_database = database;

    // Setup the model
    m_model = new QSqlQueryModel(this);
}

void QueryWidget::on_runButton_clicked()
{
    // Set the query from the query input box
    m_model->setQuery(ui->queryEdit->toPlainText(), m_database);

    // Attach the result table to the model
    ui->resultTableView->setModel(m_model);
}

void QueryWidget::on_clearButton_clicked()
{
    // Clear the query text
    ui->queryEdit->clear();
}
