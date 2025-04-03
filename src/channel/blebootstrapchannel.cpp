#include "include/channel/blebootstrapchannel.h"
#include "include/exception/notbluetoothadapterfoundveniceexception.h"


BleBootstrapChannel::BleBootstrapChannel():BootstrapChannel()
{
}


BleBootstrapChannel::~BleBootstrapChannel()
{
    BootstrapChannel::~BootstrapChannel();
    if(this->selectedLocalBLEAdapter)
        delete this->selectedLocalBLEAdapter;
}


void BleBootstrapChannel::configure()
{
    qDebug() << "Selecting a suitable bluetooth adapter device...";


    QList<QBluetoothHostInfo> localDevices = QBluetoothLocalDevice::allDevices();

    qDebug() << "BT devices size: "<< localDevices.size();
    //We look for the first avaiable and power on adapter
    //TODO Filter by LE capability
    for(int current_device_index = localDevices.size()-1; current_device_index >=0; current_device_index--){
        const QBluetoothHostInfo &device = localDevices.at(current_device_index);
        qDebug() << "Adapter Name:" << device.name();
        qDebug() << "Adapter Address:" << device.address().toString();
        QBluetoothLocalDevice localBTDevice(device.address());

        if(this->checkBleAdapter(localBTDevice))
        {
            // We only use the device if it not turned off and if it is available
            localBTDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
            this->selectedLocalBLEAdapter = new QBluetoothLocalDevice(device.address());
            qDebug() << "Selected!";
            return;
        }

    }


    throw NotBluetoothAdapterFoundVeniceException();
}

bool BleBootstrapChannel::checkBleAdapter(QBluetoothLocalDevice &bleDevice)
{
    return bleDevice.isValid() && bleDevice.hostMode() != QBluetoothLocalDevice::HostPoweredOff;
}


QBluetoothLocalDevice* BleBootstrapChannel::getSelectedLocalBLEAdapter(){
    return this->selectedLocalBLEAdapter;
}

