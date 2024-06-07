#ifndef ENTITIESWINDOW_H
#define ENTITIESWINDOW_H

#include <QWidget>
#include <QtSql>
#include <QMessageBox>
#include <QRegExp>
#include <QRegExpValidator>
namespace Ui {
class EntitiesWindow;
}

class EntitiesWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EntitiesWindow(QWidget *parent = nullptr);
    ~EntitiesWindow();

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
    Ui::EntitiesWindow *ui;
    QSqlQueryModel* model;
    QSqlQuery* query;
    QMessageBox* mes;
    QRegExpValidator* validator;
    QRegExpValidator* intvalid;
    void select_data();
};

#endif // ENTITIESWINDOW_H
