#include "tablecreatedialog.h"
#include "ui_tablecreatedialog.h"

#include <QMessageBox>

TableCreateDialog::TableCreateDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TableCreateDialog)
{
    ui->setupUi(this);
    ui->vn_spinBox->hide();
    ui->tablename_lineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-z]+"), this));
}

TableCreateDialog::~TableCreateDialog()
{
    delete ui;
}

void TableCreateDialog::on_buttonBox_accepted()
{
    if (ui->tablename_lineEdit->text().isEmpty())
    {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", "ERROR: table name shouldn't be empty");
        messageBox.setFixedSize(500, 200);
        return;
    }

    table_name = ui->tablename_lineEdit->text();

    close();
    setResult(QDialog::Accepted);
}


void TableCreateDialog::on_buttonBox_rejected()
{
    close();
    setResult(QDialog::Rejected);
}


void TableCreateDialog::on_type_comboBox_currentIndexChanged(int index)
{
    ui->vn_spinBox->hide();
    ui->fieldname_lineEdit->setText("");
    ui->notnull_checkBox->setChecked(false);
    ui->primarykey_checkBox->setChecked(false);

    switch (index)
    {
    case 0: // INT
        break;
    case 1: // BIGSERIAL
        break;
    case 2: // DATE
        break;
    case 3: // VARCHAR
        ui->vn_spinBox->setValue(1);
        ui->vn_spinBox->setDecimals(0);
        ui->vn_spinBox->setSingleStep(1.0);
        ui->vn_spinBox->show();
        break;
    case 4: // BOOLEAN
        break;
    case 5: // NUMERIC
        ui->vn_spinBox->setValue(1);
        ui->vn_spinBox->setDecimals(1);
        ui->vn_spinBox->setSingleStep(0.1);
        ui->vn_spinBox->show();
        break;
    }
}

void TableCreateDialog::on_add_pushButton_clicked()
{
    if (ui->fieldname_lineEdit->text().isEmpty() || ui->type_comboBox->currentIndex() == -1)
    {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", "ERROR: variable name or data type shouldn't be empty");
        messageBox.setFixedSize(500, 200);
        return;
    }

    if (field_names.contains(ui->fieldname_lineEdit->text()))
    {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", "ERROR: this name has been already taken");
        messageBox.setFixedSize(500, 200);
        return;
    }

    field_names.insert(ui->fieldname_lineEdit->text());

    FieldStats field_stats;
    field_stats.name = ui->fieldname_lineEdit->text();
    field_stats.type = ui->type_comboBox->currentText();
    field_stats.is_not_null = ui->notnull_checkBox->isChecked();
    field_stats.is_primary_key = ui->primarykey_checkBox->isChecked();

    // VARCHAR or NUMERIC
    if (ui->type_comboBox->currentIndex() == 3 || ui->type_comboBox->currentIndex() == 5)
    {
        field_stats.params = QString::number(ui->vn_spinBox->value()).replace(".", ",");
    }

    fields_vec.append(field_stats);

    ui->fieldname_lineEdit->setText("");
    ui->notnull_checkBox->setChecked(false);
    ui->primarykey_checkBox->setChecked(false);
}
