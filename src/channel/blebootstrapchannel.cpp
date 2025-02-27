#include "include/channel/blebootstrapchannel.h"

#include <QtDebug>
#include <QBluetoothHostInfo>
#include <QBluetoothLocalDevice>

BleBootstrapChannel::BleBootstrapChannel():BootstrapChannel()
{
}

void BleBootstrapChannel::configure()
{
    qDebug() << "Selecting a suitable bluetooth adapter devices...";
    QList<QBluetoothHostInfo> localDevices = QBluetoothLocalDevice::allDevices();

    //We look for the first avaiable and power on adapter
    //TODO Filter by LE capability
    for (const QBluetoothHostInfo &device : localDevices) {
        qDebug() << "Adapter Name:" << device.name();
        qDebug() << "Adapter Address:" << device.address().toString();
        QBluetoothLocalDevice localBTDevice(device.address());

        if (localBTDevice.isValid())
            if (localBTDevice.hostMode() != QBluetoothLocalDevice::HostPoweredOff)
            {
                // We only use the device if it not turned off and if it is available
                localBTDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
                qDebug() << "Selected!";
                return;
            }
    }

    throw NotBluetoothAdapterFoundVeniceException();
}
