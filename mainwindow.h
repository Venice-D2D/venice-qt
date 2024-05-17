#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "veniceservice.h"

#include <QMainWindow>

#include <QLowEnergyAdvertisingData>
#include <QLowEnergyController>
#include <QLowEnergyAdvertisingParameters>
#include <QLowEnergyCharacteristicData>
#include <QLowEnergyServiceData>
#include <QLowEnergyDescriptorData>
#include <QtCore/qtimer.h>

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
     * @brief on_brownseSendFileButton_clicked show a browser file windows for selecting a file to be sent
     */
    void on_browseSendFileButton_clicked();

    /**
     * @brief on_receiveFileButton_clicked
     */
    void on_receiveFileButton_clicked();


private:
    //The main window
    Ui::MainWindow *ui;

    //The venice service thread
    VeniceService *veniceService= nullptr;

    //The main application
    QApplication *mainApplication= nullptr;

};
#endif // MAINWINDOW_H
