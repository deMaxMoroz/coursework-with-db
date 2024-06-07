#ifndef INVOICESWINDOW_H
#define INVOICESWINDOW_H

#include <QWidget>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class InvoicesWindow;
}

class InvoicesWindow : public QWidget
{
    Q_OBJECT

public:
    explicit InvoicesWindow(QWidget *parent = nullptr);
    ~InvoicesWindow();

private slots:
    void on_addPushButton_clicked();

    void on_updTableView_clicked(const QModelIndex &index);

    void on_updatePushButton_clicked();

    void on_delTableView_clicked(const QModelIndex &index);

    void on_delPushButton_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void obr_ref_tab();
    void obr_ref_comb();
    void on_addComboBox_currentIndexChanged(int index);

    void on_addComboBox_2_currentIndexChanged(int index);

    void on_addComboBox_3_currentIndexChanged(int index);

    void on_addComboBox_4_currentIndexChanged(int index);

    void on_addComboBox_5_currentIndexChanged(int index);

    void on_checkBox_stateChanged(int arg1);

    void on_radioButton_4_clicked();

    void on_radioButton_3_clicked();

    void on_updComboBox_currentIndexChanged(int index);

    void on_updComboBox_2_currentIndexChanged(int index);

    void on_updComboBox_3_currentIndexChanged(int index);

    void on_updComboBox_4_currentIndexChanged(int index);

    void on_updComboBox_5_currentIndexChanged(int index);

    void on_checkBox_2_stateChanged(int arg1);

    void on_delComboBox_currentIndexChanged(int index);

    void on_delComboBox_2_currentIndexChanged(int index);

    void on_delComboBox_3_currentIndexChanged(int index);

    void on_delComboBox_4_currentIndexChanged(int index);

    void on_delComboBox_5_currentIndexChanged(int index);

    void on_radioButton_5_clicked();

    void on_radioButton_6_clicked();

    void on_checkBox_3_stateChanged(int arg1);

    void on_tabWidget_currentChanged(int index);

signals:
    void refresh_table();

    void refresh_comboboxes();

private:
    Ui::InvoicesWindow *ui;
    QSqlQueryModel* model;
    QSqlQuery* query;
    QMessageBox* mes;
    void select_data();
    void update_combo_data();
    int persAddComb,entAddComb,debAddComb,statAddComb,billAddCombo;
    int persUpdComb,entUpdComb,debUpdComb,statUpdComb,billUpdCombo;
    int persDelComb,entDelComb,debDelComb,statDelComb,billDelCombo;
};

#endif // INVOICESWINDOW_H
