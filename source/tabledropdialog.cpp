#include "tabledropdialog.h"
#include "ui_tabledropdialog.h"

#include <QMessageBox>

TableDropDialog::TableDropDialog(const QSet<QString>& tables_names, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TableDropDialog)
{
    ui->setupUi(this);
    for (auto it = tables_names.begin(); it != tables_names.end(); ++it)
    {
        ui->tables_comboBox->addItem(*it);
    }
}

TableDropDialog::~TableDropDialog()
{
    delete ui;
}

void TableDropDialog::on_buttonBox_accepted()
{
    if (ui->tables_comboBox->currentIndex() == -1)
    {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", "ERROR: specify a table to drop");
        messageBox.setFixedSize(500, 200);
        return;
    }

    table_to_drop = ui->tables_comboBox->currentText();

    close();
    setResult(QDialog::Accepted);
}


void TableDropDialog::on_buttonBox_rejected()
{
    close();
    setResult(QDialog::Rejected);
}

