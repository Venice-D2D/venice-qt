#ifndef WIFIDATACHANNEL_H
#define WIFIDATACHANNEL_H

#include <string>

#include "datachannel.h"



class WifiDataChannel: DataChannel
{
public:

    const int STARTING_PORT = 20000;
    const int ENDING_PORT = 30000;
    /**
     * /@brief WiDataChannel constructor
     * @param ip The ip address related to the channel
     * @param gateway The gateway related to the channel
     * @param netmask The netmask related to the channel
     */
    //WifiDataChannel(const char* ip,const char* gateway,const char* netmask);

    /**
     * @brief WifiDataChannel default constructor
     */
    WifiDataChannel();

    /**
      * @brief Start the network manager
      */
    ~WifiDataChannel();


    /**
     * @brief configureChannel Configures the ip and port for exchanging files
     * @throw NotSuitableWifiAdapterFoundVeniceException If a wifi adapter with an IP and avaible port are not found
     */
    void configureChannel();

    /**
     * @brief restoreChannelConfiguration Restores the channel original configuration
     */
    void restoreChannelConfiguration();

    /**
     * @brief getAdhocNetworkName Returns the name of the Adhoc-Network
     * @return  The name of the Adhoc-Network
     */
    //const char* getAdhocNetworkName();

    /**
     * @brief getPort returns the used port for file exchanging
     * @return The used port
     */
    int getPort();

    /**
     * @brief getWifiInterface returns the wifi interface related with the channel
     * @return The wifi interface
     */
    QNetworkInterface getWifiInterface();

    /**
     * @brief getNetworkAddress Returns the first network address related to the wifi adapter
     * @return The first network address
     */
    QNetworkAddressEntry getNetworkAddress();

    /**
     * @brief getSSID Returns the wifi network name related to the channel
     * @return The SSID related to the channel
     */
    QString getSSID();

private:
    // The ip related to the adapter
    //const char* ip;

    // The gateway related to the adapter
    //const char* gateway;

    // The netmask related to the adapter
    //const char* netmask;

    // Ad-hoc network name
    //const char* adhocNetworkName;

    // The wifi interface related to this channel
    QNetworkInterface wifiInterface;

    // The port used for the data exchange
    int port;

    // The name of the wifi name for this channel
    QString ssid;

    /**
      * @brief executeSystemCommand Executes the provided command
      * @param command: The command to be executed
      **/
    void executeSystemCommand(const char* command);


    /**
     * @brief searchForAvailablePort Look for an available port in the current wifi interface
     * @throw NotAvailablePortFoundVeniceException if an available port is not found by using the configured ports limits
     */
    void searchForAvailablePort() throw();

    /**
     * @brief searchForSSID Uses the nmcli command in order to get the wifi network name by using the channel ip
     * @throw NotWifiAdapterFoundVeniceException if there is not a wifi network adapter
     */
    void searchForSSID() throw();


};

#endif // WIFIDATACHANNEL_H
