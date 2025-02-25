#ifndef NOTBLUETOOTHADAPTERFOUNDVENICEEXCEPTION_H
#define NOTBLUETOOTHADAPTERFOUNDVENICEEXCEPTION_H

#include "veniceexception.h"

/**
 * @brief The NotBluetoothAdapterFoundVeniceException class indicates that there
 * is not a suitable bluetooth adapter for service advertisement
 */
class NotBluetoothAdapterFoundVeniceException : public VeniceException
{
public:
    NotBluetoothAdapterFoundVeniceException();
};

#endif // NOTBLUETOOTHADAPTERFOUNDVENICEEXCEPTION_H
