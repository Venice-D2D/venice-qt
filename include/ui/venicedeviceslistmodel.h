#ifndef VENICEDEVICESLISTMODEL_H
#define VENICEDEVICESLISTMODEL_H

#include <QAbstractListModel>
#include <QBluetoothDeviceInfo>

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
     * @brief addItem Add the item to the list of found venice devices
     * @param item The BLE venice device to add
     */
    void addItem(QBluetoothDeviceInfo *item);

    /**
     * @brief getDeviceAt Returns the venice device at the given position
     * @param position The position to get the device
     * @return The device in the given position
     */
    QBluetoothDeviceInfo *deviceAt(int position);


private:
    /**
     * @brief devices List of venice devices
     */
    QList<QBluetoothDeviceInfo*> devices;

};

#endif // VENICEDEVICESLISTMODEL_H
