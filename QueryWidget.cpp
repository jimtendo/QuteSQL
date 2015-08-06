#include "QueryWidget.h"
#include "ui_QueryWidget.h"

#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlError>

#include "ExportSelectionDialog.h"

QueryWidget::QueryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QueryWidget)
{
    // Setup UI
    ui->setupUi(this);

    // Attach SQL Highlighter
    m_highlighter = new SQLHighlighter(ui->queryEdit->document());

    // Connect history's click with fill function
    connect(ui->queryHistoryWidget, SIGNAL(querySelected(QString)), this, SLOT(setQuery(QString)));
    connect(ui->savedQueryWidget, SIGNAL(addButtonClicked()), this, SLOT(saveQuery()));
}

QueryWidget::~QueryWidget()
{
    // Delete highlighter
    delete m_highlighter;

    // Delete UI
    delete ui;
}

void QueryWidget::setDatabase(QSqlDatabase *database)
{
    // Set the database
    m_database = database;

    // Setup the model
    m_model = new QSqlQueryModel(this);

    // Setup saved query widget
    ui->savedQueryWidget->setDatabase(m_database);
}

void QueryWidget::on_runButton_clicked()
{
    // Set the query from the query input box
    m_model->setQuery(ui->queryEdit->toPlainText(), *m_database);

    // Check for errors
    if (m_model->lastError().type() != QSqlError::NoError) {
        QMessageBox::critical(this, "Could not execute query", m_model->lastError().text());
        return;
    }

    // Add to history
    ui->queryHistoryWidget->addQuery(ui->queryEdit->toPlainText());

    // Attach the result table to the model
    ui->resultTableView->setModel(m_model);

    // Enable button
    ui->exportButton->setEnabled(true);

    // Signal that a refresh is needed
    emit refreshNeeded();
}

void QueryWidget::on_clearButton_clicked()
{
    // Clear the query text
    ui->queryEdit->clear();
}

void QueryWidget::on_exportButton_clicked()
{
    // Create export selection dialog
    ExportSelectionDialog exportSelectionDialog;

    // Initialise the dialog
    exportSelectionDialog.init(ui->resultTableView);

    // Execute the dialog
    exportSelectionDialog.exec();
}

void QueryWidget::saveQuery()
{
    ui->savedQueryWidget->addQuery(ui->queryEdit->toPlainText());
}

void QueryWidget::setQuery(QString query)
{
    ui->queryEdit->setText(query);
}
