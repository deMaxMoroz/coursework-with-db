#include "stationswindow.h"
#include "ui_stationswindow.h"

StationsWindow::StationsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StationsWindow)
{
    ui->setupUi(this);
    select_data();

    connect(this,SIGNAL(refresh_table()),this,SLOT(obr_ref_tab()));

    ui->updLineEdit->setVisible(false);
    ui->delLineEdit->setVisible(false);
}

StationsWindow::~StationsWindow()
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


void StationsWindow::select_data(){
    model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM PowerStations ORDER BY ID ASC");

    model->setHeaderData(1,Qt::Horizontal,"Название");
    model->setHeaderData(2,Qt::Horizontal,"Номинальная мощность (МВт)");
    model->setHeaderData(3,Qt::Horizontal,"Макс. мощность (МВт)");
    model->setHeaderData(4,Qt::Horizontal,"Срок службы");
    model->setHeaderData(5,Qt::Horizontal,"Период использования");

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

void StationsWindow::obr_ref_tab(){
    select_data();
}

void StationsWindow::on_addPushButton_clicked()
{
    query = new QSqlQuery();
    query->prepare("INSERT INTO PowerStations(StationName,NominalPower,MaxPower,ServiceLife,InUsePeriod) VALUES (:name,:nompower,:maxpower,:servlife,:useperiod)");
    if (ui->addLineEdit->text().isEmpty()){
        mes = new QMessageBox();
        mes->setText("Добавьте название электростанции");
        mes->setIcon(QMessageBox::Warning);
        mes->show();
    } else if (ui->addDoubleSpinBox->value()==0.00 || ui->addDoubleSpinBox_2->value()==0.00){
        mes = new QMessageBox();
        mes->setText("Указана нулевая мощность. Укажите ненулевое значение");
        mes->setIcon(QMessageBox::Warning);
        mes->show();
    } else if (ui->addDoubleSpinBox->value() > ui->addDoubleSpinBox_2->value()){
        mes = new QMessageBox();
        mes->setText("Номинальная мощность не может быть больше максимальной!");
        mes->setIcon(QMessageBox::Warning);
        mes->show();
    }
    else {


        query->bindValue(":name",ui->addLineEdit->text());
        query->bindValue(":nompower",ui->addDoubleSpinBox->value());
        query->bindValue(":maxpower",ui->addDoubleSpinBox_2->value());
        query->bindValue(":servlife",ui->addSpinBox->value());
        query->bindValue(":useperiod",ui->addSpinBox_2->value());


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


void StationsWindow::on_updateTableView_clicked(const QModelIndex &index)
{
    int temp_ind;
    temp_ind = ui->updateTableView->model()->data(ui->updateTableView->model()->index(index.row(),0)).toInt();

    ui->updLineEdit->setText(QString::number(temp_ind));
    query = new QSqlQuery();
    query->prepare("SELECT StationName,NominalPower,MaxPower,ServiceLife,InUsePeriod FROM PowerStations WHERE ID=:ID ORDER BY ID ASC");
    query->bindValue(":ID",temp_ind);

    if (query->exec()){
        query->next();
        ui->updLineEdit_2->setText(query->value(0).toString());
        ui->updDoubleSpinBox->setValue(query->value(1).toDouble());
        ui->updDoubleSpinBox_2->setValue(query->value(2).toDouble());
        ui->updSpinBox->setValue(query->value(3).toInt());
        ui->updSpinBox_2->setValue(query->value(4).toInt());
    }
}


void StationsWindow::on_updatePushButton_clicked()
{
    query = new QSqlQuery();
    query->prepare("UPDATE PowerStations SET StationName=?,NominalPower=?,MaxPower=?,ServiceLife=?,InUsePeriod=? WHERE ID=?");
    if (ui->updLineEdit_2->text().isEmpty()){
        mes = new QMessageBox();
        mes->setText("Добавьте название электростанции");
        mes->setIcon(QMessageBox::Warning);
        mes->show();
    } else if (ui->updDoubleSpinBox->value()==0.00 || ui->updDoubleSpinBox_2->value()==0.00){
        mes = new QMessageBox();
        mes->setText("Указана нулевая мощность. Укажите ненулевое значение");
        mes->setIcon(QMessageBox::Warning);
        mes->show();
    }else if (ui->updDoubleSpinBox->value() > ui->updDoubleSpinBox_2->value()){
        mes = new QMessageBox();
        mes->setText("Номинальная мощность не может быть больше максимальной!");
        mes->setIcon(QMessageBox::Warning);
        mes->show();
    }
    else {


        query->bindValue(0,ui->updLineEdit_2->text());
        query->bindValue(1,ui->updDoubleSpinBox->value());
        query->bindValue(2,ui->updDoubleSpinBox_2->value());
        query->bindValue(3,ui->updSpinBox->value());
        query->bindValue(4,ui->updSpinBox_2->value());
        query->bindValue(5,ui->updLineEdit->text());

        query->exec();
    }
    emit refresh_table();
}


void StationsWindow::on_delTableView_clicked(const QModelIndex &index)
{
    int temp_ind;
    temp_ind = ui->delTableView->model()->data(ui->delTableView->model()->index(index.row(),0)).toInt();

    ui->delLineEdit->setText(QString::number(temp_ind));
    query = new QSqlQuery();
    query->prepare("SELECT StationName,NominalPower,MaxPower,ServiceLife,InUsePeriod FROM PowerStations WHERE ID=:ID ORDER BY ID ASC");
    query->bindValue(":ID",temp_ind);

    if (query->exec()){
        query->next();
        ui->delLineEdit_2->setText(query->value(0).toString());
        ui->delDoubleSpinBox->setValue(query->value(1).toDouble());
        ui->delDoubleSpinBox_2->setValue(query->value(2).toDouble());
        ui->delSpinBox->setValue(query->value(3).toInt());
        ui->delSpinBox_2->setValue(query->value(4).toInt());
    }
}


void StationsWindow::on_delPushButton_clicked()
{
    query = new QSqlQuery();
    query->prepare("DELETE FROM PowerStations WHERE ID=?");
    query->bindValue(0,ui->delLineEdit->text());
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


void StationsWindow::on_tabWidget_currentChanged(int index)
{
    emit refresh_table();
}

