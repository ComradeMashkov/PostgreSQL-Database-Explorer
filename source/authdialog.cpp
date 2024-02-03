#include "authdialog.h"
#include "ui_authdialog.h"

AuthDialog::AuthDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AuthDialog)
{
    ui->setupUi(this);
}

AuthDialog::~AuthDialog()
{
    delete ui;
}

void AuthDialog::on_buttonBox_accepted()
{
    username = ui->username_lineEdit->text();
    password = ui->password_lineEdit->text();
    host_address = ui->host_lineEdit->text();
    port = ui->port_lineEdit->text();
    database_name = ui->db_lineEdit->text();

    close();
    setResult(QDialog::Accepted);
}


void AuthDialog::on_buttonBox_rejected()
{
    close();
    setResult(QDialog::Rejected);
}

