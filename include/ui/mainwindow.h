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

#include "include/sender/filetransferserviceprovider.h"
#include "include/receiver/filetransferservicediscoverer.h"
//#include "include/ui/venicedevicesdialog.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class VeniceDevicesDialog;

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

    /**
     * @brief useSelectedDeviceForRetrieval Start the reception of the file by using the current device
     * @param device The ble device to be used for getting the file
     */
    void useSelectedDeviceForRetrieval(QBluetoothDeviceInfo *device);


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
    /**
     * @brief ui The main window
     */
    Ui::MainWindow *ui;

    /**
     * @brief veniceService The venice service thread
     */
    FileTransferServiceProvider *veniceService = nullptr;

    /**
     * @brief mainApplication The main application
     */
    QApplication *mainApplication = nullptr;

    /**
     * @brief bleDiscoverer The venice service discover
     */
    FileTransferServiceDiscoverer *bleDiscoverer = nullptr;

    /**
     * @brief venideDevicesDialog The dialog containing the found venice devices
     */
    VeniceDevicesDialog *veniceDevicesDialog = nullptr;

};
#endif // MAINWINDOW_H
