#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::getUser);

    setStyleSheet("MainWindow {background-image: url(:/login.png);}");

}

MainWindow::~MainWindow()
{
    delete ui;
    if (mes==nullptr){
        delete mes;
    }
    if (funcwin==nullptr){
        delete funcwin;
    }
}
QString MainWindow::getUser(){
    return ui->lineEdit->text();
}


bool MainWindow::connectToDb(){
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("coursework");
    db.setUserName(ui->lineEdit->text());
    db.setPassword(ui->lineEdit_2->text());
    if (db.open()){

        return true;
    }
    return false;
}

void MainWindow::on_pushButton_clicked()
{
    mes = new QMessageBox();
    if (connectToDb()){
        QString str = getUser();
        funcwin = new FunctionsWindow(str);
        //connect(this,&MainWindow::sendString,funcwin,&FunctionsWindow::receiveUser);

        //emit sendString(str);
        mes->setText("Соединение успешно!\nДобро пожаловать " + str);
        mes->show();
        this->hide();
        funcwin->show();
        //emit sendString(ui->lineEdit->text());
    } else {
        mes->setText("Неправильный логин/пароль");
        mes->show();
    }
}

