#include "invoiceswindow.h"
#include "ui_invoiceswindow.h"
#include <string>

InvoicesWindow::InvoicesWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InvoicesWindow)
{
    ui->setupUi(this);
    QSqlQuery* query = new QSqlQuery();
    if (ui->radioButton->isChecked()) ui->addComboBox_2->setEnabled(false);
    if (ui->radioButton_2->isChecked()) ui->addComboBox->setEnabled(false);
    if (ui->checkBox->isChecked()==false) ui->addDateEdit_3->setEnabled(false);

    if (ui->radioButton_4->isChecked()) ui->updComboBox_2->setEnabled(false);
    if (ui->radioButton_3->isChecked()) ui->updComboBox->setEnabled(false);
    if (ui->checkBox_2->isChecked() == false) ui->updDateEdit_3->setEnabled(false);

    if (ui->radioButton_5->isChecked()) ui->delComboBox_2->setEnabled(false);
    if (ui->radioButton_6->isChecked()) ui->delComboBox->setEnabled(false);
    if (ui->checkBox_3->isChecked() == false) ui->delDateEdit_3->setEnabled(false);



    connect(this,SIGNAL(refresh_table()),this,SLOT(obr_ref_tab()));
    connect(this,SIGNAL(refresh_comboboxes()), this,SLOT(obr_ref_comb()));

    ui->updLineEditInd->setVisible(false);
    ui->delLineEditInd->setVisible(false);
    select_data();
    query->exec("SELECT DISTINCT BillingType FROM Invoices");
    while (query->next()){
        ui->addComboBox_5->addItem(query->value(0).toString());
        ui->updComboBox_5->addItem(query->value(0).toString());
        ui->delComboBox_5->addItem(query->value(0).toString());
    }
    query->clear();
    query->exec("SELECT p.Name || ' ' || p.Surname || ' ' || p.Patronymic AS Person, p.INN, p.ID FROM PhysicalPersons p");
    while (query->next()){
        ui->addComboBox->addItem(query->value(0).toString() + " | ИНН: " + query->value(1).toString(),query->value(2).toInt());
        ui->updComboBox->addItem(query->value(0).toString() + " | ИНН: " + query->value(1).toString(),query->value(2).toInt());
        ui->delComboBox->addItem(query->value(0).toString() + " | ИНН: " + query->value(1).toString(),query->value(2).toInt());
    }
    query->clear();

    query->exec("SELECT Name, INN, ID FROM LegalEntities");
    while (query->next()){

        ui->addComboBox_2->addItem(query->value(0).toString() + " | ИНН: " + query->value(1).toString(),query->value(2).toInt());
        ui->updComboBox_2->addItem(query->value(0).toString() + " | ИНН: " + query->value(1).toString(),query->value(2).toInt());
        ui->delComboBox_2->addItem(query->value(0).toString() + " | ИНН: " + query->value(1).toString(),query->value(2).toInt());
    }
    query->clear();
    query->exec("SELECT d.AmountFee,d.LastDate,d.ID FROM Debtors d LEFT JOIN Invoices i ON i.DebtorID=d.ID WHERE i.DebtorID IS NULL");
    ui->addComboBox_3->addItem("Нет долгов", 0);
    while (query->next()){
        ui->addComboBox_3->addItem(query->value(0).toString()+ " | Край. Дата: " + query->value(1).toString(), query->value(2).toInt());
    }
    query->clear();
    query->exec("SELECT d.AmountFee,d.LastDate,d.ID FROM Debtors d");
    ui->updComboBox_3->addItem("Нет долгов", 0);
    ui->delComboBox_3->addItem("Нет долгов", 0);
    while (query->next()){
        ui->updComboBox_3->addItem(query->value(0).toString()+ " | Край. Дата: " + query->value(1).toString(), query->value(2).toInt());
        ui->delComboBox_3->addItem(query->value(0).toString()+ " | Край. Дата: " + query->value(1).toString(),  query->value(2).toInt());
    }
    query->clear();
    query->exec("SELECT StationName,ID FROM PowerStations");
    while (query->next()){
        ui->addComboBox_4->addItem(query->value(0).toString(), query->value(1).toInt());
        ui->updComboBox_4->addItem(query->value(0).toString(),query->value(1).toInt());
        ui->delComboBox_4->addItem(query->value(0).toString(),query->value(1).toInt());
    }
    query->clear();
    persAddComb = 0;
    persUpdComb = 0;
    persDelComb = 0;
    entAddComb = 0;
    entUpdComb = 0;
    entDelComb = 0;
    debAddComb = 0;
    debUpdComb = 0;
    debDelComb = 0;
    statAddComb = 0;
    statUpdComb = 0;
    statDelComb = 0;
    billAddCombo = 0;
    billUpdCombo = 0;
    billDelCombo = 0;

}

