#include "include/ui/venicedeviceslistmodel.h"

#include <QBluetoothAddress>

VeniceDevicesListModel::VeniceDevicesListModel(QObject *parent)
    : QAbstractListModel(parent){

}


int VeniceDevicesListModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : this->devices.size();
}

QVariant VeniceDevicesListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole)
        return {};
    QBluetoothDeviceInfo *currentDevice = this->devices.at(index.row());
    return currentDevice->name()+": "+currentDevice->address().toString();
}

void VeniceDevicesListModel::addItem(QBluetoothDeviceInfo *item) {
    this->beginInsertRows(QModelIndex(), this->devices.size(), this->devices.size());
    this->devices.append(item);
    this->endInsertRows();
}

QBluetoothDeviceInfo *VeniceDevicesListModel::deviceAt(int position){
    return this->devices.at(position);
}
