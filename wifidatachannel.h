#ifndef WIFIDATACHANNEL_H
#define WIFIDATACHANNEL_H

#include "datachannel.h"



class WifiDataChannel: DataChannel
{
public:
    /**
     * /@brief WiDataChannel constructor
     * @param ip The ip address related to the channel
     * @param gateway The gateway related to the channel
     * @param netmask The netmask related to the channel
     */
    WifiDataChannel(const char* ip,const char* gateway,const char* netmask);

    /**
      * @brief Start the network manager
      */
    ~WifiDataChannel();


    /**
     * @brief configureChannel Configures the ip, gateway and netmask of the wifi adapter channel
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
    const char* getAdhocNetworkName();

private:
    // The ip related to the adapter
    const char* ip;

    // The gateway related to the adapter
    const char* gateway;

    // The netmask related to the adapter
    const char* netmask;

    // Ad-hoc network name
    const char* adhocNetworkName;

    /**
      * @brief executeSystemCommand Executes the provided command
      * @param command: The command to be executed
      **/
    void executeSystemCommand(const char* command);
};

#endif // WIFIDATACHANNEL_H
