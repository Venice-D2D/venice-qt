#include "filetransferservice.h"
#include "errorstartingfiletransferserviceveniceexception.h"


FileTransferService::FileTransferService(QObject *parent, const char* filePath, QHostAddress ipAddress, quint16 port) throw(): QTcpServer(parent)
{
    this->filePath = filePath;

    if(!this->listen(ipAddress, port))
    {
        qDebug() << "Issues starting FileTransferService";
        throw ErrorStartingFileTransferServiceVeniceException();

    }
    qDebug() << "Listening on "<< this->serverAddress() << this->serverPort() << this->serverError();
}

FileTransferService::~FileTransferService()
{
    this->close();
}

