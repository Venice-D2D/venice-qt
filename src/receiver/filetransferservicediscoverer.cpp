#include "include/receiver/filetransferservicediscoverer.h"
#include "include/sender/venicebluetoothuuid.h"
#include "include/sender/filetransferserviceprovider.h"
#include "include/channel/blebootstrapchannel.h"
#include "include/channel/wifidatachannel.h"

#include <QDebug>
#include <QTimer>
#include <QProcess>
#include <QStringList>

FileTransferServiceDiscoverer::FileTransferServiceDiscoverer(DataChannel *dataChannel, BootstrapChannel *bootstrapChannel) {
    this->dataChannel = dataChannel;
    this->bootstrapChannel = bootstrapChannel;
    this->foundVeniceFileService = false;

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
    //this->bleDeviceDiscoverer->setInquiryType(QBluetoothDeviceDiscoveryAgent::GeneralUnlimitedInquiry);

    connect(this->bleDeviceDiscoverer, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &FileTransferServiceDiscoverer::processDiscoveredDevice);

    /*connect(this->bleDeviceDiscoverer, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, [=](const QBluetoothDeviceInfo &discoveredDevice) {
                if (discoveredDevice.name() == VENICE_DEVICE_NAME) {

                    QProcess::execute("bluetoothctl", {"info", discoveredDevice.address().toString()});

                    QBluetoothDeviceInfo discoveredDeviceBis(discoveredDevice);
                    discoveredDeviceBis.setCached(true);

                    if (!discoveredDeviceBis.isCached()) {
                        qDebug() << "Device not cached, restarting scan...";
                        bleDeviceDiscoverer->stop();
                        //QTimer::singleShot(500, this, [&]() {
                            bleDeviceDiscoverer->start();
                        //});
                        return;
                    }

                    qDebug() << "Cached device found:" << discoveredDeviceBis.address();
                    bleDeviceDiscoverer->stop();

                    QLowEnergyController *controller =
                        QLowEnergyController::createCentral(discoveredDeviceBis, this);
                    controller->setRemoteAddressType(QLowEnergyController::RandomAddress);
                    connect(controller, &QLowEnergyController::connected, this, [] {
                        qDebug() << "✅ Connected!";
                    });
                    connect(controller, &QLowEnergyController::errorOccurred, this,
                            [](QLowEnergyController::Error error) {
                                qWarning() << "Controller error:" << error;
                            });

                    controller->connectToDevice();
                }
            });*/

    connect(this->bleDeviceDiscoverer, &QBluetoothDeviceDiscoveryAgent::errorOccurred,
            this, &FileTransferServiceDiscoverer::dealWithDiscoveryError);

    connect(this->bleDeviceDiscoverer, &QBluetoothDeviceDiscoveryAgent::finished, this, &FileTransferServiceDiscoverer::dealWithFinishedDiscovery);
    connect(this->bleDeviceDiscoverer, &QBluetoothDeviceDiscoveryAgent::canceled, this, &FileTransferServiceDiscoverer::dealWithFinishedDiscovery);
    this->bleDeviceDiscoverer->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

void FileTransferServiceDiscoverer::processDiscoveredDevice(const QBluetoothDeviceInfo &discoveredDevice){

    qDebug() << "[FileTransferServiceDiscoverer] Device found - name: " << discoveredDevice.name() << " address: " << discoveredDevice.address().toString();
    //qDebug() << "Current selected BLE adapter - address: " << this->bleBootstrapChannel->getSelectedLocalBLEAdapter()->address().toString();
    BleBootstrapChannel *bleBootstrapChannel = dynamic_cast<BleBootstrapChannel*>(this->bootstrapChannel);

    if (discoveredDevice.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration && bleBootstrapChannel->getSelectedLocalBLEAdapter()->address().toString() != discoveredDevice.address().toString() && discoveredDevice.name() == VENICE_DEVICE_NAME) {
        QProcess::execute("bluetoothctl", {"info", discoveredDevice.address().toString()});
        qDebug() << "[FileTransferServiceDiscoverer] Connecting to BLE device... " << discoveredDevice.name();

        //QProcess bluetoothCtlDeviceConnectionProcess;

        /*QProcess::execute("bash", QStringList() << "-c" << "bluetoothctl <<EOF\n"
                                                           "power on\n"
                                                           "scan on\n"
                                                           "trust "+ discoveredDevice.address().toString()+"\n"
                                                           "connect "+ discoveredDevice.address().toString()+"\n"
                                                           "EOF\n"
                                                            "echo \"Waiting for venice device to connect...\"\n"
                                                            "for i in {1..10}; do\n"
                                                                "STATUS_DEVICE=$(bluetoothctl info "+discoveredDevice.address().toString()+" | grep \" Connected: yes\")\n"
                                                                "if [[ -n \"$STATUS_DEVICE\" ]]; then\n"
                                                                "    echo \"Venice Device connected!\"\n"
                                                                    "break\n"
                                                                "fi\n"
                                                                "sleep 1\n"
                                                            "done\n"
                                                            //"bluetoothctl <<EOF\n"
                                                            //"menu gatt\n"
                                                            //"list-attributes\n"
                                                            //"select-attribute 00002a90-0000-1000-8000-00805f9b34fb\n"
                                                            //"read\n"
                                                            //"EOF\n"
                                                           "bluetoothctl <<EOF\n"
                                                           "disconnect "+discoveredDevice.address().toString()+"\n"
                                                           "scan off\n"
                                                           "quit\n"
                                                           "EOF\n"
                                                           );*/

        //bluetoothCtlDeviceConnectionProcess.waitForFinished(-1);

        this->bleDeviceDiscoverer->stop();

        /*if(this->bleDeviceController!=NULL)
            this->bleDeviceController->deleteLater();*/

        this->bleDeviceController = QLowEnergyController::createCentral(discoveredDevice, this);

        this->bleDeviceController->setRemoteAddressType(QLowEnergyController::RandomAddress);


        /*QObject::connect(this->bleDeviceController, &QLowEnergyController::connected, []() {
            qDebug() << "Connected to device!";
        });

        QObject::connect(this->bleDeviceController, &QLowEnergyController::disconnected, []() {
            qDebug() << "Disconnected.";
        });

        QObject::connect(this->bleDeviceController, &QLowEnergyController::serviceDiscovered,
                         [](const QBluetoothUuid &uuid) {
                             qDebug() << "Service discovered:" << uuid;
                         });

        QObject::connect(this->bleDeviceController, &QLowEnergyController::discoveryFinished, []() {
            qDebug() << "Service discovery finished.";
        });*/

        QObject::connect(this->bleDeviceController, QOverload<QLowEnergyController::Error>::of(&QLowEnergyController::errorOccurred),
                         [=](QLowEnergyController::Error error) {
                             qWarning() << "[FileTransferServiceDiscoverer] ❌ Controller error:" << error;

            /*if (error == QLowEnergyController::UnknownRemoteDeviceError) {
                qWarning() << "Retrying connection after UnknownRemoteDeviceError";
                qDebug() << "Connecting to device again !";
                bleDeviceController->connectToDevice();*/
                /*QTimer::singleShot(1000, this, [&]() {
                    qDebug() << "Connecting to device again !";
                    bleDeviceController->connectToDevice();
                });*/
            //}

            /*qWarning() << "⚠️ Failed with RandonAddress, retrying with PublicAddress...";
            auto retry = QLowEnergyController::createCentral(discoveredDevice);
            retry->setRemoteAddressType(QLowEnergyController::RandomAddress);
            retry->connectToDevice();*/

            /*QObject::connect(retry, &QLowEnergyController::connected, []() {
                qDebug() << "✅ Connected successfully with RandomAddress";
            });

            QObject::connect(retry, &QLowEnergyController::errorOccurred, [](QLowEnergyController::Error error) {
                qWarning() << "❌ Retry failed too: " << error;
            });*/

                         });

        QObject::connect(this->bleDeviceController, &QLowEnergyController::stateChanged,
                         [=](QLowEnergyController::ControllerState state) {
                             qDebug() << "[FileTransferServiceDiscoverer] BLEDeviceController State changed to:" << state;
                         });

        QObject::connect(this->bleDeviceController, &QLowEnergyController::discoveryFinished, [this]() {

            qDebug() << "[FileTransferServiceDiscoverer]  BLEDeviceController Service discovery finished.";

            //this->veniceFileService = this->bleDeviceController->createServiceObject(VeniceBluetoothUuid::getVeniceFileServiceClassUuid());
            /*connect(this->veniceFileService, &QLowEnergyService::stateChanged, this, [=, this](QLowEnergyService::ServiceState state) {
                 qDebug() << "[FileTransferServiceDiscoverer] Service State changed to:" << state;
                if (state == QLowEnergyService::RemoteServiceDiscovered) {
                    auto ch = this->veniceFileService->characteristic(VeniceBluetoothUuid::getWifiChannelCharacteristicType());
                    if (ch.properties() & QLowEnergyCharacteristic::Read) {
                        this->veniceFileService->readCharacteristic(ch);
                    } else {
                        qWarning() << "Characteristic not readable";
                    }
                }
            });*/
            //this->veniceFileService->discoverDetails();

            //if(this->foundVeniceFileService){
                //this->veniceFileService = this->bleDeviceController->createServiceObject(VeniceBluetoothUuid::getVeniceFileServiceClassUuid()); //this);

                /*if (this->veniceFileService) {
                    connect(this->veniceFileService, &QLowEnergyService::characteristicRead,
                            this, &FileTransferServiceDiscoverer::onCharacteristicRead);
                    connect(this->veniceFileService, &QLowEnergyService::errorOccurred,
                            this, [](QLowEnergyService::ServiceError error) {
                                qWarning() << "Read error:" << error;
                            });
                    qDebug() << "[FileTransferServiceDiscoverer] Processing Characteristic!";
                    QLowEnergyCharacteristic charac = this->veniceFileService->characteristic(VeniceBluetoothUuid::getWifiChannelCharacteristicType());
                    while (!charac.isValid()) {
                        qDebug() << "[FileTransferServiceDiscoverer] Trying to get characteristic information...";
                        charac = this->veniceFileService->characteristic(VeniceBluetoothUuid::getWifiChannelCharacteristicType());
                        //qWarning() << "[FileTransferServiceDiscoverer] Characteristic not found!";
                        //return;
                    }



                     qDebug() << "[FileTransferServiceDiscoverer] Reading chractertistic..."<< charac.value().toStdString();

                    this->veniceFileService->readCharacteristic(charac);
                    qDebug() << "[FileTransferServiceDiscoverer] Read at the end with error: "<< this->veniceFileService->error();
                    //connect(this->veniceFileService, &QLowEnergyService::stateChanged, this, &FileTransferServiceDiscoverer::consumeService);
                    //this->veniceFileService->discoverDetails();
                }*/
            //}

            QTimer::singleShot(250, [this]() {
                qDebug() << "Ending connection with device...";
            });

        });

        qDebug() << "[FileTransferServiceDiscoverer] isValid:" << discoveredDevice.isValid();
        qDebug() << "[FileTransferServiceDiscoverer] isCached:" << discoveredDevice.isCached();



        /*connect(this->bleDeviceController, &QLowEnergyController::stateChanged, [](QLowEnergyController::ControllerState state) {
            qDebug() << "[FileTransferServiceDiscoverer] Controller state changed to:" << state;
            this->bleDeviceController->close(;)
        });*/

        connect(this->bleDeviceController, &QLowEnergyController::serviceDiscovered, this, &FileTransferServiceDiscoverer::processServiceDiscovered);
        connect(this->bleDeviceController, &QLowEnergyController::connected, this, &FileTransferServiceDiscoverer::discoverServices);
        QObject::connect(this->bleDeviceController, &QLowEnergyController::disconnected, []() {
            qDebug() << "[FileTransferServiceDiscoverer] ❌ Disconnected from BLE device.";
        });
        //connect(this->bleDeviceController, &QLowEnergyController::discoveryFinished, this, &FileTransferServiceDiscoverer::filterDiscoveredServices);

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

            //QTimer::singleShot(250, [this]() {
                qDebug() << "[FileTransferServiceDiscoverer] Discovering Service object details!";
            connect(this->veniceFileService, &QLowEnergyService::characteristicRead,
                    this, &FileTransferServiceDiscoverer::processReadServiceCharacteristic);
                this->veniceFileService->discoverDetails();
            //});
        }

    }
    else {
        qDebug() << "[FileTransferServiceDiscoverer] No a Venice Service... " << discoveredService;
    }
}

void FileTransferServiceDiscoverer::discoverServices() {
    qDebug() << "[FileTransferServiceDiscoverer] Connected to BLE device! Looking for services...";
    //QTimer::singleShot(250, [this]() {
        this->bleDeviceController->discoverServices();
    //});
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



                /*QList<QLowEnergyCharacteristic> characteristics = this->veniceFileService->characteristics();
                for(QLowEnergyCharacteristic characteristic: characteristics)
                {
                    qDebug() << "[FileTransferServiceDiscoverer] Characteristic Name: "<< characteristic.name();
                    qDebug() << "[FileTransferServiceDiscoverer] Characteristic Value: "<< QString(characteristic.value());
                }*/

                //this->bleDeviceDiscoverer->stop();
                //delete this->bleDeviceDiscoverer;
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
            this->fileReceiver = new FileReceiver(this, this->channelMetadata, this->fileMetadata);
            this->fileReceiver->startFileReception();
        }

    }

    // Check that both characterist are read and that the wifi data channel is working and connected to the required network. If it is not the case, error. This verification has to be donne also before all the process related to bluetooth.Then it is possible to start the receiver and the socket that will read the file
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
//Todo method to launh characteristic reading with ID as param and called from consume service
//Todo onCharacteristicRead to
