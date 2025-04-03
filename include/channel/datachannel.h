#ifndef DATACHANNEL_H
#define DATACHANNEL_H

#include "channel.h"

#include <QtNetwork/QNetworkInterface>

/**
 * @brief The DataChannel class allows to build data channels by providing the
 * basic methods that are required
 */
class DataChannel: public Channel
{

public:
    /**
     * @brief DataChannel constructor which configures the channel
     * @param networkInterfaceType The network interface type
     */
    DataChannel(QNetworkInterface::InterfaceType networkInterfaceType);

    /**
     * @brief ~DataChannel destructor of the class
     */
    virtual ~DataChannel();


    /**
     * @brief configureChannel Configures the channel in order to be used
     */
    virtual void configure() throw()= 0;

    /**
     * @brief restoreChannelConfiguration Restores the channel original configuration
     */
    virtual void restoreChannelConfiguration() = 0;


protected:
    // The network interface type
    QNetworkInterface::InterfaceType networkInterfaceType;

    // The network interface name
    QString networkInterfaceName;


};

#endif // DATACHANNEL_H
