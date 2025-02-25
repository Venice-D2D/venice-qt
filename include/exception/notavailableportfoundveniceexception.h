#ifndef NOTAVAILABLEPORTFOUNDVENICEEXCEPTION_H
#define NOTAVAILABLEPORTFOUNDVENICEEXCEPTION_H

#include "veniceexception.h"

/**
 * @brief The NotAvailablePortFoundVeniceException class indicates that there is
 * not at least an available port for exchange the data
 */
class NotAvailablePortFoundVeniceException : public VeniceException
{
public:
    NotAvailablePortFoundVeniceException();
};

#endif // NOTAVAILABLEPORTFOUNDVENICEEXCEPTION_H
