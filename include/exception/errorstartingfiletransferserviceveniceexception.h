#ifndef ERRORSTARTINGFILETRANSFERSERVICEVENICEEXCEPTION_H
#define ERRORSTARTINGFILETRANSFERSERVICEVENICEEXCEPTION_H

#include "veniceexception.h"

/**
 * @brief The ErrorStartingFileTransferServiceVeniceException class represents
 * an error raised with there are issues when starting the File Transfer Service
 */
class ErrorStartingFileTransferServiceVeniceException: public VeniceException
{
public:
    ErrorStartingFileTransferServiceVeniceException();
};

#endif // ERRORSTARTINGFILETRANSFERSERVICEVENICEEXCEPTION_H
