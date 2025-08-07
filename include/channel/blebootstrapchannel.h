#ifndef BLEBOOTSTRAPCHANNEL_H
#define BLEBOOTSTRAPCHANNEL_H

#include "include/channel/bootstrapchannel.h"


#include <QtDebug>
#include <QBluetoothHostInfo>
#include <QBluetoothLocalDevice>


static const QString VENICE_DEVICE_NAME = "venice";//"venice-xxxxx";

class BleBootstrapChannel: public BootstrapChannel
{
public:
    /**
     * @brief BleBootstrapChannel
     */
    BleBootstrapChannel();

    ~BleBootstrapChannel();

    /**
     * @brief configure Configures the bluetooth adapter
     * @throw NotBluetoothAdapterFoundVeniceException if a valid bluetooth adapter is found
     */
    void configure();


    /**
     * @brief getSelectedLocalBLEAdapter Returns the selected ble adapter
     * @return The selected local ble adapter
     */
    QBluetoothLocalDevice* getSelectedLocalBLEAdapter();

private:

    /**
     * @brief localBLEDevice The selected bluetooth low energy adapter
     */
    QBluetoothLocalDevice *selectedLocalBLEAdapter;

    /**
     * @brief checkBleAdapter Verifies that the BLE adapter is valid and UP
     * @return True is the ble adapter is valid, False otherwise
     */
    bool checkBleAdapter(QBluetoothLocalDevice &bleDevice);
};

#endif // BLEBOOTSTRAPCHANNEL_H
