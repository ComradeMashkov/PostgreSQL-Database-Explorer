#ifndef TABLECREATEDIALOG_H
#define TABLECREATEDIALOG_H

#include <QDialog>

namespace Ui {
class TableCreateDialog;
}

class TableCreateDialog : public QDialog
{
    friend class MainWindow;

    Q_OBJECT

public:
    explicit TableCreateDialog(QWidget *parent = nullptr);
    ~TableCreateDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_type_comboBox_currentIndexChanged(int index);

    void on_add_pushButton_clicked();

private:
    Ui::TableCreateDialog *ui;

    QString table_name;

    QSet<QString> field_names;

    struct FieldStats
    {
        QString name;
        QString type;
        bool is_primary_key;
        bool is_not_null;
        QString params { "" };
    };
    QVector<FieldStats> fields_vec;
};

#endif // TABLECREATEDIALOG_H
