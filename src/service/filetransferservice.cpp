#include "include/service/filetransferservice.h"
#include "include/service/filetransferserver.h"


#include <QtDebug>
#include <QFileDialog>
#include <QLowEnergyAdvertisingData>
#include <QLowEnergyController>
#include <QLowEnergyAdvertisingParameters>
#include <QLowEnergyCharacteristicData>
#include <QLowEnergyServiceData>
#include <QLowEnergyDescriptorData>
#include <QtCore/qtimer.h>
#include <QMetaMethod>

#include <filesystem>
#include <iostream>
#include <fstream>

using namespace std;

FileTransferService::FileTransferService(DataChannel *dataChannel, BleBootstrapChannel *bleBoostrapChannel, string filePath, QObject *parent): QThread(parent)
{
    this->filePath = filePath;
    this->dataChannel = dataChannel;
    this->bleBootstrapChannel = bleBoostrapChannel;
}

FileTransferService::~FileTransferService()
{
    delete(this->dataChannel);
    delete(this->bleBootstrapChannel);
    delete(this->fileTransferServer);
}

void FileTransferService::run()
{
    this->runFileServiceProvider();

    qDebug() << "Finishing VeniceService Thread";
}

void FileTransferService::runFileServiceProvider()
{

    //Choose a suitable bluetooth adapter
    this->configureBoostrapChannel();

    //The file is only advertised if it exists
    qDebug() << "Checking file: " << this->filePath.c_str();
    if(filesystem::exists(this->filePath))
    {
        qDebug() << "Configuring data channel...";

        this->configureDataChannel();

        qDebug() << "Starting File Transfer Service...";

        int msgMaxSize = MAX_MESSAGE_SIZE;
        QVector<VeniceMessage*> fileMessages = this->readFileData(filePath, msgMaxSize);

        WifiDataChannel* wifiDataChannel = dynamic_cast<WifiDataChannel*>(this->dataChannel);


        if(this->fileTransferServer == nullptr)
            this->fileTransferServer= new FileTransferServer(nullptr, fileMessages, wifiDataChannel->getNetworkAddress().ip(), wifiDataChannel->getPort(), this);

        else if(!this->fileTransferServer->isListening())
        {
            this->fileTransferServer->listen();
        }

        qDebug() << "Configuring BLE advertisement...";

        //File Characteristic creation
        qDebug() << "Creating File Characteristic";

        QLowEnergyCharacteristicData fileChacteristic;
        fileChacteristic.setUuid(VeniceBluetoothUuid::getFileChacteristicType());

        string fileName = filesystem::path(filePath).filename(); //File Name

         //Venice messages related to the file

        string fileCharacteristicValue = fileName+";"+to_string(msgMaxSize)+";"+to_string(fileMessages.size()); //Characteristic value has the format <fileName>;<max_message_size;<number_of_messages>

        fileChacteristic.setValue(QByteArray(fileCharacteristicValue.c_str()));

        fileChacteristic.setProperties(QLowEnergyCharacteristic::PropertyType::Read); //We are only supposed to read

        QLowEnergyDescriptorData clientConfigFileCharacteristicDescriptor(QBluetoothUuid::DescriptorType::CharacteristicUserDescription,
                                                    QByteArray(fileCharacteristicValue.c_str()));
        clientConfigFileCharacteristicDescriptor.setReadPermissions(true);
        fileChacteristic.addDescriptor(clientConfigFileCharacteristicDescriptor);



        //Channel Characteristic
        qDebug() << "Creating Channel Characteristic";
        QLowEnergyCharacteristicData channelChacteristic;
        channelChacteristic.setUuid(VeniceBluetoothUuid::getWifiChannelCharacteristicType());
        channelChacteristic.setProperties(QLowEnergyCharacteristic::PropertyType::Read);
        //TODO Update according the code for enabling exchange via wifi
        string channelCharacteristicValue = WIFI_DATA_CHANNEL+";"+wifiDataChannel->getNetworkAddress().ip().toString().toStdString()+";"+wifiDataChannel->getSSID().toStdString()      +";"+std::to_string(wifiDataChannel->getPort()); // chanel identifier, address, ssid (Ap identifier), key/port
        channelChacteristic.setValue(QByteArray(channelCharacteristicValue.c_str()));
        QLowEnergyDescriptorData clientConfigChannelCharacteristicDescriptor(QBluetoothUuid::DescriptorType::CharacteristicUserDescription,
                                                    QByteArray(channelCharacteristicValue.c_str()));
        clientConfigChannelCharacteristicDescriptor.setReadPermissions(true);
        fileChacteristic.addDescriptor(clientConfigChannelCharacteristicDescriptor);


        //Service Data
        qDebug() << "Creating File Service data";

        QLowEnergyServiceData serviceData;
        serviceData.setType(QLowEnergyServiceData::ServiceType::ServiceTypePrimary);
        serviceData.setUuid(VeniceBluetoothUuid::getVeniceFileServiceClassUuid());

        serviceData.addCharacteristic(fileChacteristic);
        serviceData.addCharacteristic(channelChacteristic);

        //Advertisement data
        qDebug() << "Creating Advertisement data";

        QLowEnergyAdvertisingData advertisingData;
        advertisingData.setDiscoverability(QLowEnergyAdvertisingData::DiscoverabilityGeneral);
        advertisingData.setIncludePowerLevel(true);
        advertisingData.setLocalName("venice");
        advertisingData.setServices(QList<QBluetoothUuid>() << VeniceBluetoothUuid::getVeniceFileServiceClassUuid());

        //Service Data
        qDebug() << "Creating service Controller";
        const unique_ptr<QLowEnergyController> btController(QLowEnergyController::createPeripheral(this->bleBootstrapChannel->getSelectedLocalBLEAdapter()));

        //Creating File Service
        qDebug() << "Creating File Service";
        unique_ptr<QLowEnergyService> service(btController->addService(serviceData));


        qDebug() << "Starting Service Advertisement";


        qDebug() << "Starting BLE Advertising...";

        btController->startAdvertising(QLowEnergyAdvertisingParameters(), advertisingData,
                                       advertisingData);


        this->exec();

        btController->stopAdvertising();
    }

    qDebug() << "File Service execution ended ";
}

