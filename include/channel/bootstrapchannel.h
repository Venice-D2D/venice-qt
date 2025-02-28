#ifndef BOOTSTRAPCHANNEL_H
#define BOOTSTRAPCHANNEL_H

#include "datachannel.h"

class BootstrapChannel : public Channel
{
public:
    /**
     * @brief BootstrapChannel Constructeur of the bootstrap channel
     */
    BootstrapChannel();

    /**
     * @brief ~DataChannel destructor of the class
     */
    virtual ~BootstrapChannel();

};

#endif // BOOTSTRAPCHANNEL_H
