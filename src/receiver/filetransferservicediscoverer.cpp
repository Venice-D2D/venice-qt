#include "include/receiver/filetransferservicediscoverer.h"
#include "include/sender/venicebluetoothuuid.h"
#include "include/sender/filetransferserviceprovider.h"
#include "include/channel/blebootstrapchannel.h"
#include "include/channel/wifidatachannel.h"

#include <QDebug>
#include <QTimer>
#include <QProcess>
#include <QStringList>

FileTransferServiceDiscoverer::FileTransferServiceDiscoverer(DataChannel *dataChannel, BootstrapChannel *bootstrapChannel, bool useProtobuf) {
    this->dataChannel = dataChannel;
    this->bootstrapChannel = bootstrapChannel;
    this->foundVeniceFileService = false;

    this->useProtobuf = useProtobuf;

    this->dataChannel->configure();

}

FileTransferServiceDiscoverer::~FileTransferServiceDiscoverer() {

    delete this->bleDeviceDiscoverer;
    delete this->bootstrapChannel;
    delete this->dataChannel;
    delete this->bootstrapChannel;

    if(this->fileReceiver !=nullptr)
        delete this->fileReceiver;
}

void FileTransferServiceDiscoverer::setDirectoryPath(QString directoryPath){
    this->directoryPath = directoryPath;
}

