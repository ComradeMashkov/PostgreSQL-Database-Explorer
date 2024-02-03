#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "authdialog.h"
#include "tablecreatedialog.h"

#include <pqxx/pqxx>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionConnect_triggered();

    void on_actionCreate_triggered();

    void on_actionDrop_triggered();

private:
    Ui::MainWindow* ui;
    pqxx::connection* c = nullptr;
    AuthDialog* auth_dialog = new AuthDialog();
    TableCreateDialog* tc_dialog = new TableCreateDialog();

    void InitializeAuthDialog();
    void ConnectToDatabase();
    bool CheckAuthorizationValidity();

    void InitializeTableCreateDialog();
};
#endif // MAINWINDOW_H