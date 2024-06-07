#include "debtorswindow.h"
#include "ui_debtorswindow.h"

DebtorsWindow::DebtorsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DebtorsWindow)
{
    ui->setupUi(this);
    select_data();

    connect(this,SIGNAL(refresh_table()),this,SLOT(obr_ref_tab()));

    ui->updLineEdit->setVisible(false);
    ui->delLineEdit->setVisible(false);

    ui->addComboBox->addItems({"Оказывается", "Остановлено"});
    ui->updComboBox->addItems({"Оказывается", "Остановлено"});
    ui->delComboBox->addItems({"Оказывается", "Остановлено"});

    postcombo = 0;
    updcombo = 0;
    delcombo = 0;

}

DebtorsWindow::~DebtorsWindow()
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

void DebtorsWindow::select_data(){
    model = new QSqlQueryModel();
    model->setQuery("SELECT d.ID, CASE WHEN i.PersonID IS NOT NULL THEN (p.Name || ' ' || p.Surname || ' ' || p.Patronymic) WHEN i.EntityID IS NOT NULL THEN l.Name ELSE '' END AS Debtor,d.AmountFee,d.LastDate,\
                    CASE WHEN d.ServiceDisabled = true THEN 'Остановлено'\
                    ELSE 'Оказывается'\
                    END AS Service\
                    FROM Debtors d LEFT JOIN Invoices i ON i.DebtorID = d.ID\
                    LEFT JOIN PhysicalPersons p ON i.PersonID = p.ID\
                    LEFT JOIN LegalEntities l ON i.EntityID = l.ID ORDER BY d.ID ASC");

    model->setHeaderData(1,Qt::Horizontal,"Должник");
    model->setHeaderData(2,Qt::Horizontal,"Сумма долга");
    model->setHeaderData(3,Qt::Horizontal,"Крайний срок оплаты");
    model->setHeaderData(4,Qt::Horizontal,"Предоставление услуг");

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

void DebtorsWindow::on_addPushButton_clicked()
{
    query = new QSqlQuery();
    query->prepare("INSERT INTO Debtors(AmountFee,LastDate,ServiceDisabled) VALUES (:amount,:date,:serv)");

    if (ui->addDoubleSpinBox->value()==0.00){
        mes = new QMessageBox();
        mes->setText("Указана нулевая сумма. Укажите ненулевое значение");
        mes->setIcon(QMessageBox::Warning);
        mes->show();
    }
    else {
        query->bindValue(":amount",ui->addDoubleSpinBox->value());
        query->bindValue(":date",ui->addDateEdit->date());
        query->bindValue(":serv",(bool) postcombo);


        mes = new QMessageBox();
        if (!query->exec()){
            mes->setText(query->lastError().text());
            mes->setIcon(QMessageBox::Critical);
            mes->show();
            qDebug() << query->lastError().text();
        } else {
            mes->setText("Успешно! Данные добавлены!\nНе забудьте изменить квитанцию, добавив в неё сумму долга.");
            mes->show();
        }
    }
    emit refresh_table();
}

void DebtorsWindow::obr_ref_tab(){
    select_data();
}


void DebtorsWindow::on_addComboBox_currentIndexChanged(int index)
{
    postcombo = index;
}


void DebtorsWindow::on_updateTableView_clicked(const QModelIndex &index)
{
    int temp_ind;
    temp_ind = ui->updateTableView->model()->data(ui->updateTableView->model()->index(index.row(),0)).toInt();

    ui->updLineEdit->setText(QString::number(temp_ind));
    query = new QSqlQuery();
    query->prepare("SELECT d.ID, CASE WHEN i.PersonID IS NOT NULL THEN (p.Name || ' ' || p.Surname || ' ' || p.Patronymic) WHEN i.EntityID IS NOT NULL THEN l.Name ELSE '' END AS Debtor,d.AmountFee,d.LastDate,d.ServiceDisabled FROM Debtors d LEFT JOIN Invoices i ON i.DebtorID = d.ID\
                   LEFT JOIN PhysicalPersons p ON i.PersonID = p.ID\
                   LEFT JOIN LegalEntities l ON i.EntityID = l.ID WHERE d.ID=:ID ORDER BY d.ID ASC");
    query->bindValue(":ID",temp_ind);

    if (query->exec()){
        query->next();
        ui->updLineEdit_2->setText(query->value(1).toString());
        ui->updDoubleSpinBox->setValue(query->value(2).toDouble());
        ui->updDateEdit->setDate(query->value(3).toDate());
        //ui->updComboBox->getItemData(query->value(3).toInt());
        ui->updComboBox->setCurrentIndex(query->value(4).toInt());
    }
}


void DebtorsWindow::on_updatePushButton_clicked()
{
    query = new QSqlQuery();
    query->prepare("UPDATE Debtors SET AmountFee=?,LastDate=?,ServiceDisabled=? WHERE ID=?");
    if (ui->updDoubleSpinBox->value()==0.00){
        mes = new QMessageBox();
        mes->setText("Указана нулевая сумма. Укажите ненулевое значение");
        mes->setIcon(QMessageBox::Warning);
        mes->show();
    }
    else {
        query->bindValue(0,ui->updDoubleSpinBox->value());
        query->bindValue(1,ui->updDateEdit->date());
        query->bindValue(2,(bool) updcombo);
        query->bindValue(3,ui->updLineEdit->text());
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


void DebtorsWindow::on_updComboBox_currentIndexChanged(int index)
{
    updcombo = index;
}


void DebtorsWindow::on_delTableView_clicked(const QModelIndex &index)
{
    int temp_ind;
    temp_ind = ui->delTableView->model()->data(ui->delTableView->model()->index(index.row(),0)).toInt();

    ui->delLineEdit->setText(QString::number(temp_ind));
    query = new QSqlQuery();
    query->prepare("SELECT d.ID, CASE WHEN i.PersonID IS NOT NULL THEN (p.Name || ' ' || p.Surname || ' ' || p.Patronymic) WHEN i.EntityID IS NOT NULL THEN l.Name ELSE '' END AS Debtor,d.AmountFee,d.LastDate,d.ServiceDisabled FROM Debtors d LEFT JOIN Invoices i ON i.DebtorID = d.ID\
                   LEFT JOIN PhysicalPersons p ON i.PersonID = p.ID\
                   LEFT JOIN LegalEntities l ON i.EntityID = l.ID WHERE d.ID=:ID ORDER BY d.ID ASC");
    query->bindValue(":ID",temp_ind);

    if (query->exec()){
        query->next();
        ui->delLineEdit_2->setText(query->value(1).toString());
        ui->delDoubleSpinBox->setValue(query->value(2).toDouble());
        ui->delDateEdit->setDate(query->value(3).toDate());
        //ui->updComboBox->getItemData(query->value(3).toInt());
        ui->delComboBox->setCurrentIndex(query->value(4).toInt());
    }
}


void DebtorsWindow::on_delComboBox_currentIndexChanged(int index)
{
    delcombo = index;
}


void DebtorsWindow::on_delPushButton_clicked()
{
    query = new QSqlQuery();
    query->prepare("DELETE FROM Debtors WHERE ID=?");
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


void DebtorsWindow::on_tabWidget_currentChanged(int index)
{
    emit refresh_table();
}

