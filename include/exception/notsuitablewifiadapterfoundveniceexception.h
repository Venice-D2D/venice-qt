#ifndef NOTSUITABLEWIFIADAPTERFOUNDVENICEEXCEPTION_H
#define NOTSUITABLEWIFIADAPTERFOUNDVENICEEXCEPTION_H

#include "veniceexception.h"

/**
 * @brief The NotSuitableWifiAdapterFoundVeniceException class indicates that
 * there is not a suitable wifi adapter for exchanging data
 */
class NotSuitableWifiAdapterFoundVeniceException: public VeniceException
{
public:
    NotSuitableWifiAdapterFoundVeniceException();
};

#endif // NOTSUITABLEWIFIADAPTERFOUNDVENICEEXCEPTION_H
