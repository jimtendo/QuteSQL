#include "ColumnDialog.h"
#include "ui_ColumnDialog.h"

#include "Extension/Extension.h"

ColumnDialog::ColumnDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColumnDialog)
{
    ui->setupUi(this);


    ui->valuesTextEdit->setHidden(true);
}

ColumnDialog::~ColumnDialog()
{
    delete ui;
}

void ColumnDialog::setDataTypes(QMap<QString, int> dataTypes)
{
    QMap<QString, int>::const_iterator i = dataTypes.constBegin();
    while (i != dataTypes.constEnd()) {
        ui->typeComboBox->addItem(i.key(), QVariant(i.value()));
        ++i;
    }

}

void ColumnDialog::setName(QString name)
{
    ui->nameLineEdit->setText(name);
}

void ColumnDialog::setType(QString type)
{
    ui->typeComboBox->setCurrentIndex(ui->typeComboBox->findText(type));
}

void ColumnDialog::setLength(int length)
{
    ui->lengthLineEdit->setText(QString::number(length));
}

void ColumnDialog::setNullable(bool nullable)
{
    ui->nullableCheckBox->setChecked(nullable);
}

void ColumnDialog::setDefaultValue(QString defaultValue)
{
    ui->defaultLineEdit->setText(defaultValue);
}

QString ColumnDialog::getName()
{
    return ui->nameLineEdit->text();
}

QString ColumnDialog::getType()
{
    return ui->typeComboBox->currentText();
}

int ColumnDialog::getLength()
{
    if (!ui->lengthLineEdit->isEnabled()) {
        return false;
    }

    return ui->lengthLineEdit->text().toInt();
}

bool ColumnDialog::getNullable()
{
    if (!ui->nullableCheckBox->isEnabled()) {
        return false;
    }

    return ui->nullableCheckBox->isChecked();
}

QString ColumnDialog::getDefault()
{
    if (!ui->defaultLineEdit->isEnabled()) {
        return "";
    }

    return ui->defaultLineEdit->text();
}



void ColumnDialog::on_typeComboBox_currentIndexChanged(int index)
{
    int flags = ui->typeComboBox->itemData(index).toInt();

    if (flags & HAS_LENGTH) {
        ui->lengthLineEdit->setEnabled(true);
    } else {
        ui->lengthLineEdit->setEnabled(false);
    }

    if (flags & HAS_NULLABLE) {
        ui->nullableCheckBox->setEnabled(true);
    } else {
        ui->nullableCheckBox->setEnabled(false);
    }

    if (flags & HAS_DEFAULT) {
        ui->defaultLineEdit->setEnabled(true);
    } else {
        ui->defaultLineEdit->setEnabled(false);
    }

    if (flags & HAS_ENUM) {
        ui->valuesTextEdit->setEnabled(true);
    } else {
        ui->valuesTextEdit->setEnabled(false);
    }
}
