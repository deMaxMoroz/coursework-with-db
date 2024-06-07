#include "personswindow.h"
#include "ui_personswindow.h"

PersonsWindow::PersonsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonsWindow)
{
    ui->setupUi(this);

    select_data();

    connect(this,SIGNAL(refresh_table()),this,SLOT(obr_ref_tab()));
    ui->updLineEditInd->setVisible(false);
    ui->delLineEditInd->setVisible(false);
   // this->setStyleSheet("QWidget#PersonsWindow {background-image: url(:/otherwin.png);}");

    validator = new QRegExpValidator(QRegExp("^[\\+]?[(]?[0-9]{3}[)]?[-\\s\\.]?[0-9]{3}[-\\s\\.]?[0-9]{4,6}$"));
    ui->lineEdit_5->setValidator(validator);
    ui->updLineEdit_5->setValidator(validator);
    ui->delLineEdit_5->setValidator(validator);

    intvalid = new QRegExpValidator(QRegExp("^\\d{12}$"));
    ui->lineEdit_7->setValidator(intvalid);
    ui->updLineEdit_7->setValidator(intvalid);
    ui->updLineEdit_7->setValidator(intvalid);


}
void PersonsWindow::select_data(){
    model = new QSqlQueryModel();
    model->setQuery("SELECT ID,Name,Surname,Patronymic,Address,PhoneNumber,Email,INN,DiscountReason,MeterServiceLife,AccuracyClass FROM PhysicalPersons ORDER BY ID ASC");
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

    ui->updateTableView->setModel(model);
    ui->updateTableView->resizeColumnsToContents();
    ui->updateTableView->hideColumn(0);
    ui->updateTableView->show();

    ui->delTableView->setModel(model);
    ui->delTableView->resizeColumnsToContents();
    ui->delTableView->hideColumn(0);
    ui->delTableView->show();


}
PersonsWindow::~PersonsWindow()
{
    delete ui;
    delete model;
    if (intvalid==nullptr){
        delete intvalid;
    }

    if (validator==nullptr){
        delete validator;
    }
    if (query==nullptr){
        delete query;
    }
    if (mes==nullptr){
        delete mes;
    }
}

