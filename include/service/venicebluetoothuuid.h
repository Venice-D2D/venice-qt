#ifndef VENICEBLUETOOTHUUID_H
#define VENICEBLUETOOTHUUID_H

#include <QBluetoothUuid>//;

/**
 * @brief The VeniceBluetoothUuid class is a wrapper for venice uuid values
 */
class VeniceBluetoothUuid: public QBluetoothUuid
{
public:
    /**
     * @brief VeniceBluetoothUuid constructor of the class
     */
    VeniceBluetoothUuid();

    /**
     * @brief getFileChacteristicType
     * @return The file characteristic type
     */
    static QBluetoothUuid getFileChacteristicType();

    /**
     * @brief getWifiChannelChacteristicType
     * @return The Wifi channel characteristic type
     */
    static QBluetoothUuid getWifiChannelCharacteristicType();

    /**
     * @brief getVeniceFileServiceClassUuid
     * @return The venice file service class uuid
     */
    static QBluetoothUuid getVeniceFileServiceClassUuid();

};

#endif // VENICEBLUETOOTHUUID_H
