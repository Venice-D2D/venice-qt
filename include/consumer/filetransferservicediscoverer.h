#ifndef FILETRANSFERSERVICEDISCOVERER_H
#define FILETRANSFERSERVICEDISCOVERER_H

#include "include/channel/blebootstrapchannel.h"

#include <QThread>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>

class FileTransferServiceDiscoverer: public QObject
{
public:
    FileTransferServiceDiscoverer(BleBootstrapChannel *bleBootstrapChannel);
    ~FileTransferServiceDiscoverer();

    /**
     * @brief runFileServiceConsumer Executes the bluetooth service discover
     */
    void runDiscoverer();

private:

    /**
     * @brief blDiscoverer The ble device discoverer
     */
    QBluetoothDeviceDiscoveryAgent *bleDeviceDiscoverer;

    /**
     * @brief bleDeviceController The ble device controller
     */
    QLowEnergyController *bleDeviceController;

    /**
     * @brief bootstrapChannel The object to configure and manage the ble bootstrap channel
     */
    BleBootstrapChannel *bleBootstrapChannel;








private slots:

    /**
     * @brief processDiscoveredDevice Determines is the discovered device is a BLE
     * device and if is the case, it connects to it
     * @param discoveredDevice The discovererd device
     */
    void processDiscoveredDevice(const QBluetoothDeviceInfo &discoveredDevice);

    /**
     * @brief discoverServices Searches for service once the connection with a BLE device is done
     */
    void discoverServices();

    /**
     * @brief filterDiscoveredServices Selects the venice service if available
     */
    void filterDiscoveredServices();


    /**
     * @brief dealWithSearchFinished Deals with a ble device search that has finished
     */
    void dealWithFinishedDiscovery();

    /**
     * @brief dealWithDiscoveryError Deals with a discovery error
     */
    void dealWithDiscoveryError();

    /**
     * @brief consumeService Use the service
     */
    void consumeService(QLowEnergyService::ServiceState state);




};

#endif // FILETRANSFERSERVICEDISCOVERER_H
