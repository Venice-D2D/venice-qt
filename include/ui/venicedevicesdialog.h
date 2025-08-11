#ifndef VENICEDEVICESDIALOG_H
#define VENICEDEVICESDIALOG_H

#include "include/ui/venicedeviceslistmodel.h"

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class VeniceDevicesDialog; }
QT_END_NAMESPACE

class VeniceDevicesDialog : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief VeniceDevicesDialog Constructor of the class
     * @param parent Parent of the dialog
     */
    VeniceDevicesDialog(QWidget *parent);

    ~VeniceDevicesDialog();

    VeniceDevicesListModel* getVeniceDevicesListModel();

private slots:

    void useSelectedVeniceDevice();

private:
    Ui::VeniceDevicesDialog *ui;
};


#endif // VENICEDEVICESDIALOG_H
