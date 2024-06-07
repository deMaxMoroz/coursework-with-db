#ifndef FUNCTIONSWINDOW_H
#define FUNCTIONSWINDOW_H

#include <QMainWindow>
#include "helpwindow.h"
#include "ui_helpwindow.h"

#include "personswindow.h"
#include "ui_personswindow.h"

#include "entitieswindow.h"
#include "ui_entitieswindow.h"

#include "stationswindow.h"
#include "ui_stationswindow.h"

#include "debtorswindow.h"
#include "ui_debtorswindow.h"

#include "invoiceswindow.h"
#include "ui_invoiceswindow.h"


#include <QSqlQueryModel>
#include <QMessageBox>
namespace Ui {
class FunctionsWindow;
}

class FunctionsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FunctionsWindow(const QString& data, QWidget *parent = nullptr);
    ~FunctionsWindow();
    HelpWindow* helpwin;
    PersonsWindow* perswin;
    EntitiesWindow* entwin;
    StationsWindow* stnwin;
    DebtorsWindow* debwin;
    InvoicesWindow* invwin;



private slots:
    void on_action_triggered();

    void on_InvoicesPushButton_clicked();

    void on_PersonsPushButton_clicked();

    void on_action_3_triggered();

    void on_EntitiesPushButton_clicked();

    void on_DebtorsPushButton_clicked();

    void on_action_4_triggered();

    void on_StationsPushButton_clicked();

    void on_action_5_triggered();

    void on_action_6_triggered();

    void on_action_2_triggered();

    void on_PayPushButton_clicked();

    void on_PotentialPushButton_clicked();

    void on_PotentialPushButton_2_clicked();

private:
    Ui::FunctionsWindow *ui;
    QSqlQueryModel* model;
    QMessageBox* mes;
    QString user;
    QString data_;

};

#endif // FUNCTIONSWINDOW_H
