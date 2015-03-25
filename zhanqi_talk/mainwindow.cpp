#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../tcp_socket_test/danmuconnection.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendMsg()
{
    QString s = ui->lineEdit->text();

    dc.postDanmu(s);
}