InvoicesWindow::~InvoicesWindow()
{
    delete ui;
    delete model;
    if (query==nullptr){
        delete query;
    }
    if (mes==nullptr){
        delete mes;
    }
}

void InvoicesWindow::select_data(){
    model = new QSqlQueryModel();
    model->setQuery("SELECT i.ID, p.Name || ' ' || p.Surname || ' ' || p.Patronymic AS Person, l.Name as EntityName, CASE WHEN d.AmountFee IS NOT NULL THEN d.AmountFee \
                    ELSE 0 END AS AmountFee,i.ConsumedAmount,i.CostPerOne,i.BillingType,i.CalculationDate,i.DueDate,i.PaymentDate,ps.StationName \
                    FROM Invoices i LEFT JOIN PhysicalPersons p ON i.PersonID=p.ID LEFT JOIN LegalEntities l ON i.EntityID = l.ID \
                    LEFT JOIN Debtors d ON i.DebtorID = d.ID LEFT JOIN PowerStations ps ON i.StationID = ps.ID ORDER BY i.ID ASC;");

    model->setHeaderData(1,Qt::Horizontal,"Физ.Лицо");
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

    ui->updTableView->setModel(model);
    ui->updTableView->resizeColumnsToContents();
    ui->updTableView->hideColumn(0);
    ui->updTableView->show();

    ui->delTableView->setModel(model);
    ui->delTableView->resizeColumnsToContents();
    ui->delTableView->hideColumn(0);
    ui->delTableView->show();


}


void InvoicesWindow::update_combo_data(){
    QSqlQuery* query = new QSqlQuery();
    ui->addComboBox->clear();
    ui->addComboBox_2->clear();
    ui->addComboBox_3->clear();
    ui->addComboBox_4->clear();

    ui->updComboBox->clear();
    ui->updComboBox_2->clear();
    ui->updComboBox_3->clear();
    ui->updComboBox_4->clear();

    ui->delComboBox->clear();
    ui->delComboBox_2->clear();
    ui->delComboBox_3->clear();
    ui->delComboBox_4->clear();


    query->exec("SELECT p.Name || ' ' || p.Surname || ' ' || p.Patronymic AS Person, p.INN, p.ID FROM PhysicalPersons p");
    while (query->next()){
        ui->addComboBox->addItem(query->value(0).toString() + " | ИНН: " + query->value(1).toString(),query->value(2).toInt());
        ui->updComboBox->addItem(query->value(0).toString() + " | ИНН: " + query->value(1).toString(),query->value(2).toInt());
        ui->delComboBox->addItem(query->value(0).toString() + " | ИНН: " + query->value(1).toString(),query->value(2).toInt());
    }
    query->clear();

    query->exec("SELECT Name, INN, ID FROM LegalEntities");
    while (query->next()){

        ui->addComboBox_2->addItem(query->value(0).toString() + " | ИНН: " + query->value(1).toString(),query->value(2).toInt());
        ui->updComboBox_2->addItem(query->value(0).toString() + " | ИНН: " + query->value(1).toString(),query->value(2).toInt());
        ui->delComboBox_2->addItem(query->value(0).toString() + " | ИНН: " + query->value(1).toString(),query->value(2).toInt());
    }
    query->clear();
    query->exec("SELECT d.AmountFee,d.LastDate,d.ID FROM Debtors d LEFT JOIN Invoices i ON i.DebtorID=d.ID WHERE i.DebtorID IS NULL");
    ui->addComboBox_3->addItem("Нет долгов", 0);
    while (query->next()){
        ui->addComboBox_3->addItem(query->value(0).toString()+ " | Край. Дата: " + query->value(1).toString(), query->value(2).toInt());
    }
    query->clear();
    query->exec("SELECT d.AmountFee,d.LastDate,d.ID FROM Debtors d");
    ui->updComboBox_3->addItem("Нет долгов", 0);
    ui->delComboBox_3->addItem("Нет долгов", 0);
    while (query->next()){
        ui->updComboBox_3->addItem(query->value(0).toString()+ " | Край. Дата: " + query->value(1).toString(), query->value(2).toInt());
        ui->delComboBox_3->addItem(query->value(0).toString()+ " | Край. Дата: " + query->value(1).toString(),  query->value(2).toInt());
    }
    query->clear();
    query->exec("SELECT StationName,ID FROM PowerStations");
    while (query->next()){
        ui->addComboBox_4->addItem(query->value(0).toString(), query->value(1).toInt());
        ui->updComboBox_4->addItem(query->value(0).toString(),query->value(1).toInt());
        ui->delComboBox_4->addItem(query->value(0).toString(),query->value(1).toInt());
    }
    query->clear();


}

