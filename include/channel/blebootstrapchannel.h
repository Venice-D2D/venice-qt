#ifndef BLEBOOTSTRAPCHANNEL_H
#define BLEBOOTSTRAPCHANNEL_H

#include "include/channel/bootstrapchannel.h"


#include <QtDebug>
#include <QBluetoothHostInfo>
#include <QBluetoothLocalDevice>


class BleBootstrapChannel: public BootstrapChannel
{
public:
    /**
     * @brief BleBootstrapChannel
     */
    BleBootstrapChannel();

    /**
     * @brief configure
     * @throw NotBluetoothAdapterFoundVeniceException if a valid bluetooth adapter is found
     */
    void configure();

private:

    /**
     * @brief checkBleAdapter Verifies that the BLE adapter is valid and UP
     * @return True is the ble adapter is valid, False otherwise
     */
    bool checkBleAdapter(QBluetoothLocalDevice &bleDevice);
};

#endif // BLEBOOTSTRAPCHANNEL_H
