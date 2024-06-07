#include "entitieswindow.h"
#include "ui_entitieswindow.h"

EntitiesWindow::EntitiesWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EntitiesWindow)
{
    ui->setupUi(this);
    select_data();

    connect(this,SIGNAL(refresh_table()),this,SLOT(obr_ref_tab()));

    ui->updLineEdit->setVisible(false);
    ui->delLineEdit->setVisible(false);

    validator = new QRegExpValidator(QRegExp("^[\\+]?[(]?[0-9]{3}[)]?[-\\s\\.]?[0-9]{3}[-\\s\\.]?[0-9]{4,6}$"));
    ui->lineEdit_3->setValidator(validator);
    ui->updLineEdit_4->setValidator(validator);
    ui->delLineEdit_4->setValidator(validator);

    intvalid = new QRegExpValidator(QRegExp("^\\d{10}$"));
    ui->lineEdit_5->setValidator(intvalid);
    ui->updLineEdit_6->setValidator(intvalid);
    ui->updLineEdit_6->setValidator(intvalid);


}

EntitiesWindow::~EntitiesWindow()
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

void EntitiesWindow::select_data(){
    model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM LegalEntities ORDER BY ID ASC");

    model->setHeaderData(1,Qt::Horizontal,"Наименование");
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

    ui->updateTableView->setModel(model);
    ui->updateTableView->resizeColumnsToContents();
    ui->updateTableView->hideColumn(0);
    ui->updateTableView->show();

    ui->delTableView->setModel(model);
    ui->delTableView->resizeColumnsToContents();
    ui->delTableView->hideColumn(0);
    ui->delTableView->show();


}



void EntitiesWindow::on_addPushButton_clicked()
{
    query = new QSqlQuery();
    query->prepare("INSERT INTO LegalEntities(Name,Address,PhoneNumber,Email,INN,TransformersAmount,ElectricMetersAmount) VALUES (:name,:address,:phonenumber,:email,:inn,:transformersamount,:metersamount)");

    if (ui->lineEdit->text().isEmpty()){
        mes = new QMessageBox();
        mes->setText("Добавьте наименование");
        mes->setIcon(QMessageBox::Warning);
        mes->show();

    } else if (ui->lineEdit_2->text().isEmpty()){
        mes = new QMessageBox();
        mes->setText("Укажите место жительства");
        mes->setIcon(QMessageBox::Warning);
        mes->show();

    }
    else if (ui->lineEdit_5->text().isEmpty() || ui->lineEdit_5->text().length()<10){
        mes = new QMessageBox();
        mes->setText("Добавьте корректный ИНН");
        mes->setIcon(QMessageBox::Warning);
        mes->show();

    } else {
        query->bindValue(":name",ui->lineEdit->text());
        query->bindValue(":address",ui->lineEdit_2->text());

        query->bindValue(":phonenumber",ui->lineEdit_3->text());
        query->bindValue(":email",ui->lineEdit_4->text());
        query->bindValue(":inn",ui->lineEdit_5->text());
        query->bindValue(":transformersamount",ui->spinBox->value());
        query->bindValue(":metersamount",ui->spinBox_2->value());
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


void EntitiesWindow::obr_ref_tab(){
    select_data();
}


void EntitiesWindow::on_updateTableView_clicked(const QModelIndex &index)
{
    int temp_ind;
    temp_ind = ui->updateTableView->model()->data(ui->updateTableView->model()->index(index.row(),0)).toInt();

    ui->updLineEdit->setText(QString::number(temp_ind));
    query = new QSqlQuery();
    query->prepare("SELECT Name,Address,PhoneNumber,Email,INN,TransformersAmount,ElectricMetersAmount FROM LegalEntities WHERE ID=:ID ORDER BY ID ASC");
    query->bindValue(":ID",temp_ind);

    if (query->exec()){
        query->next();
        ui->updLineEdit_2->setText(query->value(0).toString());
        ui->updLineEdit_3->setText(query->value(1).toString());
        ui->updLineEdit_4->setText(query->value(2).toString());
        ui->updLineEdit_5->setText(query->value(3).toString());
        ui->updLineEdit_6->setText(query->value(4).toString());
        ui->updSpinBox->setValue(query->value(5).toInt());
        ui->updSpinBox_2->setValue(query->value(6).toInt());
    }
}


void EntitiesWindow::on_updatePushButton_clicked()
{
    query = new QSqlQuery();
    query->prepare("UPDATE LegalEntities SET Name=?,Address=?,PhoneNumber=?,Email=?,INN=?,TransformersAmount=?,ElectricMetersAmount=? WHERE ID=?");
    if (ui->updLineEdit_2->text().isEmpty()){
        mes = new QMessageBox();
        mes->setText("Добавьте наименование");
        mes->setIcon(QMessageBox::Warning);
        mes->show();

    } else if (ui->updLineEdit_3->text().isEmpty()){
        mes = new QMessageBox();
        mes->setText("Укажите место жительства");
        mes->setIcon(QMessageBox::Warning);
        mes->show();

    }
    else if (ui->updLineEdit_6->text().isEmpty() || ui->updLineEdit_6->text().length()<10){
        mes = new QMessageBox();
        mes->setText("Добавьте корректный ИНН");
        mes->setIcon(QMessageBox::Warning);
        mes->show();

    } else {
        query->bindValue(0,ui->updLineEdit_2->text());
        query->bindValue(1,ui->updLineEdit_3->text());
        query->bindValue(2,ui->updLineEdit_4->text());
        query->bindValue(3,ui->updLineEdit_5->text());
        query->bindValue(4,ui->updLineEdit_6->text());
        query->bindValue(5,ui->updSpinBox->value());
        query->bindValue(6,ui->updSpinBox_2->value());
        query->bindValue(7,ui->updLineEdit->text());

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


void EntitiesWindow::on_delTableView_clicked(const QModelIndex &index)
{
    int temp_ind;
    temp_ind = ui->delTableView->model()->data(ui->delTableView->model()->index(index.row(),0)).toInt();

    ui->delLineEdit->setText(QString::number(temp_ind));
    query = new QSqlQuery();
    query->prepare("SELECT Name,Address,PhoneNumber,Email,INN,TransformersAmount,ElectricMetersAmount FROM LegalEntities WHERE ID=:ID ORDER BY ID ASC");
    query->bindValue(":ID",temp_ind);

    if (query->exec()){
        query->next();
        ui->delLineEdit_2->setText(query->value(0).toString());
        ui->delLineEdit_3->setText(query->value(1).toString());
        ui->delLineEdit_4->setText(query->value(2).toString());
        ui->delLineEdit_5->setText(query->value(3).toString());
        ui->delLineEdit_6->setText(query->value(4).toString());
        ui->delSpinBox->setValue(query->value(5).toInt());
        ui->delSpinBox_2->setValue(query->value(6).toInt());
    }
}


void EntitiesWindow::on_delPushButton_clicked()
{
    query = new QSqlQuery();
    query->prepare("DELETE FROM LegalEntities WHERE ID=?");
    query->bindValue(0,ui->delLineEdit->text().toInt());
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


void EntitiesWindow::on_tabWidget_currentChanged(int index)
{
    emit refresh_table();
}