void InvoicesWindow::obr_ref_comb(){
    update_combo_data();
}

void InvoicesWindow::obr_ref_tab(){
    select_data();
}

void InvoicesWindow::on_addPushButton_clicked()
{
    query = new QSqlQuery();
    query->prepare("INSERT INTO Invoices(PersonID,EntityID,DebtorID,StationID,ConsumedAmount,CostPerOne,BillingType,CalculationDate,DueDate,PaymentDate) VALUES\
 (:person,:entity,:debtor,:station,:consumed,:cost,:billtype,:calcdate,:duedate,:paydate)");

    if (ui->addDateEdit->date() > ui->addDateEdit_2->date() || (ui->addDateEdit_3->isEnabled() && ui->addDateEdit->date() >ui->addDateEdit_3->date())){
        mes = new QMessageBox();
        mes->setText("Некорректно установлены даты!");
        mes->setIcon(QMessageBox::Warning);
        mes->show();

    } else if (((ui->addComboBox_5->itemText(billAddCombo)=="Жилой" || ui->addComboBox_5->itemText(billAddCombo)=="Льготный") && ui->addComboBox_2->isEnabled())  || ((ui->addComboBox_5->itemText(billAddCombo)=="Коммерческий" || ui->addComboBox_5->itemText(billAddCombo)=="Промышленный") && ui->addComboBox->isEnabled())){
        mes = new QMessageBox();
        mes->setText("Невозможный тип тарифа!\nДля физ.лиц тарифы - Льготный/Жилой.\nДля юр.лиц тарифы - Коммерческий/Промышленный.");
        mes->setIcon(QMessageBox::Warning);
        mes->show();

    }else {
        if (ui->radioButton->isChecked() && ui->addComboBox->isEnabled()){
            int persInd = ui->addComboBox->itemData(persAddComb, Qt::UserRole).toInt();
            query->bindValue(":person",persInd);

        }
        if (ui->radioButton_2->isChecked() && ui->addComboBox_2->isEnabled()){
            int entInd = ui->addComboBox_2->itemData(entAddComb, Qt::UserRole).toInt();
            query->bindValue(":entity",entInd);
        }

        int debInd = ui->addComboBox_3->itemData(debAddComb, Qt::UserRole).toInt();
        if (debInd != 0 ) {
            query->bindValue(":debtor",debInd);
        }
        int statInd = ui->addComboBox_4->itemData(statAddComb, Qt::UserRole).toInt();
        query->bindValue(":station",statInd);
        query->bindValue(":consumed",ui->addDoubleSpinBox->value());
        query->bindValue(":cost",ui->addDoubleSpinBox_2->value());

        query->bindValue(":billtype",ui->addComboBox_5->itemText(billAddCombo));
        query->bindValue(":calcdate",ui->addDateEdit->date());
        query->bindValue(":duedate",ui->addDateEdit_2->date());
        if (ui->addDateEdit_3->isEnabled()){
            query->bindValue(":paydate",ui->addDateEdit_3->date());
        }


        mes = new QMessageBox();


        if (!query->exec()){
            mes->setText(query->lastError().text());
            mes->setIcon(QMessageBox::Critical);
            mes->show();
            qDebug() << query->lastError().text();
        } else {
            mes->setText("Успешно! Данные добавлены!");
            mes->show();
        }
    }

    emit refresh_table();
}


