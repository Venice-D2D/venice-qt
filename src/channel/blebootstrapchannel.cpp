#include "include/channel/blebootstrapchannel.h"
#include "include/exception/notbluetoothadapterfoundveniceexception.h"


BleBootstrapChannel::BleBootstrapChannel():BootstrapChannel()
{
}

void BleBootstrapChannel::configure()
{
    qDebug() << "Selecting a suitable bluetooth adapter devices...";

    QBluetoothLocalDevice defaultLocalBTDevice;

    if(defaultLocalBTDevice.address().isNull())
    {
        QList<QBluetoothHostInfo> localDevices = QBluetoothLocalDevice::allDevices();

        //We look for the first avaiable and power on adapter
        //TODO Filter by LE capability
        for (const QBluetoothHostInfo &device : localDevices) {
            qDebug() << "Adapter Name:" << device.name();
            qDebug() << "Adapter Address:" << device.address().toString();
            QBluetoothLocalDevice localBTDevice(device.address());

            if (this->checkBleAdapter(localBTDevice))
            {
                // We only use the device if it not turned off and if it is available
                localBTDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
                qDebug() << "Selected!";
                return;
            }

        }
    }
    else if(this->checkBleAdapter(defaultLocalBTDevice))
    {
        // We only use the device if it not turned off and if it is available
        defaultLocalBTDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
        qDebug() << "Default Adapter Name:" << defaultLocalBTDevice.name();
        qDebug() << "Default Adapter Address:" << defaultLocalBTDevice.address().toString();
        qDebug() << "Selected!";
        return;
    }

    throw NotBluetoothAdapterFoundVeniceException();
}

bool BleBootstrapChannel::checkBleAdapter(QBluetoothLocalDevice &bleDevice)
{
    return bleDevice.isValid() && bleDevice.hostMode() != QBluetoothLocalDevice::HostPoweredOff;
}
