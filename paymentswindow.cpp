#include "paymentswindow.h"
#include "ui_paymentswindow.h"

PaymentsWindow::PaymentsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaymentsWindow)
{
    ui->setupUi(this);
    select_data();

    connect(this,SIGNAL(refresh_table()),this,SLOT(obr_ref_tab()));
}

PaymentsWindow::~PaymentsWindow()
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

void PaymentsWindow::select_data(){
    model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM Payments ORDER BY ID ASC");
    model->setHeaderData(0,Qt::Horizontal,"ID");
    model->setHeaderData(1,Qt::Horizontal,"Сумма платежа");
    model->setHeaderData(2,Qt::Horizontal,"Дата платежа");

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->show();

    ui->updateTableView->setModel(model);
    ui->updateTableView->resizeColumnsToContents();
    ui->updateTableView->show();

    ui->delTableView->setModel(model);
    ui->delTableView->resizeColumnsToContents();
    ui->delTableView->show();


}
void PaymentsWindow::obr_ref_tab(){
    select_data();
}
void PaymentsWindow::on_addPushButton_clicked()
{
    query = new QSqlQuery();
    query->prepare("INSERT INTO Payments(PaymentSum,PaymentDate) VALUES (:paysum,:paydate)");
    query->bindValue(":paysum",ui->doubleSpinBox->value());
    query->bindValue(":surname",ui->dateEdit->date());


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

    emit refresh_table();
}



void PaymentsWindow::on_updateTableView_clicked(const QModelIndex &index)
{
    int temp_ind;
    temp_ind = ui->updateTableView->model()->data(ui->updateTableView->model()->index(index.row(),0)).toInt();

    ui->updLineEdit->setText(QString::number(temp_ind));
    query = new QSqlQuery();
    query->prepare("SELECT PaymentSum,PaymentDate FROM Payments WHERE ID=:ID ORDER BY ID ASC");
    query->bindValue(":ID",temp_ind);

    if (query->exec()){
        query->next();
        ui->updSpinBox->setValue(query->value(0).toDouble());
        ui->updDateEdit->setDate(query->value(1).toDate());
    }
}


void PaymentsWindow::on_updPushButton_clicked()
{
    query = new QSqlQuery();
    query->prepare("UPDATE Payments SET PaymentSum=?,PaymentDate=? WHERE ID=?");
    query->bindValue(0,ui->updSpinBox->value());
    query->bindValue(1,ui->updDateEdit->date());
    query->bindValue(2,ui->updLineEdit->text());


    query->exec();
    emit refresh_table();
}


void PaymentsWindow::on_delTableView_clicked(const QModelIndex &index)
{
    int temp_ind;
    temp_ind = ui->delTableView->model()->data(ui->delTableView->model()->index(index.row(),0)).toInt();

    ui->delLineEdit->setText(QString::number(temp_ind));
    query = new QSqlQuery();
    query->prepare("SELECT PaymentSum,PaymentDate FROM Payments WHERE ID=:ID ORDER BY ID ASC");
    query->bindValue(":ID",temp_ind);

    if (query->exec()){
        query->next();
        ui->updSpinBox->setValue(query->value(0).toDouble());
        ui->updDateEdit->setDate(query->value(1).toDate());
    }

}


void PaymentsWindow::on_delPushButton_clicked()
{
    query = new QSqlQuery();
    query->prepare("DELETE FROM Payments WHERE ID=?");
    query->bindValue(0,ui->delLineEdit->text());
    query->exec();
    emit refresh_table();
}

