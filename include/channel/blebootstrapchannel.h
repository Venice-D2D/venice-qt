#ifndef BLEBOOTSTRAPCHANNEL_H
#define BLEBOOTSTRAPCHANNEL_H

#include "channel.h"
#include "include/exception/notbluetoothadapterfoundveniceexception.h"
#include "include/channel/bootstrapchannel.h"


class BleBootstrapChannel: public BootstrapChannel
{
public:
    /**
     * @brief BleBootstrapChannel
     */
    BleBootstrapChannel();

    /**
     * @brief configure
     * @throw NotBluetoothAdapterFoundVeniceException if a valid bluetooth adapter is found
     */
    void configure();
};

#endif // BLEBOOTSTRAPCHANNEL_H
