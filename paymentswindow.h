#ifndef PAYMENTSWINDOW_H
#define PAYMENTSWINDOW_H

#include <QWidget>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class PaymentsWindow;
}

class PaymentsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PaymentsWindow(QWidget *parent = nullptr);
    ~PaymentsWindow();

private slots:
    void on_addPushButton_clicked();

    void obr_ref_tab();

    void on_updateTableView_clicked(const QModelIndex &index);

    void on_updPushButton_clicked();

    void on_delTableView_clicked(const QModelIndex &index);

    void on_delPushButton_clicked();
signals:
    void refresh_table();

private:
    Ui::PaymentsWindow *ui;
    QSqlQueryModel* model;
    QSqlQuery* query;
    QMessageBox* mes;
    void select_data();
};

#endif // PAYMENTSWINDOW_H
