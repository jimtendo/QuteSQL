#include "SchemaWidget.h"
#include "ui_SchemaWidget.h"

#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>

#include "ColumnDialog.h"

SchemaWidget::SchemaWidget(QWidget *parent, QSqlDatabase *database, Extension *extension) :
    QWidget(parent),
    ui(new Ui::SchemaWidget)
{
    ui->setupUi(this);

    // Set the database
    m_database = database;

    // Set the extension
    m_extension = extension;

    connect(ui->addColumnButton, SIGNAL(clicked()), this, SLOT(addColumn()));
    connect(ui->removeColumnButton, SIGNAL(clicked()), this, SLOT(removeColumn()));
}

SchemaWidget::~SchemaWidget()
{
    delete ui;
}

void SchemaWidget::init()
{
    // Setup the model
    m_model = new QSqlQueryModel(this);

    // Hide add column if not supported
    if (!m_extension || !m_extension->hasCapability(ADD_COLUMN)) {
        ui->addColumnButton->setVisible(false);
    }

    // Hide remove column if not supported
    if (!m_extension || !m_extension->hasCapability(REMOVE_COLUMN)) {
        ui->removeColumnButton->setVisible(false);
    }
}

bool SchemaWidget::setTable(QString table)
{
    // Set current table name
    m_tableName = table;

    // Get the schema
    m_model->setQuery(m_extension->viewSchemaQuery(table), *m_database);

    // Set the query from the query input box
    if (m_database->driverName() == "QSQLITE") {
        m_model->setQuery("PRAGMA table_info(" + table + ")", *m_database);
    }

    // Attach the result table to the model
    ui->tableView->setEnabled(true);
    ui->tableView->setModel(m_model);

    return true;
}

void SchemaWidget::refresh()
{
    setTable(m_tableName);
}

void SchemaWidget::addColumn()
{
    // Show add column dialog
    ColumnDialog columnDialog;
    columnDialog.setDataTypes(m_extension->getDataTypes());
    columnDialog.exec();

    // Actually add the column
    m_extension->addColumn(m_tableName, columnDialog.getName(), columnDialog.getType(), columnDialog.getLength(),
                           columnDialog.getNullable(), columnDialog.getDefault());

    // Refresh schema
    refresh();

    // Emit refresh needed
    emit refreshNeeded();
}

void SchemaWidget::removeColumn()
{
    // Display messagebox for confirmation
    if (QMessageBox::Yes == QMessageBox::question(this, "Drop Column", "Are you sure you want to remove this column?")) {

        // Get column name
        QModelIndex index = ui->tableView->model()->index(ui->tableView->currentIndex().row(), m_extension->getSchemaColumn(NAME));
        QString column = ui->tableView->model()->data(index).toString();

        // Drop column
        m_extension->removeColumn(m_tableName, column);

        // Refresh schema
        refresh();

        // Emit refresh needed
        emit refreshNeeded();
    }
}

void SchemaWidget::on_tableView_activated(const QModelIndex &index)
{
    // Only open dialog if supports altering column
    if (!m_extension->hasCapability(ALTER_COLUMN)) {
        return;
    }

    // Get column name
    QModelIndex nameIndex = ui->tableView->model()->index(ui->tableView->currentIndex().row(), m_extension->getSchemaColumn(NAME));
    QString name = ui->tableView->model()->data(nameIndex).toString();

    // Get column type
    QModelIndex typeIndex = ui->tableView->model()->index(ui->tableView->currentIndex().row(), m_extension->getSchemaColumn(TYPE));
    QString type = ui->tableView->model()->data(typeIndex).toString();
    qDebug() << type;

    // Extract parameters from type
    QRegularExpression bracketsExp("/\((.*?)\)/");
    QString parameters = bracketsExp.match(type).captured();
    qDebug() << parameters;

    // Remove parameters from type
    //type.remove()

    // Extract length from type
    QRegularExpression lengthExp("[0-9]+");
    int length = lengthExp.match(type).captured().toInt();
    qDebug() << length;

    // Remove digits and other characters so that only type remains
    type.remove(QRegularExpression("[^A-Za-z ]"));
    qDebug() << type;

    // Get nullable
    QModelIndex nullableIndex = ui->tableView->model()->index(ui->tableView->currentIndex().row(), m_extension->getSchemaColumn(NULLABLE));
    bool nullable = (ui->tableView->model()->data(nullableIndex).toString() == "YES") ? true : false;

    // Get default value
    QModelIndex defaultValueIndex = ui->tableView->model()->index(ui->tableView->currentIndex().row(), m_extension->getSchemaColumn(DEFAULT_VALUE));
    QString defaultValue = ui->tableView->model()->data(defaultValueIndex).toString();

    // Show column dialog
    ColumnDialog columnDialog;
    columnDialog.setDataTypes(m_extension->getDataTypes());
    columnDialog.setName(name);
    columnDialog.setType(type);
    columnDialog.setLength(length);
    columnDialog.setNullable(nullable);
    columnDialog.setDefaultValue(defaultValue);
    columnDialog.exec();

    // Actually add the column
    m_extension->alterColumn(m_tableName, name, columnDialog.getName(), columnDialog.getType(), columnDialog.getLength(),
                           columnDialog.getNullable(), columnDialog.getDefault());

    // Refresh schema
    refresh();

    // Emit refresh needed
    emit refreshNeeded();
}
