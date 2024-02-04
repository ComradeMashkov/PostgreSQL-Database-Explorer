#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    if (c != nullptr)
    {
        c->disconnect();
    }
    delete c;
    delete ui;
}

void MainWindow::InitializeAuthDialog() {
    auth_dialog->setModal(true);
    auth_dialog->exec();
}

void MainWindow::ConnectToDatabase() {
    try
    {
        QString connection_query { "user=" + auth_dialog->username +
                                 " password=" + auth_dialog->password +
                                 " host=" + auth_dialog->host_address +
                                 " port=" + auth_dialog->port +
                                 " dbname=" + auth_dialog->database_name +
                                 " target_session_attrs=read-write" };

        c = new pqxx::connection(qPrintable(connection_query));
        qDebug() << "Connection is successful!";
    }

    catch (const std::exception& e)
    {
        qDebug() << e.what();
        return;
    }
}

bool MainWindow::CheckAuthorizationValidity()
{
    // Not valid
    if (auth_dialog->username.isEmpty() || auth_dialog->host_address.isEmpty() || auth_dialog->port.isEmpty() || auth_dialog->database_name.isEmpty())
    {
        return false;
    }

    // Valid
    return true;
}

void MainWindow::on_actionConnect_triggered()
{
    qDebug() << "Connect button is triggered!";
    InitializeAuthDialog();

    if (auth_dialog->result() == QDialog::Rejected)
    {
        return;
    }

    if (!CheckAuthorizationValidity())
    {
        qDebug() << "ERROR: specify your authorization parameters";
        return;
    }

    ConnectToDatabase();
}

void MainWindow::InitializeTableCreateDialog()
{
    tc_dialog = new TableCreateDialog();
    tc_dialog->setModal(true);
    tc_dialog->exec();
}


void MainWindow::on_actionCreate_triggered()
{
    if (c == nullptr)
    {
        qDebug() << "ERROR: connect to a database before creating a table";
        return;
    }

    qDebug() << "Create button is triggered!";
    InitializeTableCreateDialog();

    if (tc_dialog->result() == QDialog::Rejected)
    {
        return;
    }

    QString create_query = "CREATE TABLE " + tc_dialog->table_name + " (";
    for (int i = 0; i < tc_dialog->fields_vec.length(); ++i)
    {
        create_query += tc_dialog->fields_vec[i].name + " ";
        create_query += tc_dialog->fields_vec[i].type;
        if (tc_dialog->fields_vec[i].type == "VARCHAR" || tc_dialog->fields_vec[i].type == "NUMERIC")
        {
            create_query += "(" + tc_dialog->fields_vec[i].params + ")";
        }
        if (tc_dialog->fields_vec[i].is_not_null)
        {
            create_query += " NOT NULL";
        }
        if (tc_dialog->fields_vec[i].is_primary_key)
        {
            create_query += " PRIMARY KEY";
        }
        create_query += ", ";

    }
    create_query += ");";
    create_query.replace(", );", ");");

    delete tc_dialog;

    qDebug() << create_query;

    try
    {
        pqxx::work w(*c);
        w.exec(qPrintable(create_query));
        w.commit();
    }
    catch (const std::exception& e)
    {
        qDebug() << e.what();
        return;
    }
}

void MainWindow::InitializeTableDropDialog()
{
    td_dialog = new TableDropDialog(tables_names);
    td_dialog->setModal(true);
    td_dialog->exec();
}


void MainWindow::on_actionDrop_triggered()
{
    if (c == nullptr)
    {
        qDebug() << "ERROR: connect to a database before creating a table";
        return;
    }

    qDebug() << "Drop button is triggered!";

    pqxx::work w(*c);
    pqxx::result rows = w.exec("SELECT table_name FROM information_schema.tables WHERE table_schema = 'public';");
    for (const auto& row : rows)
    {
        tables_names.insert(QString::fromStdString(row[0].as<std::string>()));
    }
    w.commit();

    InitializeTableDropDialog();

    if (td_dialog->result() == QDialog::Rejected)
    {
        return;
    }

    QString drop_query = "DROP TABLE " + td_dialog->table_to_drop + ";";
    tables_names.remove(td_dialog->table_to_drop);

    qDebug() << drop_query;

    delete td_dialog;

    try
    {
        pqxx::work w(*c);
        w.exec(qPrintable(drop_query));
        w.commit();
    }
    catch (const std::exception& e)
    {
        qDebug() << e.what();
        return;
    }
}

