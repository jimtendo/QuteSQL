#ifndef COLUMNDIALOG_H
#define COLUMNDIALOG_H

#include <QDialog>
#include <QMap>

namespace Ui {
class ColumnDialog;
}

class ColumnDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColumnDialog(QWidget *parent = 0);
    ~ColumnDialog();

    void setDataTypes(QMap<QString, int> dataTypes);

    void setName(QString name);
    void setType(QString type);
    void setLength(int length);
    void setNullable(bool nullable);
    void setDefaultValue(QString defaultValue);

    QString getName();
    QString getType();
    int getLength();
    bool getNullable();
    QString getDefault();

private slots:
    void on_typeComboBox_currentIndexChanged(int index);

private:
    Ui::ColumnDialog *ui;
};

#endif // COLUMNDIALOG_H
