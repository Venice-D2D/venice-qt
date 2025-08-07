#ifndef FILETRANSFERSERVICEDISCOVERER_H
#define FILETRANSFERSERVICEDISCOVERER_H

#include "include/channel/bootstrapchannel.h"
#include "include/metadata/filemetadata.h"
#include "include/metadata/channelmetadata.h"
#include "include/receiver/filereceiver.h"

#include <QThread>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QLowEnergyCharacteristic>
#include <QBluetoothUuid>
#include <QByteArray>
#include <QString>


class FileTransferServiceDiscoverer: public QObject
{
public:
    /**
     * @brief FileTransferServiceDiscoverer Constructor of the class
     * @param dataChannel The Channel used for data transfer
     * @param bootstrapChannel The bootstrap used for service discovering
     * @param useProtobuf true indicates that protobuf is used for serialisation. If false, json is used instead
     */
    FileTransferServiceDiscoverer(DataChannel *dataChannel, BootstrapChannel *bootstrapChannel, bool useProtobuf);
    ~FileTransferServiceDiscoverer();

    /**
     * @brief runFileServiceConsumer Executes the bluetooth service discover
     */
    void runDiscoverer();

    /**
     * @brief setDirectoryPath Set the path related to the file to be stored
     * @param directoryPath The path to store the file
     */
    void setDirectoryPath(QString directoryPath);

private:

    /**
     * @brief blDiscoverer The ble device discoverer
     */
    QBluetoothDeviceDiscoveryAgent *bleDeviceDiscoverer= NULL;

    /**
     * @brief bleDeviceController The ble device controller
     */
    QLowEnergyController *bleDeviceController;


    /**
     * @brief dataChannel The object to configure and manage the data channel
     */
    DataChannel *dataChannel;

    /**
     * @brief bootstrapChannel The object to configure and manage the ble bootstrap channel
     */
    BootstrapChannel *bootstrapChannel;

    /**
     * @brief veniceFileService The File service
     */
    QLowEnergyService *veniceFileService;

    /**
     * @brief foundVeniceFileService Indicates if the venice service has been found
     */
    bool foundVeniceFileService;

    /**
     * @brief channelMetadata The metadata related to the data channel to receive the file
     */
    ChannelMetadata *channelMetadata = nullptr;

    /**
     * @brief fileMetadata The file metadata
     */
    FileMetadata *fileMetadata = nullptr;

    /**
     * @brief direactoryPath Path to store the received file
     */
    QString directoryPath;

    /**
     * @brief fileReceiver
     */
    FileReceiver *fileReceiver = nullptr;

    /**
     * @brief useProtobuf Use protobuf for data serialisation
     */
    bool useProtobuf = true;



    /**
     *
     * @brief readServiceCharacteristicByUuid Read a characteristic by using the given Uuid
     * @param characteristicUuid The characteristic Uuid
     */
    void readServiceCharacteristicByUuid(QBluetoothUuid characteristicUuid);

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
    void readServiceCharacteristics(QLowEnergyService::ServiceState state);


    /**
     * @brief processServiceDiscovered Determines if the discovered service is a
     * Venice service and if it is the case, it connects to it
     * @param discoveredService
     */
    void processServiceDiscovered(const QBluetoothUuid &discoveredService);


    /**
     * @brief onServiceCharacteristicRead Process values related to read characteristic
     * @param characteristic The read characteristic
     * @param value The read value
     */
    void processReadServiceCharacteristic(const QLowEnergyCharacteristic &characteristic, const QByteArray &value);




};

#endif // FILETRANSFERSERVICEDISCOVERER_H
