#include "include/ui/mainwindow.h"
#include "ui_mainwindow.h"
#include "include/channel/datachannel.h"
#include "include/channel/wifidatachannel.h"
#include "include/channel/blebootstrapchannel.h"
#include "include/consumer/filetransferservicediscoverer.h"

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
        DataChannel* dataChannel = new WifiDataChannel();
        BleBootstrapChannel* boostrapChannel = new BleBootstrapChannel();
        this->veniceService = new FileTransferService(dataChannel, boostrapChannel, filePath.toStdString(), this);

        //We associated the signal aboutToQuit with a lamba function that stop the service as slot
        this->connect(this->mainApplication, &QApplication::aboutToQuit, veniceService, [this]{
            veniceService->quit();
            veniceService->wait(5000);});
        veniceService->start();
        ui->sendFileButton->setEnabled(false);

    }

}

void MainWindow::doPopup()
{

}

// Send Panel
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



void MainWindow::on_bleSendRadioButton_clicked()
{
    if(ui->bleSendRadioButton->isChecked())
        ui->wifiSendRadioButton->setEnabled(true);
   else
   {
        ui->wifiSendRadioButton->setEnabled(false);
        ui->wifiSendRadioButton->setChecked(false);
        ui->browseSendFileButton->setEnabled(false);
   }

}

void MainWindow::on_wifiSendRadioButton_clicked()
{
    if(ui->wifiSendRadioButton->isChecked())
        ui->browseSendFileButton->setEnabled(true);
    else
        ui->browseSendFileButton->setEnabled(false);
}

//Receive panel

void MainWindow::on_receiveFileButton_clicked()
{
    BleBootstrapChannel* boostrapChannel = new BleBootstrapChannel();

    FileTransferServiceDiscoverer bleDiscover(boostrapChannel);


    QString directoryPath= ui->selectedReceiveFileLineEdit->text();

    if(!directoryPath.isEmpty())
        bleDiscover.runDiscoverer();

}

void MainWindow::on_bleReceiveRadioButton_clicked()
{

    if(ui->bleReceiveRadioButton->isChecked())
        ui->wifiReceiveRadioButton->setEnabled(true);
    else
    {
        ui->wifiReceiveRadioButton->setEnabled(false);
        ui->wifiReceiveRadioButton->setChecked(false);
        ui->browseReceiveFileButton->setEnabled(false);
    }
}

void MainWindow::on_wifiReceiveRadioButton_clicked()
{
    if(ui->wifiReceiveRadioButton->isChecked())
        ui->browseReceiveFileButton->setEnabled(true);
    else
        ui->browseReceiveFileButton->setEnabled(false);

}

void MainWindow::on_browseReceiveFileButton_clicked()
{
    QString directoryName = QFileDialog::getExistingDirectory(this,
                                                    tr("Select Directory"), QDir::homePath());


    if(!directoryName.isEmpty())
    {
        ui->selectedReceiveFileLineEdit->setText(directoryName);
        ui->receiveFileButton->setEnabled(true);
    }

}
