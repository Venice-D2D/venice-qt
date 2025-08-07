#include "include/tool/networkinformationmanager.h"
#include "include/exception/notsuitablewifiadapterfoundveniceexception.h"

NetworkInformationManager::NetworkInformationManager() {}


QHostAddress NetworkInformationManager::getIpAddress(QNetworkInterface::InterfaceType networkInterfaceType)
{
    QHostAddress ip;

    QList<QNetworkInterface> currentInterfaces = QNetworkInterface::allInterfaces();


    for(QNetworkInterface currentInterface: currentInterfaces)
    {
        if(currentInterface.type()== networkInterfaceType)
        {
            QList<QNetworkAddressEntry> addresses = currentInterface.addressEntries();

            if(!addresses.isEmpty())
                // Here we assume that there is only one IP
                ip = addresses.first().ip();
            else
            {
                qDebug() << "There is not IP address related to the type ";
                qDebug() << NetworkInformationManager::networkInterfaceTypeToString(networkInterfaceType);
            }

            break;
        }
    }

    return ip;
}

QString NetworkInformationManager::getNetworkInterfaceNameByType(QNetworkInterface::InterfaceType networkInterfaceType)
{
    QString interfaceName;

    QList<QNetworkInterface> currentInterfaces = QNetworkInterface::allInterfaces();


    for(QNetworkInterface currentInterface: currentInterfaces)
    {
        if(currentInterface.type()== networkInterfaceType)
        {
            interfaceName = currentInterface.name();

            qDebug() << "Network interface Name " << interfaceName;

            break;
        }
    }

    return interfaceName;
}





const char* NetworkInformationManager::networkInterfaceTypeToString(QNetworkInterface::InterfaceType networkInterfaceType)
{

    switch (networkInterfaceType) {

    case QNetworkInterface::Wifi:
        return WIFI;
    case QNetworkInterface::Ethernet:
        return ETHERNET;
    case QNetworkInterface::Ppp:
        return PPP;
    case QNetworkInterface::Fddi:
        return FDDI;
    case QNetworkInterface::Slip:
        return SLIP;
    case QNetworkInterface::Loopback:
        return LOOPBACK;
    case QNetworkInterface::Virtual:
        return VIRTUAL;
    case QNetworkInterface::CanBus:
        return CANBUS;
    case QNetworkInterface::Phonet:
        return PHONET;
    case QNetworkInterface::SixLoWPAN:
        return SIXLOWPAN;
    case QNetworkInterface::Ieee802154:
        return IEEE802154;
    case QNetworkInterface::Ieee80216:
        return IEEE80216;
    case QNetworkInterface::Ieee1394:
        return IEEE1394;
    default:
        return UNKNOWN;
    }

}

QNetworkInterface NetworkInformationManager::searchActiveNetworkInterfaceByType(QNetworkInterface::InterfaceType networkInterfaceType)
{
    QNetworkInterface networkInterface;

    QList<QNetworkInterface> currentInterfaces = QNetworkInterface::allInterfaces();


    for(QNetworkInterface currentInterface: currentInterfaces)
    {
        if(currentInterface.type()== networkInterfaceType && currentInterface.flags().testFlag(QNetworkInterface::IsUp) && currentInterface.flags().testFlag(QNetworkInterface::IsRunning) && !currentInterface.flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            networkInterface = currentInterface;

            qDebug() << "Selected Network interface Name " << networkInterface.name();
            qDebug() << "Adresses " << networkInterface.allAddresses().size();
            qDebug() << "Adresses Entries " << networkInterface.addressEntries().size();

            return networkInterface;
        }
    }

    throw NotSuitableWifiAdapterFoundVeniceException();
}
