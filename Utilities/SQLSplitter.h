#ifndef SQLSPLITTER_H
#define SQLSPLITTER_H

#include <QTextStream>
#include <QString>
#include <QStringList>

class SQLSplitter
{
public:
    SQLSplitter(QIODevice *stream);
    SQLSplitter(QString *sql);

    bool atEnd();

    QString getNext(int count = 1);

    QStringList getAll(int inChunksOf = 1);

    int getPosition();
    int getLength();

private:
    QTextStream *m_stream;
    //QString m_sql;
    int m_position;
    int m_size;
};

#endif // SQLSPLITTER_H
