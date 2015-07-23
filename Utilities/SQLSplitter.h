#ifndef SQLSPLITTER_H
#define SQLSPLITTER_H

#include <QString>
#include <QStringList>

class SQLSplitter
{
public:
    SQLSplitter(QString sql);

    bool atEnd();

    QString getNext(int count = 1);

    QStringList getAll(int inChunksOf = 1);

    int getPosition();
    int getLength();

private:
    QString m_sql;
    int m_position;
};

#endif // SQLSPLITTER_H
