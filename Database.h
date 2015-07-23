#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>

class Query
{
public:
    Query();

private:
    QString m_query;
    QString m_datetime;
};

class Database : public QSqlDatabase
{
public:
    Database();

    QString getName();

    QSqlQuery executeQuery(QString query);

    QList<Query> getSavedQueries();
    QList<Query> getHistoricalQueries();
};

#endif // DATABASE_H
