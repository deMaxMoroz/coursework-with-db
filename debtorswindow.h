#ifndef DEBTORSWINDOW_H
#define DEBTORSWINDOW_H

#include <QWidget>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class DebtorsWindow;
}

class DebtorsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DebtorsWindow(QWidget *parent = nullptr);
    ~DebtorsWindow();
    int postcombo, updcombo, delcombo;

private slots:
    void on_addPushButton_clicked();

    void obr_ref_tab();

    void on_addComboBox_currentIndexChanged(int index);
    void on_updateTableView_clicked(const QModelIndex &index);

    void on_updatePushButton_clicked();

    void on_updComboBox_currentIndexChanged(int index);

    void on_delTableView_clicked(const QModelIndex &index);

    void on_delComboBox_currentIndexChanged(int index);

    void on_delPushButton_clicked();

    void on_tabWidget_currentChanged(int index);

signals:
    void refresh_table();

private:
    Ui::DebtorsWindow *ui;
    QSqlQueryModel* model;
    QSqlQuery* query;
    QMessageBox* mes;
    void select_data();
};

#endif // DEBTORSWINDOW_H
