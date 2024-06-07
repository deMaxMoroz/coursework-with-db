#ifndef PERSONSWINDOW_H
#define PERSONSWINDOW_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
#include <QRegExp>
#include <QRegExpValidator>
namespace Ui {
class PersonsWindow;
}

class PersonsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PersonsWindow(QWidget *parent = nullptr);
    ~PersonsWindow();
    int temp_ind;

private slots:
    void on_addPushButton_clicked();

    void obr_ref_tab();
    void on_updateTableView_clicked(const QModelIndex &index);

    void on_updatePushButton_clicked();

    void on_delPushButton_clicked();

    void on_delTableView_clicked(const QModelIndex &index);

    void on_tabWidget_currentChanged(int index);

signals:
    void refresh_table();

private:
    Ui::PersonsWindow *ui;
    QSqlQueryModel* model;
    QSqlQueryModel* idModel;
    QSqlQuery* query;
    QMessageBox* mes;
    QRegExpValidator* validator;
    QRegExpValidator* intvalid;
    void select_data();
};

#endif // PERSONSWINDOW_H
