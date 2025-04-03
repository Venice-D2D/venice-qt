#include "include/consumer/filetransferservicediscoverer.h"
#include "include/service/venicebluetoothuuid.h"

#include <QBluetoothDeviceDiscoveryAgent>
#include <QDebug>

FileTransferServiceDiscoverer::FileTransferServiceDiscoverer(BleBootstrapChannel *bleBootstrapChannel) {
    this->bleBootstrapChannel = bleBootstrapChannel;
}

FileTransferServiceDiscoverer::~FileTransferServiceDiscoverer() {

    delete this->bleDeviceDiscoverer;
    delete this->bleBootstrapChannel;
}


void FileTransferServiceDiscoverer::runDiscoverer(){

    this->bleBootstrapChannel->configure();

    this->bleDeviceDiscoverer = new QBluetoothDeviceDiscoveryAgent(this->bleBootstrapChannel->getSelectedLocalBLEAdapter()->address(),this);
    this->bleDeviceDiscoverer->setLowEnergyDiscoveryTimeout(5000);

    connect(this->bleDeviceDiscoverer, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &FileTransferServiceDiscoverer::processDiscoveredDevice);
    connect(this->bleDeviceDiscoverer, &QBluetoothDeviceDiscoveryAgent::errorOccurred,
            this, &FileTransferServiceDiscoverer::dealWithDiscoveryError);

    connect(this->bleDeviceDiscoverer, &QBluetoothDeviceDiscoveryAgent::finished, this, &FileTransferServiceDiscoverer::dealWithFinishedDiscovery);
    connect(this->bleDeviceDiscoverer, &QBluetoothDeviceDiscoveryAgent::canceled, this, &FileTransferServiceDiscoverer::dealWithFinishedDiscovery);
    this->bleDeviceDiscoverer->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

void FileTransferServiceDiscoverer::processDiscoveredDevice(const QBluetoothDeviceInfo &discoveredDevice){

    qDebug() << "Device found - name: " << discoveredDevice.name() << " address: " << discoveredDevice.address().toString();
    //qDebug() << "Current selected BLE adapter - address: " << this->bleBootstrapChannel->getSelectedLocalBLEAdapter()->address().toString();
    if (discoveredDevice.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration && this->bleBootstrapChannel->getSelectedLocalBLEAdapter()->address().toString() != discoveredDevice.address().toString() && discoveredDevice.name() == VENICE_DEVICE_NAME) {
        qDebug() << "Connecting to BLE device... " << discoveredDevice.name();
        this->bleDeviceController = QLowEnergyController::createCentral(discoveredDevice, this);
        connect(this->bleDeviceController, &QLowEnergyController::connected, this, &FileTransferServiceDiscoverer::discoverServices);
        connect(this->bleDeviceController, &QLowEnergyController::discoveryFinished, this, &FileTransferServiceDiscoverer::filterDiscoveredServices);
        this->bleDeviceController->connectToDevice();
    }
    else
        qDebug() << "Ignoring BLE Device...";


}

void FileTransferServiceDiscoverer::discoverServices() {
    qDebug() << "Connected to BLE device! Looking for services...";
    this->bleDeviceController->discoverServices();
}

void FileTransferServiceDiscoverer::filterDiscoveredServices() {
    qDebug() << "Provided services:";
    for (const auto &service : this->bleDeviceController->services()) {
        qDebug() << service.toString();
    }

    qDebug() << "Filtering Venice Service";



    for (const auto &service : this->bleDeviceController->services()) {
    if (service == VeniceBluetoothUuid::getVeniceFileServiceClassUuid()) {  // Filter by specific UUID
        qDebug() << "Found Venice Service: " << service.toString();
        QLowEnergyService *veniceService = bleDeviceController->createServiceObject(service, this);
        if (veniceService) {
            qDebug() << "Service object created successfully!";
            connect(veniceService, &QLowEnergyService::stateChanged, this, &FileTransferServiceDiscoverer::consumeService);
            veniceService->discoverDetails();
        }
    }
}
}

void FileTransferServiceDiscoverer::dealWithFinishedDiscovery() {
    qDebug() << "Discovery finished !";
}

void FileTransferServiceDiscoverer::dealWithDiscoveryError(){
    qDebug() << "Discovery finished.";

}

void FileTransferServiceDiscoverer::consumeService(QLowEnergyService::ServiceState state){

    if (state == QLowEnergyService::RemoteServiceDiscovered) {
        qDebug() << "Service details discovered!";
    }
}