void InvoicesWindow::on_updTableView_clicked(const QModelIndex &index)
{
    int temp_ind;
    temp_ind = ui->updTableView->model()->data(ui->updTableView->model()->index(index.row(),0)).toInt();

    ui->updLineEditInd->setText(QString::number(temp_ind));
    query = new QSqlQuery();
    query->prepare("SELECT i.PersonID, i.EntityID, i.DebtorID, i.StationID,i.ConsumedAmount,i.CostPerOne,i.BillingType,i.CalculationDate,i.DueDate,i.PaymentDate \
                   FROM Invoices i WHERE i.ID=:ID ORDER BY i.ID ASC");
    query->bindValue(":ID",temp_ind);

    if (query->exec()){
        query->next();
        if (query->value(0) != NULL){
            ui->radioButton_4->setChecked(true);
            ui->updComboBox->setEnabled(true);
            int setPersInd = ui->updComboBox->findData(QVariant(query->value(0).toInt()));
            ui->updComboBox->setCurrentIndex(setPersInd);
            ui->radioButton_3->setChecked(false);
            ui->updComboBox_2->setEnabled(false);
        }
        if (query->value(1)!=NULL){
            ui->radioButton_3->setChecked(true);
            ui->updComboBox_2->setEnabled(true);
            int setEntInd = ui->updComboBox_2->findData(QVariant(query->value(1).toInt()));
            ui->updComboBox_2->setCurrentIndex(setEntInd);
            ui->radioButton_4->setChecked(false);
            ui->updComboBox->setEnabled(false);
        }
        if (query->value(2)==NULL){
            ui->updComboBox_3->setCurrentIndex(0);
        }else{
            int setDebInd = ui->updComboBox_3->findData(QVariant(query->value(2).toInt()));
            ui->updComboBox_3->setCurrentIndex(setDebInd);
        }
        int setStatInd = ui->updComboBox_4->findData(QVariant(query->value(3).toInt()));
        ui->updComboBox_4->setCurrentIndex(setStatInd);
        int billInd = 0;
        for (int i = 0; i<ui->updComboBox_5->count();i++){
            if (ui->updComboBox_5->itemText(i) == query->value(6).toString()){
                billInd = i;
                break;
            }
        }
        ui->updComboBox_5->setCurrentIndex(billInd);
        ui->updDoubleSpinBox->setValue(query->value(4).toDouble());
        ui->updDoubleSpinBox_2->setValue(query->value(5).toDouble());
        ui->updDateEdit->setDate(query->value(7).toDate());
        ui->updDateEdit_2->setDate(query->value(8).toDate());

        if (query->value(9)==NULL || query->value(9).toString()==""){
            ui->checkBox_2->setCheckState(Qt::Unchecked);
            ui->updDateEdit_3->setEnabled(false);
        }else{
            ui->checkBox_2->setCheckState(Qt::Checked);
            ui->updDateEdit_3->setEnabled(true);
            ui->updDateEdit_3->setDate(query->value(9).toDate());
        }
    } else {
        qDebug() << query->lastError().text();
    }
}


void InvoicesWindow::on_updatePushButton_clicked()
{
    query = new QSqlQuery();
    query->prepare("UPDATE Invoices SET PersonID=?,EntityID=?,DebtorID=?,StationID=?,ConsumedAmount=?,CostPerOne=?,BillingType=?,CalculationDate=?,DueDate=?,PaymentDate=? WHERE ID=?");
    if (ui->updDateEdit->date() > ui->updDateEdit_2->date() || (ui->updDateEdit_3->isEnabled() && ui->updDateEdit->date() >ui->updDateEdit_3->date())){
        mes = new QMessageBox();
        mes->setText("Некорректно установлены даты!");
        mes->setIcon(QMessageBox::Warning);
        mes->show();

    } else if (((ui->updComboBox_5->itemText(billUpdCombo)=="Жилой" || ui->updComboBox_5->itemText(billUpdCombo)=="Льготный") && ui->updComboBox_2->isEnabled())  || ((ui->updComboBox_5->itemText(billUpdCombo)=="Коммерческий" || ui->updComboBox_5->itemText(billUpdCombo)=="Промышленный") && ui->updComboBox->isEnabled())){
        mes = new QMessageBox();
        mes->setText("Невозможный тип тарифа!\nДля физ.лиц тарифы - Льготный/Жилой.\nДля юр.лиц тарифы - Коммерческий/Промышленный.");
        mes->setIcon(QMessageBox::Warning);
        mes->show();

    }else{
        if (ui->radioButton_4->isChecked() && ui->updComboBox->isEnabled()){
            int persInd = ui->updComboBox->itemData(persUpdComb, Qt::UserRole).toInt();
            query->bindValue(0,persInd);
            query->bindValue(1,QVariant());
        }
        if (ui->radioButton_3->isChecked() && ui->updComboBox_2->isEnabled()){
            int entInd = ui->updComboBox_2->itemData(entUpdComb, Qt::UserRole).toInt();
            query->bindValue(1,entInd);
            query->bindValue(0,QVariant());
        }

        int debInd = ui->updComboBox_3->itemData(debUpdComb, Qt::UserRole).toInt();
        if (debInd != 0 ) {
            query->bindValue(2,debInd);
        } else {
            query->bindValue(2,QVariant());
        }
        int statInd = ui->updComboBox_4->itemData(statUpdComb, Qt::UserRole).toInt();
        query->bindValue(3,statInd);
        query->bindValue(4,ui->updDoubleSpinBox->value());
        query->bindValue(5,ui->updDoubleSpinBox_2->value());
        query->bindValue(6,ui->updComboBox_5->itemText(billUpdCombo));
        query->bindValue(7,ui->updDateEdit->date());
        query->bindValue(8,ui->updDateEdit_2->date());
        if (ui->updDateEdit_3->isEnabled()){
            query->bindValue(9,ui->updDateEdit_3->date());
        }else{
            query->bindValue(9,QVariant());
        }
        query->bindValue(10,ui->updLineEditInd->text());


        mes = new QMessageBox();


        if (!query->exec()){
            mes->setText(query->lastError().text());
            mes->setIcon(QMessageBox::Critical);
            mes->show();
            qDebug() << query->lastError().text();
        } else {
            mes->setText("Успешно! Данные изменены!");
            mes->show();
        }
    }

    emit refresh_table();
}


