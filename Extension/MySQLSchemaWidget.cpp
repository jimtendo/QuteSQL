#include "MySQLSchemaWidget.h"
#include "ui_SchemaWidget.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

MySQLSchemaModel::MySQLSchemaModel(QObject *parent, QSqlDatabase *database)
{
    m_database = database;
}

Qt::ItemFlags MySQLSchemaModel::flags(const QModelIndex &index) const
{
    if (!index.isValid() || index.column() >= 3)
        return Qt::ItemIsEnabled;
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool MySQLSchemaModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole && !(index.row() < 0))
    {
        int row = index.row();

        // Name
        QModelIndex nameIndex = this->index(row, 0, QModelIndex());
        QString oldName = data(nameIndex).toString();
        QString newName = data(nameIndex).toString();

        // Type
        QModelIndex typeIndex = this->index(row, 1, QModelIndex());
        QString type = data(typeIndex).toString();

        // Nullable
        QModelIndex nullableIndex = this->index(row, 2, QModelIndex());
        QString nullable = data(nullableIndex).toString();
        (nullable == "YES") ? nullable = "NULL" : nullable = "NOT NULL";

        switch(index.column())
        {
            case 0:
                newName = value.toString();
                break;
            case 1:
                type = value.toString();
                break;
            case 2:
                nullable = value.toString();
                (nullable == "YES") ? nullable = "NULL" : nullable = "NOT NULL";
                break;
            default:
                return false;
        }

        QSqlQuery query = m_database->exec("ALTER TABLE " + m_tableName + " CHANGE `" + oldName + "` `" + newName + "` " + type + " " + nullable);
        if (query.lastError().type() != QSqlError::NoError) {
            qDebug() << query.lastError().databaseText();
        }

        setTable(m_tableName);

        return true;
    }

    return false;
}

void MySQLSchemaModel::setTable(QString table)
{
    m_tableName = table;

    setQuery("DESC " + m_tableName, *m_database);
}

MySQLSchemaWidget::MySQLSchemaWidget(QWidget *parent, QSqlDatabase *database) :
    SchemaWidget(parent, database)
{

}

void MySQLSchemaWidget::init()
{
    // Setup the model
    m_model = new MySQLSchemaModel(this, m_database);
}

bool MySQLSchemaWidget::setTable(QString table)
{
    // Set current table name
    m_tableName = table;

    MySQLSchemaModel* model = (MySQLSchemaModel*)m_model;
    model->setTable(table);

    // Attach the result table to the model
    ui->tableView->setEnabled(true);
    ui->tableView->setModel(m_model);

    return true;
}
