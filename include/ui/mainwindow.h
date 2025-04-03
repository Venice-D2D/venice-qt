#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QLowEnergyAdvertisingData>
#include <QLowEnergyController>
#include <QLowEnergyAdvertisingParameters>
#include <QLowEnergyCharacteristicData>
#include <QLowEnergyServiceData>
#include <QLowEnergyDescriptorData>
#include <QtCore/qtimer.h>

#include "include/service/filetransferservice.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief The MainWindow class is the entry point for the final user UI
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow constructor
     * @param parent The Parent of MainWindow
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief MainWindow destructor
     */
    ~MainWindow();

    /**
     * @brief setMainApplication defines the QT main application related to the window
     * @param application Tha main appliction
     */

    void setMainApplication(QApplication *application);


private slots:

    //The venice service thread

    /**
     * @brief on_sendFileButton_clicked starts a thread for advertising a service that accepts files transfer
     */
    void on_sendFileButton_clicked();

    /**
     * @brief doPopup
     */
    void doPopup();

    /**
     * @brief on_brownseSendFileButton_clicked shows a browser file windows for selecting a file to be sent
     */
    void on_browseSendFileButton_clicked();



    /**
     * @brief on_bleSendRadioButton_clicked enables/disables the wifi radio button
     */
    void on_bleSendRadioButton_clicked();

    /**
     * @brief on_wifiSendRadioButton_clicked enables/disables the send file button
     */
    void on_wifiSendRadioButton_clicked();

    //Receive Panel

    /**
     * @brief on_receiveFileButton_clicked starts the ble device discoverer
     */
    void on_receiveFileButton_clicked();

    /**
     * @brief on_bleReceiveSendRadioButton enables/disables the receive file button
     */
    void on_bleReceiveRadioButton_clicked();

    /**
     * @brief on_wifiReceiveRadioButton_clicked enables/disables the receive file button
     */
    void on_wifiReceiveRadioButton_clicked();

    /**
     * @brief on_browseReceiveFileButton_clicked shows a browser file windows for selecting a directory to store the file
     */
    void on_browseReceiveFileButton_clicked();


private:
    //The main window
    Ui::MainWindow *ui;

    //The venice service thread
    FileTransferService *veniceService= nullptr;

    //The main application
    QApplication *mainApplication= nullptr;

};
#endif // MAINWINDOW_H