void InvoicesWindow::on_delTableView_clicked(const QModelIndex &index)
{
    int temp_ind;
    temp_ind = ui->delTableView->model()->data(ui->delTableView->model()->index(index.row(),0)).toInt();

    ui->delLineEditInd->setText(QString::number(temp_ind));
    query = new QSqlQuery();
    query->prepare("SELECT i.PersonID, i.EntityID, i.DebtorID, i.StationID,i.ConsumedAmount,i.CostPerOne,i.BillingType,i.CalculationDate,i.DueDate,i.PaymentDate \
                   FROM Invoices i WHERE i.ID=:ID ORDER BY i.ID ASC");
    query->bindValue(":ID",temp_ind);
    if (query->exec()){
        query->next();
        if (query->value(0) != NULL){
            ui->radioButton_5->setChecked(true);
            ui->delComboBox->setEnabled(true);
            int delPersInd = ui->delComboBox->findData(QVariant(query->value(0).toInt()));
            ui->delComboBox->setCurrentIndex(delPersInd);
            ui->radioButton_6->setChecked(false);
            ui->delComboBox_2->setEnabled(false);
        }
        if (query->value(1)!=NULL){
            ui->radioButton_6->setChecked(true);
            ui->delComboBox_2->setEnabled(true);
            int delEntInd = ui->delComboBox_2->findData(QVariant(query->value(1).toInt()));

            ui->delComboBox_2->setCurrentIndex(delEntInd);
            ui->radioButton_5->setChecked(false);
            ui->delComboBox->setEnabled(false);
        }
        if (query->value(2)==NULL){
            ui->delComboBox_3->setCurrentIndex(0);
        }else{
            int delDebInd = ui->delComboBox_3->findData(QVariant(query->value(2).toInt()));
            ui->delComboBox_3->setCurrentIndex(delDebInd);
        }
        int delStatInd = ui->delComboBox_4->findData(QVariant(query->value(3).toInt()));
        ui->delComboBox_4->setCurrentIndex(delStatInd);
        int billInd = 0;
        for (int i = 0; i<ui->delComboBox_5->count();i++){
            if (ui->delComboBox_5->itemText(i) == query->value(6).toString()){
                billInd = i;
                break;
            }
        }
        ui->delComboBox_5->setCurrentIndex(billInd);
        ui->delDoubleSpinBox->setValue(query->value(4).toDouble());
        ui->delDoubleSpinBox_2->setValue(query->value(5).toDouble());
        ui->delDateEdit->setDate(query->value(7).toDate());
        ui->delDateEdit_2->setDate(query->value(8).toDate());

        if (query->value(9)==NULL || query->value(9).toString()==""){
            ui->checkBox_3->setCheckState(Qt::Unchecked);
            ui->delDateEdit_3->setEnabled(false);
        }else{
            ui->checkBox_3->setCheckState(Qt::Checked);
            ui->delDateEdit_3->setEnabled(true);
            ui->delDateEdit_3->setDate(query->value(9).toDate());
        }
    } else {
        qDebug() << query->lastError().text();
    }
}


