#ifndef VENICEDEVICESLISTMODEL_H
#define VENICEDEVICESLISTMODEL_H

#include <QAbstractListModel>
#include <QBluetoothDeviceInfo>

/**
 * @brief The VeniceDevicesListModel class for dealing with discovered venice devices
 */
class VeniceDevicesListModel : public QAbstractListModel
{
public:
    /**
     * @brief VeniceDevicesListModel Constructor of the class
     * @param parent parent of the model
     */
    VeniceDevicesListModel(QObject *parent = nullptr);


    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /**
     * @brief addItem Add the device to the list of found venice devices
     * @param device The BLE venice device to add
     */
    void addDevice(const QBluetoothDeviceInfo &device);

    /**
     * @brief getDeviceAt Returns the venice device at the given position
     * @param position The position to get the device
     * @return The device in the given position
     */
    QBluetoothDeviceInfo deviceAt(int position);


    /**
     * @brief clear Remove all the devices in the
     */
    void clear();

    /**
     * @brief isDeviceInList Indicate if the given device is in the venice devices list
     * @param device The device to look for in the devices list
     * @return True if device in devices, false otherwise
     */
    bool isDeviceInList(const QBluetoothDeviceInfo &device);


private:
    /**
     * @brief devices List of venice devices
     */
    QList<QBluetoothDeviceInfo> devices;

};

#endif // VENICEDEVICESLISTMODEL_H