QVector<VeniceMessage*> FileTransferService::readFileData(const string& name, const int& max_size)
{
    ifstream inputFile(name, ios_base::binary);
    QVector<VeniceMessage*> messages;

    // Determine the length of the file by seeking
    // to the end of the file, reading the value of the
    // position indicator, and then seeking back to the beginning.
    inputFile.seekg(0, ios_base::end);
    int length = inputFile.tellg();
    inputFile.seekg(0, ios_base::beg);

    // Make a buffer of the exact size of the file and read the data into it.
    vector<byte> buffer(length);
    inputFile.read(reinterpret_cast<char*>(buffer.data()), length);

    // We divide the buffer in chunks with max_size as length
    int from_byte = 0;
    int current_position = 0;

    while(from_byte<=length-1)
    {

        int current_data_size = max_size;

        if((from_byte+max_size-1) > length)
        {

            current_data_size = length - from_byte;
        }

        vector<byte> current_data(current_data_size);

        copy(buffer.begin()+from_byte, buffer.begin()+from_byte+current_data_size, current_data.begin());

        VeniceMessage* current_message= new VeniceMessage(current_position, false, current_data);

        messages.push_back(current_message);

        current_position++;
        from_byte+= current_data_size;
    }

    inputFile.close();
    return messages;
}

void FileTransferService::configureDataChannel() throw()
{
    this->dataChannel->configure();
}

void FileTransferService::configureBoostrapChannel() throw()
{
    this->bleBootstrapChannel->configure();

}

