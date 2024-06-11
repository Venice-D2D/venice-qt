#include "filetransferservice.h"


FileTransferService::FileTransferService(QObject *parent, string filePath, QHostAddress ipAddress, quint16 port): QTcpServer(parent)
{
    this->filePath = filePath;

    this-> listen(ipAddress, port);

    qDebug() << this->serverAddress() << this->serverPort() << this->serverError();
}

FileTransferService::~FileTransferService()
{
    this->close();
}

