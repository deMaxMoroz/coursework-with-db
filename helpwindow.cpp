#include "helpwindow.h"
#include "ui_helpwindow.h"

HelpWindow::HelpWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HelpWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("QWidget#HelpWindow {background-color: rgb(255, 243, 211);}");
}

HelpWindow::~HelpWindow()
{
    delete ui;
}

void HelpWindow::on_pushButton_clicked()
{
    this->close();
}

