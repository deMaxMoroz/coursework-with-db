#include "functionswindow.h"
#include "ui_functionswindow.h"


FunctionsWindow::FunctionsWindow(const QString& data, QWidget *parent) :
    QMainWindow(parent),data_(data),
    ui(new Ui::FunctionsWindow)
{
    ui->setupUi(this);
    setStyleSheet("FunctionsWindow {background-image: url(:/funcwin.png);}");
    mes = new QMessageBox();
    user = data_;

    if (user == "view_user"){
        ui->menu_2->setEnabled(false);
        mes->setText("Внимание! Вы зашли как пользователь с правами только просмотра!\nВам не доступна функция \"Редактирование\"! ");
        mes->setIcon(QMessageBox::Warning);
        mes->show();
    }


}

FunctionsWindow::~FunctionsWindow()
{
    delete ui;

    if (model==nullptr){
        delete model;
    }
    if (helpwin==nullptr){
        delete helpwin;
    }
    if (perswin==nullptr){
        delete perswin;
    }
    if (entwin==nullptr){
        delete entwin;
    }
    if (mes==nullptr){
        delete mes;
    }
}

void FunctionsWindow::on_action_triggered()
{
    helpwin = new HelpWindow();
    helpwin->show();

}

void FunctionsWindow::on_InvoicesPushButton_clicked()
{
    model = new QSqlQueryModel();
    model->setQuery("SELECT i.ID, p.Name || ' ' || p.Surname || ' ' || p.Patronymic AS Person, l.Name as EntityName, CASE WHEN d.AmountFee IS NOT NULL THEN d.AmountFee \
                    ELSE 0 END AS AmountFee,i.ConsumedAmount,i.CostPerOne,i.BillingType,i.CalculationDate,i.DueDate,i.PaymentDate,ps.StationName \
                    FROM Invoices i LEFT JOIN PhysicalPersons p ON i.PersonID=p.ID LEFT JOIN LegalEntities l ON i.EntityID = l.ID \
                    LEFT JOIN Debtors d ON i.DebtorID = d.ID LEFT JOIN PowerStations ps ON i.StationID = ps.ID ORDER BY i.ID ASC;");

    model->setHeaderData(1,Qt::Horizontal,"Физ.лицо");
    model->setHeaderData(2,Qt::Horizontal,"Юр.Лицо");
    model->setHeaderData(3,Qt::Horizontal,"Сумма долга");
    model->setHeaderData(4,Qt::Horizontal,"Потреблено кВт*ч");
    model->setHeaderData(5,Qt::Horizontal,"Стоимость 1 кВт*ч");
    model->setHeaderData(6,Qt::Horizontal,"Тип тарифа");
    model->setHeaderData(7,Qt::Horizontal,"Дата выставления");
    model->setHeaderData(8,Qt::Horizontal,"Срок оплаты");
    model->setHeaderData(9,Qt::Horizontal,"Дата платежа");
    model->setHeaderData(10,Qt::Horizontal,"Обслуживающая ЭС");

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->hideColumn(0);
    ui->tableView->show();


}


void FunctionsWindow::on_PersonsPushButton_clicked()
{
    model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM PhysicalPersons ORDER BY ID ASC");
    model->setHeaderData(1,Qt::Horizontal,"Имя");
    model->setHeaderData(2,Qt::Horizontal,"Фамилия");
    model->setHeaderData(3,Qt::Horizontal,"Отчество");
    model->setHeaderData(4,Qt::Horizontal,"Адрес");
    model->setHeaderData(5,Qt::Horizontal,"Телефон");
    model->setHeaderData(6,Qt::Horizontal,"Email");
    model->setHeaderData(7,Qt::Horizontal,"ИНН");
    model->setHeaderData(8,Qt::Horizontal,"Причина льгот");
    model->setHeaderData(9,Qt::Horizontal,"Срок службы счётчика");
    model->setHeaderData(10,Qt::Horizontal,"Класс точности счётчика");
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->hideColumn(0);
    ui->tableView->show();

}


void FunctionsWindow::on_action_3_triggered()
{
    perswin = new PersonsWindow();
    perswin->show();
}


void FunctionsWindow::on_EntitiesPushButton_clicked()
{
    model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM LegalEntities ORDER BY ID ASC");
    model->setHeaderData(1,Qt::Horizontal,"Название");
    model->setHeaderData(2,Qt::Horizontal,"Адрес");
    model->setHeaderData(3,Qt::Horizontal,"Телефон");
    model->setHeaderData(4,Qt::Horizontal,"Email");
    model->setHeaderData(5,Qt::Horizontal,"ИНН");
    model->setHeaderData(6,Qt::Horizontal,"Кол-во трансформаторов");
    model->setHeaderData(7,Qt::Horizontal,"Кол-во счётчиков");
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->hideColumn(0);
    ui->tableView->show();
}


void FunctionsWindow::on_StationsPushButton_clicked()
{
    model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM PowerStations ORDER BY ID ASC");
    model->setHeaderData(1,Qt::Horizontal,"Станция");
    model->setHeaderData(2,Qt::Horizontal,"Номинальная мощность (МВт)");
    model->setHeaderData(3,Qt::Horizontal,"Предельная мощность (МВт)");
    model->setHeaderData(4,Qt::Horizontal,"Срок службы");
    model->setHeaderData(5,Qt::Horizontal,"Период использования");
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->hideColumn(0);
    ui->tableView->show();
}



