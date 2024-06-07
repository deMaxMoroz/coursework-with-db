#ifndef STATIONSWINDOW_H
#define STATIONSWINDOW_H

#include <QWidget>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class StationsWindow;
}

class StationsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StationsWindow(QWidget *parent = nullptr);
    ~StationsWindow();

private slots:
    void obr_ref_tab();

    void on_addPushButton_clicked();
    void on_updateTableView_clicked(const QModelIndex &index);

    void on_updatePushButton_clicked();

    void on_delTableView_clicked(const QModelIndex &index);

    void on_delPushButton_clicked();

    void on_tabWidget_currentChanged(int index);

signals:
    void refresh_table();

private:
    Ui::StationsWindow *ui;
    QSqlQueryModel* model;
    QSqlQuery* query;
    QMessageBox* mes;
    void select_data();
};

#endif // STATIONSWINDOW_H
