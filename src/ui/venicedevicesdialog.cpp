#include "include/ui/venicedevicesdialog.h"
#include "ui_venicedevicesdialog.h"
#include "include/ui/venicedeviceslistmodel.h"
#include "include/ui/mainwindow.h"


VeniceDevicesDialog::VeniceDevicesDialog(QWidget *parent): QDialog(parent),
    ui(new Ui::VeniceDevicesDialog){

    qDebug() << "[VeniceDevicesDialog] Init Ui "<< this->ui;
    this->ui->setupUi(this);
    qDebug() << "[VeniceDevicesDialog] Ui after Init "<< this->ui;
    //this->setModal(true);

    this->ui->devicesListView->setModel(new VeniceDevicesListModel(this));

    qDebug() << "[VeniceDevicesDialog] Associating slots with signals";
    connect(this->ui->cancelButtonBox, &QDialogButtonBox::accepted, this, &VeniceDevicesDialog::useSelectedVeniceDevice);
    connect(this->ui->cancelButtonBox, &QDialogButtonBox::rejected, this, &VeniceDevicesDialog::reject);
}

VeniceDevicesDialog::~VeniceDevicesDialog(){

    qDebug() << "[VeniceDevicesDialog] Deleting Ui";
    delete this->ui;
}

void VeniceDevicesDialog::useSelectedVeniceDevice(){

    QModelIndexList selectedIndexes = this->ui->devicesListView->selectionModel()->selectedIndexes();

    if(selectedIndexes.size()>0)
    {
        QModelIndex selectedIndex = selectedIndexes.first();

        MainWindow *mainWindow = dynamic_cast<MainWindow*>(this->parent());

        mainWindow->useSelectedDeviceForRetrieval(this->veniceDevicesListModel()->deviceAt(selectedIndex.row()));

    }


}

VeniceDevicesListModel* VeniceDevicesDialog::veniceDevicesListModel(){
    return dynamic_cast<VeniceDevicesListModel*>(this->ui->devicesListView->model());
}


