#ifndef NETWORKINFORMATIONMANAGER_H
#define NETWORKINFORMATIONMANAGER_H

#include <QNetworkInterface>


//Constants

static const char* WIFI = "Wifi";

static const char* ETHERNET = "Ethernet";

static const char* PPP = "Ppp";

static const char* FDDI = "Fddi";

static const char* SLIP = "Slip";

static const char* LOOPBACK = "Loopback";

static const char* VIRTUAL = "Virtual";

static const char* CANBUS = "CanBus";

static const char* PHONET = "Phonet";

static const char* SIXLOWPAN = "SixLoWPAN";

static const char* IEEE802154 = "Ieee802154";

static const char* IEEE80216 = "Ieee80216";

static const char* IEEE1394 = "Ieee1394";

static const char* UNKNOWN = "Unknown";

class NetworkInformationManager
{
    public:
        NetworkInformationManager();

        /**
         * @brief getIPWifiAddress looks for a IP address of the given type
         * @param networkInterfaceType The type of the network (Wifi, Ethernet)
         * @return The IP Address or null if the type is not found
         */
        static QHostAddress getIpAddress(QNetworkInterface::InterfaceType networkInterfaceType);

        /**
         * @brief networkInterfaceTypeToString Transform the network interface type into a string
         * @return The string related to the network type
         */
        static const char* networkInterfaceTypeToString(QNetworkInterface::InterfaceType networkInterfaceType);

        /**
         * @brief getNetworkInterfaceNameByType returns the first found adapter's name for a given type
         * @param networkInterfaceType The network interface type
         * @return The network adapter's name
         */
        static QString getNetworkInterfaceNameByType(QNetworkInterface::InterfaceType networkInterfaceType);

        /**
         * @brief searchActiveNetworkInterfaceByType Searches a network interface by the given type
         * @param networkInterfaceType QNetworkInterface::InterfaceType::Ethernet or QNetworkInterface::InterfaceType::Wifi
         * @return The first network found of the given type or null if an interface is not found
         * @throw NotSuitableWifiAdapterFoundVeniceException if an adapter of the given type is not found
         */
        static QNetworkInterface searchActiveNetworkInterfaceByType(QNetworkInterface::InterfaceType networkInterfaceType);
};

#endif // NETWORKINFORMATIONMANAGER_H