void FileTransferServiceDiscoverer::runDiscoverer(){

    this->bootstrapChannel->configure();

    BleBootstrapChannel *bleBootstrapChannel = dynamic_cast<BleBootstrapChannel*>(this->bootstrapChannel);

    this->bleDeviceDiscoverer = new QBluetoothDeviceDiscoveryAgent(bleBootstrapChannel->getSelectedLocalBLEAdapter()->address(),this);
    this->bleDeviceDiscoverer->setLowEnergyDiscoveryTimeout(0);

    connect(this->bleDeviceDiscoverer, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &FileTransferServiceDiscoverer::processDiscoveredDevice);

    connect(this->bleDeviceDiscoverer, &QBluetoothDeviceDiscoveryAgent::errorOccurred,
            this, &FileTransferServiceDiscoverer::dealWithDiscoveryError);

    connect(this->bleDeviceDiscoverer, &QBluetoothDeviceDiscoveryAgent::finished, this, &FileTransferServiceDiscoverer::dealWithFinishedDiscovery);
    connect(this->bleDeviceDiscoverer, &QBluetoothDeviceDiscoveryAgent::canceled, this, &FileTransferServiceDiscoverer::dealWithFinishedDiscovery);
    this->bleDeviceDiscoverer->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

void FileTransferServiceDiscoverer::processDiscoveredDevice(const QBluetoothDeviceInfo &discoveredDevice){

    qDebug() << "[FileTransferServiceDiscoverer] Device found - name: " << discoveredDevice.name() << " address: " << discoveredDevice.address().toString();

    BleBootstrapChannel *bleBootstrapChannel = dynamic_cast<BleBootstrapChannel*>(this->bootstrapChannel);

    if (discoveredDevice.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration && bleBootstrapChannel->getSelectedLocalBLEAdapter()->address().toString() != discoveredDevice.address().toString() && discoveredDevice.name() == VENICE_DEVICE_NAME) {

        qDebug() << "[FileTransferServiceDiscoverer] Connecting to BLE device... " << discoveredDevice.name();

        this->bleDeviceDiscoverer->stop();

        this->bleDeviceController = QLowEnergyController::createCentral(discoveredDevice, this);

        this->bleDeviceController->setRemoteAddressType(QLowEnergyController::RandomAddress);

        QObject::connect(this->bleDeviceController, QOverload<QLowEnergyController::Error>::of(&QLowEnergyController::errorOccurred),
                         [=](QLowEnergyController::Error error) {
                             qWarning() << "[FileTransferServiceDiscoverer] ❌ Controller error:" << error;
                         });

        QObject::connect(this->bleDeviceController, &QLowEnergyController::stateChanged,
                         [=](QLowEnergyController::ControllerState state) {
                             qDebug() << "[FileTransferServiceDiscoverer] BLEDeviceController State changed to:" << state;
                         });

        QObject::connect(this->bleDeviceController, &QLowEnergyController::discoveryFinished, [this]() {

            qDebug() << "[FileTransferServiceDiscoverer]  BLEDeviceController Service discovery finished.";

            QTimer::singleShot(250, [this]() {
                qDebug() << "Ending connection with device...";
            });

        });

        qDebug() << "[FileTransferServiceDiscoverer] isValid:" << discoveredDevice.isValid();
        qDebug() << "[FileTransferServiceDiscoverer] isCached:" << discoveredDevice.isCached();

        connect(this->bleDeviceController, &QLowEnergyController::serviceDiscovered, this, &FileTransferServiceDiscoverer::processServiceDiscovered);
        connect(this->bleDeviceController, &QLowEnergyController::connected, this, &FileTransferServiceDiscoverer::discoverServices);
        QObject::connect(this->bleDeviceController, &QLowEnergyController::disconnected, []() {
            qDebug() << "[FileTransferServiceDiscoverer] ❌ Disconnected from BLE device.";
        });

        qDebug() << "[FileTransferServiceDiscoverer] Controller created ! ";

        this->bleDeviceController->connectToDevice();


        qDebug() << "[FileTransferServiceDiscoverer] Connecting to BLE device done !";
    }
    else
        qDebug() << "[FileTransferServiceDiscoverer] Ignoring BLE Device...";


}

void FileTransferServiceDiscoverer::processServiceDiscovered(const QBluetoothUuid &discoveredService)
{
    if (discoveredService == VeniceBluetoothUuid::getVeniceFileServiceClassUuid()) {
        qDebug() << "[FileTransferServiceDiscoverer] Venice service discovered. Waiting for service scan to be done...";

        this->foundVeniceFileService = true;

        this->veniceFileService = this->bleDeviceController->createServiceObject(discoveredService); //this);
        if (this->veniceFileService) {
            qDebug() << "[FileTransferServiceDiscoverer] QLowEnergyService Service object created successfully!";
            connect(this->veniceFileService, &QLowEnergyService::stateChanged, this, &FileTransferServiceDiscoverer::readServiceCharacteristics);


            qDebug() << "[FileTransferServiceDiscoverer] Discovering Service object details!";
            connect(this->veniceFileService, &QLowEnergyService::characteristicRead,
                    this, &FileTransferServiceDiscoverer::processReadServiceCharacteristic);
                this->veniceFileService->discoverDetails();
        }

    }
    else {
        qDebug() << "[FileTransferServiceDiscoverer] No a Venice Service... " << discoveredService;
    }
}

void FileTransferServiceDiscoverer::discoverServices() {
    qDebug() << "[FileTransferServiceDiscoverer] Connected to BLE device! Looking for services...";
    this->bleDeviceController->discoverServices();
}

void FileTransferServiceDiscoverer::filterDiscoveredServices() {
    qDebug() << "[FileTransferServiceDiscoverer] Provided services:";
    for (const auto &service : this->bleDeviceController->services()) {
        qDebug() << service.toString();
    }

    qDebug() << "[FileTransferServiceDiscoverer] Filtering Venice Service";



    for (const auto &service : this->bleDeviceController->services()) {
        if (service == VeniceBluetoothUuid::getVeniceFileServiceClassUuid()) {  // Filter by specific UUID
            qDebug() << "[FileTransferServiceDiscoverer] Found Venice Service: " << service.toString();
            this->veniceFileService = this->bleDeviceController->createServiceObject(service); //this);
            if (this->veniceFileService) {
                qDebug() << "[FileTransferServiceDiscoverer] Service object created successfully!";
                connect(this->veniceFileService, &QLowEnergyService::stateChanged, this, &FileTransferServiceDiscoverer::readServiceCharacteristics);

                QTimer::singleShot(1000, [this]() {
                    qDebug() << "[FileTransferServiceDiscoverer] Discovering Service object details!";
                    veniceFileService->discoverDetails();
                });
            }
        }
}
}

void FileTransferServiceDiscoverer::dealWithFinishedDiscovery() {
    qDebug() << "[FileTransferServiceDiscoverer] Discovery finished !";
}

void FileTransferServiceDiscoverer::dealWithDiscoveryError(){
    qDebug() << "[FileTransferServiceDiscoverer] Discovery finished.";

}

void FileTransferServiceDiscoverer::readServiceCharacteristics(QLowEnergyService::ServiceState state){

    qDebug() << "[FileTransferServiceDiscoverer] Service State: "<< state;
    if (state == QLowEnergyService::RemoteServiceDiscovered) {
        this->readServiceCharacteristicByUuid(VeniceBluetoothUuid::getFileChacteristicType());
        this->readServiceCharacteristicByUuid(VeniceBluetoothUuid::getWifiChannelCharacteristicType());
    }

}

void FileTransferServiceDiscoverer::processReadServiceCharacteristic(const QLowEnergyCharacteristic &characteristic, const QByteArray &characteristicValueBytes) {
    QString characteristicValueString(characteristicValueBytes);
    qDebug() << "[FileTransferServiceDiscoverer] Characteristic: " << characteristic.uuid() << "read value:" << characteristicValueBytes.toHex() << characteristicValueString;

    QStringList characteristValuesList = characteristicValueString.split(";");

    if(characteristic.uuid()==VeniceBluetoothUuid::getFileChacteristicType() && characteristValuesList.size()==3)
    {
        this->fileMetadata = new FileMetadata(characteristValuesList.at(0), characteristValuesList.at(1).toInt(), characteristValuesList.at(2).toInt(), this->directoryPath);

    }
    else if(characteristic.uuid()==VeniceBluetoothUuid::getWifiChannelCharacteristicType() && characteristValuesList.size()==5)
    {
        this->channelMetadata = new ChannelMetadata(characteristValuesList.at(0), characteristValuesList.at(1), characteristValuesList.at(2),"", characteristValuesList.at(4).toInt());
    }
    else
    {
        qWarning() << "[FileTransferServiceDiscoverer] Unknown Characteristic" << characteristValuesList;

    }

    //We check if whole information to retrive the file is available
    if(this->fileMetadata!=nullptr && this->channelMetadata!=nullptr)
    {
        //We check that used network is the same the one used by the file provider as well as the channel type
        qDebug() << "[FileTransferServiceDiscoverer] Metadata OK";
        WifiDataChannel *wifiDataChannel = dynamic_cast<WifiDataChannel*>(this->dataChannel);
        qDebug() << "[FileTransferServiceDiscoverer] metadata info - Network Identifier "<< this->channelMetadata->getNetworkIdentifier() << " - Channel identifier " << this->channelMetadata->getChannelIdentifier();
        qDebug() << "[FileTransferServiceDiscoverer] expected info - SSID " << wifiDataChannel->getSSID() << " - Channel Identifier " << FileTransferServiceProvider::WIFI_DATA_CHANNEL;

        if(wifiDataChannel->getSSID()==this->channelMetadata->getNetworkIdentifier() &&     FileTransferServiceProvider::WIFI_DATA_CHANNEL==this->channelMetadata->getChannelIdentifier())
        {
            this->fileReceiver = new FileReceiver(this, this->channelMetadata, this->fileMetadata, this->useProtobuf);
            this->fileReceiver->startFileReception();
        }

    }
}

void FileTransferServiceDiscoverer::readServiceCharacteristicByUuid(QBluetoothUuid characteristicUuid){

    //Ask for characteristic
    QLowEnergyCharacteristic characteristic = this->veniceFileService->characteristic(characteristicUuid);
    if (!characteristic.isValid()) {
        qWarning() << "[FileTransferServiceDiscoverer] Characteristic not found: "<< characteristicUuid;
    }

    this->veniceFileService->readCharacteristic(characteristic);  // Will emit characteristicRead()
    qDebug() << "[FileTransferServiceDiscoverer] Read with error: "<< this->veniceFileService->error();

}
