#ifndef TABLEDROPDIALOG_H
#define TABLEDROPDIALOG_H

#include <QDialog>

namespace Ui {
class TableDropDialog;
}

class TableDropDialog : public QDialog
{
    friend class MainWindow;

    Q_OBJECT

public:
    explicit TableDropDialog(const QSet<QString>& tables_names, QWidget *parent = nullptr);
    ~TableDropDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::TableDropDialog* ui;
    QString table_to_drop;
};

#endif // TABLEDROPDIALOG_H
