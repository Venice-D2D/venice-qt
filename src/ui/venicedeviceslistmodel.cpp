#include "include/ui/venicedeviceslistmodel.h"

#include <QBluetoothAddress>

VeniceDevicesListModel::VeniceDevicesListModel(QObject *parent)
    : QAbstractListModel(parent){

}


int VeniceDevicesListModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : this->devices.size();
}

QVariant VeniceDevicesListModel::data(const QModelIndex &index, int role) const {
    qDebug() << "[VeniceDevicesListModel] Quering data index " << index <<" role "<<role;
    if (!index.isValid() || role != Qt::DisplayRole)
        return {};
    qDebug() << "[VeniceDevicesListModel] Index row   " << index.row();
    QBluetoothDeviceInfo currentDevice = this->devices.at(index.row());
    qDebug() << "[VeniceDevicesListModel] Data returned " << currentDevice.name();
    return currentDevice.name()+": "+currentDevice.address().toString();
}

void VeniceDevicesListModel::addDevice(const QBluetoothDeviceInfo &device) {
    qDebug() << "[VeniceDevicesListModel] Adding device "<< "-" << device.name() <<" - " << device.address().toString();
    this->beginInsertRows(QModelIndex(), this->devices.size(), this->devices.size());
    this->devices.append(device);
    this->endInsertRows();
    qDebug() << "[VeniceDevicesListModel] Device added ";
}

QBluetoothDeviceInfo VeniceDevicesListModel::deviceAt(int position){
    return this->devices.at(position);
}

void VeniceDevicesListModel::clear(){

    if (!devices.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, devices.size() - 1);
        devices.clear();
        endRemoveRows();
    }
}

bool VeniceDevicesListModel::isDeviceInList(const QBluetoothDeviceInfo &device){

    return devices.contains(device);

}
