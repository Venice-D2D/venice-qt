#include "veniceservice.h"
#include "venicebluetoothuuid.h"

#include <QtDebug>
#include <QFileDialog>
#include <QLowEnergyAdvertisingData>
#include <QLowEnergyController>
#include <QLowEnergyAdvertisingParameters>
#include <QLowEnergyCharacteristicData>
#include <QLowEnergyServiceData>
#include <QLowEnergyDescriptorData>
#include <QtCore/qtimer.h>

#include <filesystem>
#include <iostream>
#include <fstream>

using namespace std;

VeniceService::VeniceService(QObject *parent, string filePath): QThread(parent)
{
    this->filePath = filePath;
    this->channel = new WifiDataChannel("192.168.1.1", "0.0.0.0", "255.255.255.0");
}

VeniceService::~VeniceService()
{
    this->channel->restoreChannelConfiguration();
    delete(this->channel);
}

void VeniceService::run()
{
    this->runFileServiceProvider();

    qDebug() << "Finishing VeniceService Thread";
}

void VeniceService::runFileServiceProvider()
{
    //The file is only advertised if it exists
    if(filesystem::exists(this->filePath))
    {
        qDebug() << "Configuring data channel...";

        this->configureDataChannel();

        qDebug() << "Configuring BLE advertisement...";

        //File Characteristic creation
        qDebug() << "Creating File Characteristic";

        QLowEnergyCharacteristicData fileChacteristic;
        fileChacteristic.setUuid(VeniceBluetoothUuid::getFileChacteristicType());

        string fileName = filesystem::path(filePath).filename(); //File Name
        int msgMaxSize = MAX_MESSAGE_SIZE;
        vector<VeniceMessage> messages = this->readFileData(filePath, msgMaxSize); //Venice messages related to the file

        string fileCharacteristicValue = fileName+";"+to_string(msgMaxSize)+";"+to_string(messages.size()); //Characteristic value has the format <fileName>;<max_message_size;<number_of_messages>

        fileChacteristic.setValue(QByteArray(fileCharacteristicValue.c_str()));

        fileChacteristic.setProperties(QLowEnergyCharacteristic::Read); //We are only supposed to read
        const QLowEnergyDescriptorData clientConfigFileCharacteristic(QBluetoothUuid::DescriptorType::CharacteristicUserDescription,
                                                    QByteArray(fileCharacteristicValue.c_str()));
        fileChacteristic.addDescriptor(clientConfigFileCharacteristic);


        //Channel Characteristic
        qDebug() << "Creating Channel Characteristic";
        QLowEnergyCharacteristicData channelChacteristic;
        channelChacteristic.setUuid(VeniceBluetoothUuid::getWifiChannelCharacteristicType());
        channelChacteristic.setProperties(QLowEnergyCharacteristic::Read);
        //TODO Update accoring the code for enabling exchange via wifi
        string channelCharacteristicValue = WIFI_DATA_CHANNEL+";192.168.1.2;"+this->channel->getAdhocNetworkName()+";0"; // chanel identifier, address, ssid (Ap identifier), key
        channelChacteristic.setValue(QByteArray(channelCharacteristicValue.c_str()));
        const QLowEnergyDescriptorData clientConfigChannelCharacteristic(QBluetoothUuid::DescriptorType::CharacteristicUserDescription,
                                                    QByteArray(channelCharacteristicValue.c_str()));
        fileChacteristic.addDescriptor(clientConfigChannelCharacteristic);


        //Service Data
        qDebug() << "Creating File Service data";

        QLowEnergyServiceData serviceData;
        serviceData.setType(QLowEnergyServiceData::ServiceTypePrimary);
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
        const unique_ptr<QLowEnergyController> btController(QLowEnergyController::createPeripheral());

        //Creating File Service
        qDebug() << "Creating File Service";
        unique_ptr<QLowEnergyService> service(btController->addService(serviceData));


        qDebug() << "Starting Service Advertiement";
        btController->startAdvertising(QLowEnergyAdvertisingParameters(), advertisingData,
                                       advertisingData);


        //Signal-Slots are only required is a write capabily is related to the service
        //QTimer veniceTimer;

        //const auto veniceProvider = [&service]() {
        //};
        //this->connect(&veniceTimer, &QTimer::timeout, veniceProvider);
        //veniceTimer.start(1000);

        this->exec();

        btController->stopAdvertising();
    }

    qDebug() << "File Service execution ended ";
}

vector<VeniceMessage> VeniceService::readFileData(const string& name, const int& max_size)
{

    ifstream inputFile(name, ios_base::binary);
    vector<VeniceMessage> messages;

    // Determine the length of the file by seeking
    // to the end of the file, reading the value of the
    // position indicator, and then seeking back to the beginning.
    inputFile.seekg(0, ios_base::end);
    auto length = inputFile.tellg();
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

        VeniceMessage current_message(current_position, false, current_data);

        messages.push_back(current_message);

        current_position++;
        from_byte+= current_data_size;
    }

    inputFile.close();
    return messages;
}

void VeniceService::configureDataChannel()
{
    this->channel->configureChannel();
}

