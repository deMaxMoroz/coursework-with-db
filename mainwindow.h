#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "helpwindow.h"
#include "ui_helpwindow.h"
#include "functionswindow.h"
#include "ui_functionswindow.h"
#include <QtSql>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    FunctionsWindow* funcwin;
    QMessageBox* mes;
private slots:
    void on_pushButton_clicked();

    QString getUser();

signals:
    void sendString(QString text);

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    bool connectToDb();

};
#endif // MAINWINDOW_H
