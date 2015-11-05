#include "SQLSplitter.h"

#include <QDebug>

SQLSplitter::SQLSplitter(QIODevice *stream)
{
    m_size = stream->size();
    m_stream = new QTextStream(stream);
    m_position = 0;
}

SQLSplitter::SQLSplitter(QString *sql)
{
    m_size = sql->length();
    m_stream = new QTextStream(sql);
    //m_sql = sql;
    m_position = 0;
}

bool SQLSplitter::atEnd()
{
    if (m_position == m_size-1) {
        return true;
    }

    return false;
}

QString SQLSplitter::getNext(int count)
{
    QString statements;

    for (int i = 0; i < count && !atEnd(); i++) {

        bool endFound = false;
        QChar current;
        QChar previous;
        unsigned int quoteCount = 0;

        do {
            if (atEnd()) {
                break;
            }

            current = m_stream->read(1)[0];

            if (current == '\'' && previous != '\\'){
                ++quoteCount;
            } else if (current == ';' && previous != '\\' && (quoteCount % 2) == 0) {
                endFound = true;
            }

            statements += current;
            previous = current;

            m_position++;

            //qDebug() << m_stream->pos() << ':' << m_position;

        } while (!endFound && m_position != m_size);

    }

    return statements;
}

QStringList SQLSplitter::getAll(int inChunksOf)
{
    QStringList allStatements;

    while (!atEnd()) {
        allStatements.append(getNext(inChunksOf));
    }

    return allStatements;
}

int SQLSplitter::getPosition()
{
    return m_position;
}

int SQLSplitter::getLength()
{
    return m_size;
}