void PersonsWindow::on_addPushButton_clicked()
{
    query = new QSqlQuery();
    query->prepare("INSERT INTO PhysicalPersons(Name,Surname,Patronymic,Address,PhoneNumber,Email,INN,DiscountReason,MeterServiceLife,AccuracyClass) VALUES (:name,:surname,:patronymic,:address,:phonenumber,:email,:inn,:discount,:meterlife,:accuracyclass)");
    if (ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty()){
        mes = new QMessageBox();
        mes->setText("Добавьте имя и фамилию");
        mes->setIcon(QMessageBox::Warning);
        mes->show();

    } else if (ui->lineEdit_4->text().isEmpty()){
        mes = new QMessageBox();
        mes->setText("Укажите адрес нахождения");
        mes->setIcon(QMessageBox::Warning);
        mes->show();

    }
    else if (ui->lineEdit_7->text().isEmpty() || ui->lineEdit_7->text().length()<12){
        mes = new QMessageBox();
        mes->setText("Добавьте корректный ИНН");
        mes->setIcon(QMessageBox::Warning);
        mes->show();

    } else {
        query->bindValue(":name",ui->lineEdit->text());
        query->bindValue(":surname",ui->lineEdit_2->text());
        query->bindValue(":patronymic",ui->lineEdit_3->text());
        query->bindValue(":address",ui->lineEdit_4->text());
        query->bindValue(":phonenumber",ui->lineEdit_5->text());
        query->bindValue(":email",ui->lineEdit_6->text());
        query->bindValue(":inn",ui->lineEdit_7->text());
        query->bindValue(":discount",ui->lineEdit_8->text());
        query->bindValue(":meterlife",ui->addSpinBox->value());
        query->bindValue(":accuracyclass",ui->addDoubleSpinBox->value());
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


void PersonsWindow::obr_ref_tab(){
    select_data();
}


void PersonsWindow::on_updateTableView_clicked(const QModelIndex &index)
{

    temp_ind = ui->updateTableView->model()->data(ui->updateTableView->model()->index(index.row(),0)).toInt();

    ui->updLineEditInd->setText(QString::number(temp_ind));
    query = new QSqlQuery();
    query->prepare("SELECT Name,Surname,Patronymic,Address,PhoneNumber,Email,INN,DiscountReason,MeterServiceLife,AccuracyClass FROM PhysicalPersons WHERE ID=:ID ORDER BY ID ASC");
    query->bindValue(":ID",ui->updLineEditInd->text());

    if (query->exec()){
        query->next();
        ui->updLineEdit->setText(query->value(0).toString());
        ui->updLineEdit_2->setText(query->value(1).toString());
        ui->updLineEdit_3->setText(query->value(2).toString());
        ui->updLineEdit_4->setText(query->value(3).toString());
        ui->updLineEdit_5->setText(query->value(4).toString());
        ui->updLineEdit_6->setText(query->value(5).toString());
        ui->updLineEdit_7->setText(query->value(6).toString());
        ui->updLineEdit_8->setText(query->value(7).toString());
        ui->updSpinBox->setValue(query->value(8).toInt());
        ui->updDoubleSpinBox->setValue(query->value(9).toDouble());
    }
}


void PersonsWindow::on_updatePushButton_clicked()
{
    query = new QSqlQuery();
    query->prepare("UPDATE PhysicalPersons SET Name=?,Surname=?,Patronymic=?,Address=?,PhoneNumber=?,Email=?,INN=?,DiscountReason=?,MeterServiceLife=?,AccuracyClass=? WHERE ID=?");
    if (ui->updLineEdit->text().isEmpty() || ui->updLineEdit_2->text().isEmpty()){
        mes = new QMessageBox();
        mes->setText("Укажите имя и фамилию");
        mes->setIcon(QMessageBox::Warning);
        mes->show();

    } else if (ui->updLineEdit_4->text().isEmpty()){
        mes = new QMessageBox();
        mes->setText("Укажите адрес нахождения");
        mes->setIcon(QMessageBox::Warning);
        mes->show();

    }
    else if (ui->updLineEdit_7->text().isEmpty() || ui->updLineEdit_7->text().length()<12){
        mes = new QMessageBox();
        mes->setText("Укажите корректный ИНН");
        mes->setIcon(QMessageBox::Warning);
        mes->show();

    } else {
        query->bindValue(0,ui->updLineEdit->text());
        query->bindValue(1,ui->updLineEdit_2->text());
        query->bindValue(2,ui->updLineEdit_3->text());
        query->bindValue(3,ui->updLineEdit_4->text());
        query->bindValue(4,ui->updLineEdit_5->text());
        query->bindValue(5,ui->updLineEdit_6->text());
        query->bindValue(6,ui->updLineEdit_7->text());
        query->bindValue(7,ui->updLineEdit_8->text());
        query->bindValue(8,ui->updSpinBox->value());
        query->bindValue(9,ui->updDoubleSpinBox->value());
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


void PersonsWindow::on_delPushButton_clicked()
{
    query = new QSqlQuery();
    query->prepare("DELETE FROM PhysicalPersons WHERE ID=?");
    query->bindValue(0,ui->delLineEditInd->text());
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


void PersonsWindow::on_delTableView_clicked(const QModelIndex &index)
{

    temp_ind = ui->delTableView->model()->data(ui->delTableView->model()->index(index.row(),0)).toInt();

    ui->delLineEditInd->setText(QString::number(temp_ind));
    query = new QSqlQuery();
    query->prepare("SELECT Name,Surname,Patronymic,Address,PhoneNumber,Email,INN,DiscountReason,MeterServiceLife,AccuracyClass FROM PhysicalPersons WHERE ID=:ID ORDER BY ID ASC");
    query->bindValue(":ID",ui->delLineEditInd->text());

    if (query->exec()){
        query->next();
        ui->delLineEdit->setText(query->value(0).toString());
        ui->delLineEdit_2->setText(query->value(1).toString());
        ui->delLineEdit_3->setText(query->value(2).toString());
        ui->delLineEdit_4->setText(query->value(3).toString());
        ui->delLineEdit_5->setText(query->value(4).toString());
        ui->delLineEdit_6->setText(query->value(5).toString());
        ui->delLineEdit_7->setText(query->value(6).toString());
        ui->delLineEdit_8->setText(query->value(7).toString());
        ui->delSpinBox->setValue(query->value(8).toInt());
        ui->delDoubleSpinBox->setValue(query->value(9).toDouble());
    }
}


void PersonsWindow::on_tabWidget_currentChanged(int index)
{
    emit refresh_table();
}

