#include "include/service/venicebluetoothuuid.h"

VeniceBluetoothUuid::VeniceBluetoothUuid()
{

}

QBluetoothUuid VeniceBluetoothUuid::getFileChacteristicType()
{
    // Here we use of CharacteristicType that is a string
    return QBluetoothUuid::CharacteristicType::ManufacturerNameString;
}

QBluetoothUuid VeniceBluetoothUuid::getWifiChannelCharacteristicType()
{
    // Here we use of CharacteristicType that is a string
    return QBluetoothUuid::CharacteristicType::LastName;
}

QBluetoothUuid VeniceBluetoothUuid::getVeniceFileServiceClassUuid()
{
    // Here we use of CharacteristicType that is an existing service
    return QBluetoothUuid::ServiceClassUuid::HeartRate;
}
