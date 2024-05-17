#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "veniceservice.h"
#include <memory>
#include <QFileDialog>
#include <iostream>
#include <QtCore/qtimer.h>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;

    if(veniceService != nullptr)
    {
        delete veniceService;
    }
}

void MainWindow::setMainApplication(QApplication *application)
{
    this->mainApplication = application;
}

void MainWindow::on_sendFileButton_clicked()
{

    QString filePath= ui->selectedSendFileLineEdit->text();

    if(!filePath.isEmpty())
    {
        veniceService = new VeniceService(this, filePath.toStdString());

        //We associated the signal aboutToQuit with a lamba function that stop the service as slot
        this->connect(this->mainApplication, &QApplication::aboutToQuit, veniceService, [=]{
            veniceService->quit();
            veniceService->wait(5000);});
        veniceService->start();
        ui->sendFileButton->setEnabled(false);

    }

}

void MainWindow::doPopup()
{

}

void MainWindow::on_browseSendFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), QDir::homePath(), tr(""));


    if(!fileName.isEmpty())
    {
        ui->selectedSendFileLineEdit->setText(fileName);
        ui->sendFileButton->setEnabled(true);
    }


}

void MainWindow::on_receiveFileButton_clicked()
{
}
