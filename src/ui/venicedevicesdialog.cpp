#include "include/ui/venicedevicesdialog.h"
#include "ui_venicedevicesdialog.h"
#include "include/ui/venicedeviceslistmodel.h"
#include "include/ui/mainwindow.h"


VeniceDevicesDialog::VeniceDevicesDialog(QWidget *parent): QDialog(parent),
    ui(new Ui::VeniceDevicesDialog){

    this->ui->setupUi(this);
    this->setModal(true);

    this->ui->devicesListView->setModel(new VeniceDevicesListModel(this));

    connect(this->ui->cancelButtonBox, &QDialogButtonBox::accepted, this, &VeniceDevicesDialog::useSelectedVeniceDevice);
    connect(this->ui->cancelButtonBox, &QDialogButtonBox::rejected, this, &VeniceDevicesDialog::reject);
}

VeniceDevicesDialog::~VeniceDevicesDialog(){

    delete this->ui;
}

void VeniceDevicesDialog::useSelectedVeniceDevice(){

    QModelIndexList selectedIndexes = this->ui->devicesListView->selectionModel()->selectedIndexes();

    if(selectedIndexes.size()>0)
    {
        QModelIndex selectedIndex = selectedIndexes.first();

        MainWindow *mainWindow = dynamic_cast<MainWindow*>(this->parent());

        mainWindow->useSelectedDeviceForRetrieval(this->getVeniceDevicesListModel()->deviceAt(selectedIndex.row()));

    }


}

VeniceDevicesListModel* VeniceDevicesDialog::getVeniceDevicesListModel(){
    return dynamic_cast<VeniceDevicesListModel*>(this->ui->devicesListView->model());
}