void InvoicesWindow::on_delPushButton_clicked()
{
    query = new QSqlQuery();
    query->prepare("DELETE FROM Invoices WHERE ID=?");
    query->bindValue(0,ui->delLineEditInd->text().toInt());
    mes = new QMessageBox();
    if (!query->exec()){
        mes->setText(query->lastError().text());
        mes->setIcon(QMessageBox::Critical);
        mes->show();
        qDebug() << query->lastError().text();
    } else {
        mes->setText("Успешно! Данные удалены!");
        mes->show();
    }
    emit refresh_table();
}


void InvoicesWindow::on_radioButton_clicked()
{
    ui->addComboBox_2->setEnabled(false);
    ui->addComboBox->setEnabled(true);
}



void InvoicesWindow::on_radioButton_2_clicked()
{
    ui->addComboBox->setEnabled(false);
    ui->addComboBox_2->setEnabled(true);
}


void InvoicesWindow::on_addComboBox_currentIndexChanged(int index)
{
    persAddComb = index;
}


void InvoicesWindow::on_addComboBox_2_currentIndexChanged(int index)
{
    entAddComb = index;
}


void InvoicesWindow::on_addComboBox_3_currentIndexChanged(int index)
{
    debAddComb = index;
}


void InvoicesWindow::on_addComboBox_4_currentIndexChanged(int index)
{
    statAddComb = index;
}


void InvoicesWindow::on_addComboBox_5_currentIndexChanged(int index)
{
    billAddCombo = index;
}


void InvoicesWindow::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked){
        ui->addDateEdit_3->setEnabled(true);
    }
    else {
        ui->addDateEdit_3->setEnabled(false);
    }
}


void InvoicesWindow::on_radioButton_4_clicked()
{
    ui->updComboBox->setEnabled(true);
    ui->updComboBox_2->setEnabled(false);
}


void InvoicesWindow::on_radioButton_3_clicked()
{
    ui->updComboBox_2->setEnabled(true);
    ui->updComboBox->setEnabled(false);
}


void InvoicesWindow::on_updComboBox_currentIndexChanged(int index)
{
    persUpdComb = index;
}


void InvoicesWindow::on_updComboBox_2_currentIndexChanged(int index)
{
    entUpdComb = index;
}



void InvoicesWindow::on_updComboBox_3_currentIndexChanged(int index)
{
    debUpdComb = index;
}

void InvoicesWindow::on_updComboBox_4_currentIndexChanged(int index)
{
    statUpdComb = index;
}


void InvoicesWindow::on_updComboBox_5_currentIndexChanged(int index)
{
    billUpdCombo = index;
}

void InvoicesWindow::on_checkBox_2_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked){
        ui->updDateEdit_3->setEnabled(true);
    }
    else {
        ui->updDateEdit_3->setEnabled(false);
    }
}



void InvoicesWindow::on_delComboBox_currentIndexChanged(int index)
{
    persDelComb = index;
}


void InvoicesWindow::on_delComboBox_2_currentIndexChanged(int index)
{
    entDelComb = index;
}


void InvoicesWindow::on_delComboBox_3_currentIndexChanged(int index)
{
    debDelComb = index;
}


void InvoicesWindow::on_delComboBox_4_currentIndexChanged(int index)
{
    statDelComb = index;
}



void InvoicesWindow::on_delComboBox_5_currentIndexChanged(int index)
{
    billDelCombo = index;
}


void InvoicesWindow::on_radioButton_5_clicked()
{
    ui->delComboBox->setEnabled(true);
    ui->delComboBox_2->setEnabled(false);
}


void InvoicesWindow::on_radioButton_6_clicked()
{
    ui->delComboBox_2->setEnabled(true);
    ui->delComboBox->setEnabled(false);
}


void InvoicesWindow::on_checkBox_3_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked){
        ui->delDateEdit_3->setEnabled(true);
    }
    else {
        ui->delDateEdit_3->setEnabled(false);
    }
}


void InvoicesWindow::on_tabWidget_currentChanged(int index)
{
    emit refresh_table();
    emit refresh_comboboxes();
}