void FunctionsWindow::on_DebtorsPushButton_clicked()
{
    model = new QSqlQueryModel();
    model->setQuery("SELECT d.ID, CASE WHEN i.PersonID IS NOT NULL THEN (p.Name || ' ' || p.Surname || ' ' || p.Patronymic) \
                    WHEN i.EntityID IS NOT NULL THEN l.Name ELSE '' END AS Debtor,d.AmountFee,d.LastDate,\
                    CASE WHEN d.ServiceDisabled = true THEN 'Остановлено'\
                    ELSE 'Оказывается'\
                    END AS Service\
                    FROM Debtors d LEFT JOIN Invoices i ON i.DebtorID = d.ID\
                    LEFT JOIN PhysicalPersons p ON i.PersonID = p.ID\
                    LEFT JOIN LegalEntities l ON i.EntityID = l.ID ORDER BY d.ID ASC");
    model->setHeaderData(1,Qt::Horizontal,"Должник");
    model->setHeaderData(2,Qt::Horizontal,"Сумма долга");
    model->setHeaderData(3,Qt::Horizontal,"Крайник срок");
    model->setHeaderData(4,Qt::Horizontal,"Обслуживание");
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->hideColumn(0);
    ui->tableView->show();
}


void FunctionsWindow::on_action_4_triggered()
{
    entwin = new EntitiesWindow();
    entwin->show();

}


void FunctionsWindow::on_action_5_triggered()
{
    stnwin = new StationsWindow();
    stnwin->show();
}


void FunctionsWindow::on_action_6_triggered()
{
    debwin = new DebtorsWindow();
    debwin->show();
}


void FunctionsWindow::on_action_2_triggered()
{
    invwin = new InvoicesWindow();
    invwin->show();
}



void FunctionsWindow::on_PayPushButton_clicked()
{
    model = new QSqlQueryModel();
    model->setQuery("SELECT i.ID, CASE WHEN i.PersonID IS NOT NULL THEN p.Name || ' ' ||  p.Surname || ' ' || p.Patronymic \
                    ELSE l.Name \
                    END AS Name, i.ConsumedAmount*i.CostPerOne AS TotalSum FROM Invoices i \
  LEFT JOIN PhysicalPersons p ON p.ID = i.PersonID \
  LEFT JOIN LegalEntities l ON l.ID = i.EntityID ORDER BY i.ID ASC;");
    model->setHeaderData(1,Qt::Horizontal,"Клиент");
    model->setHeaderData(2,Qt::Horizontal,"Итоговая сумма оплаты");
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->hideColumn(0);
    ui->tableView->show();
}


void FunctionsWindow::on_PotentialPushButton_clicked()
{
    model = new QSqlQueryModel();
    model->setQuery("SELECT i.ID,CASE WHEN i.PersonID IS NOT NULL THEN p.Name || ' ' ||  p.Surname || ' ' || p.Patronymic \
                    ELSE l.Name \
                    END AS Name, \
        CASE WHEN i.PaymentDate > i.DueDate THEN 'Просрочено' \
        ELSE 'Не оплачено' \
        END AS Status \
                    FROM Invoices i \
                    LEFT JOIN PhysicalPersons p ON p.ID = i.PersonID \
  LEFT JOIN LegalEntities l ON l.ID = i.EntityID WHERE i.PaymentDate > i.DueDate OR (i.DueDate < CURRENT_DATE AND i.PaymentDate is NULL) ORDER BY i.ID ASC");
    model->setHeaderData(1,Qt::Horizontal,"Клиент");
    model->setHeaderData(2,Qt::Horizontal,"Статус оплаты");
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->hideColumn(0);
    ui->tableView->show();
}


void FunctionsWindow::on_PotentialPushButton_2_clicked()
{
    model = new QSqlQueryModel();
    model->setQuery("SELECT hi.ID,hi.InvoiceID,p.Name || ' ' || p.Surname || ' ' || p.Patronymic AS Person, l.Name,d.AmountFee,ps.StationName, hi.ConsumedAmount,hi.CostPerOne,hi.BillingType,hi.CalculationDate,hi.DueDate,hi.PaymentDate,hi.Operation,hi.CreationTime FROM HistoricalInvoices hi \
                    LEFT JOIN PhysicalPersons p ON hi.PersonID=p.ID \
                    LEFT JOIN LegalEntities l ON hi.EntityID = l.ID \
                    LEFT JOIN Debtors d ON hi.DebtorID = d.ID\
                    LEFT JOIN PowerStations ps ON hi.StationID = ps.ID ORDER BY hi.CreationTime DESC");
   model->setHeaderData(1,Qt::Horizontal,"Номер квитанции");
   model->setHeaderData(2,Qt::Horizontal,"Физ.лицо");
   model->setHeaderData(3,Qt::Horizontal,"Юр.Лицо");
   model->setHeaderData(4,Qt::Horizontal,"Сумма долга");
   model->setHeaderData(5,Qt::Horizontal,"Обслуживающая ЭС");
   model->setHeaderData(6,Qt::Horizontal,"Потреблено кВт*ч");
   model->setHeaderData(7,Qt::Horizontal,"Стоимость 1 кВт*ч");
   model->setHeaderData(8,Qt::Horizontal,"Тип тарифа");
   model->setHeaderData(9,Qt::Horizontal,"Дата выставления");
   model->setHeaderData(10,Qt::Horizontal,"Срок оплаты");
   model->setHeaderData(11,Qt::Horizontal,"Дата платежа");
   model->setHeaderData(12,Qt::Horizontal,"Тип операции");
   model->setHeaderData(13,Qt::Horizontal,"Время операции");

   ui->tableView->setModel(model);
   ui->tableView->resizeColumnsToContents();
   ui->tableView->hideColumn(0);
   ui->tableView->show();

}

