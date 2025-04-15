#include "include/service/filetransferserver.h"
#include "include/exception/errorstartingfiletransferserviceveniceexception.h"
#include "external/protobuf/cpp_proto/venice.pb.h"

FileTransferServer::FileTransferServer(QObject *parent, QVector<VeniceMessage*> fileMessages, QHostAddress ipAddress, quint16 port, FileTransferService *fileTransferService): QTcpServer(parent)
{
    this->ipAddress = ipAddress;
    this->port = port;
    this->fileTransferService = fileTransferService;

    this->listenForConnections();

    qDebug() << "Listening on "<< this->serverAddress() << this->serverPort() << this->serverError();

    this->fileMessages = fileMessages;

    this->clientSocket = new QTcpSocket(this);

    this->resubmissionTimers = {};

    this->useProtobuf = true;

}

FileTransferServer::~FileTransferServer()
{
    delete this->clientSocket;
    this->close();
}

void FileTransferServer::incomingConnection(qintptr socketDescriptor){
    qDebug() << "We got a connection...";

    // Set the socket descriptor
    if (this->clientSocket->setSocketDescriptor(socketDescriptor)) {
        qDebug() << "Usage of port "<< QString::number(this->clientSocket->localPort());
        qDebug() << "Usage of adress "<< this->clientSocket->localAddress();

        connect(this->clientSocket, &QTcpSocket::errorOccurred,
                    this, &FileTransferServer::onError);

        if(connect(this->clientSocket, &QTcpSocket::readyRead, this, &FileTransferServer::onDataReadyToBeRead))
            qDebug() << "onDataReadyToBeRead connected!!";

        connect(this->clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);

        qDebug() << "New connection established!";

        //qDebug() << "Re-start the file transfer service";
        //qDebug() << "Quit";
        //this->fileTransferService->quit();
        //qDebug() << "Wait";
        //this->fileTransferService->wait();
        //qDebug() << "Start";
        //this->fileTransferService->start();

        qDebug() << "sending the messages";
        this->sendVeniceMessages();
        this->clientSocket->disconnectFromHost();

    } else {
        qDebug() << "Client connection not accepted...";
        delete clientSocket; // Cleanup in case of an error
    }
}

void FileTransferServer::onBytesWritten(__attribute__((unused))qint64 bytes) {
    this->sendNextVeniceMessage();
}

void FileTransferServer::onError(QAbstractSocket::SocketError socketError) {
    qCritical() << "Socket error:" << socketError;
}

void FileTransferServer::onDataReadyToBeRead(){

    if (this->clientSocket->state() == QTcpSocket::ConnectedState) {
        qDebug() << "Socket is connected.";
    } else {
        qDebug() << "Socket is not connected. Current state:" << this->clientSocket->state();
        qDebug() << this->clientSocket->errorString();
    }

    qDebug() << "New data received...";

    QByteArray data = this->clientSocket->readAll();  // Read the incoming data
    qDebug() << "Received:" << data;

    VeniceMessage message = VeniceMessage::fromJson(data);
    if(message.isAck())
    {
        QWriteLocker locker(&this->lock);
        QMap<int, VeniceTimer*>::const_iterator timerIterator = this->resubmissionTimers.find(message.getMessageId());

        if(timerIterator!= this->resubmissionTimers.end())
        {
            VeniceTimer* currentTimer = timerIterator.value();
            currentTimer->stop();
            this->resubmissionTimers.remove(message.getMessageId());
            delete currentTimer->getMessage();
            delete currentTimer;
        }
    }

}

void FileTransferServer::sendVeniceMessages(){

    QTimer* waitingForAMessageTimer = new QTimer();

    while(!this->fileMessages.isEmpty() || !this->resubmissionTimers.isEmpty())
    {
        if(this->fileMessages.isEmpty())
        {
            waitingForAMessageTimer->start(200);
        }
        else
        {
            VeniceMessage* currentMessage = this->fileMessages.first();
            this->fileMessages.remove(0);
            VeniceTimer* messageTimer = this->sendVeniceMessage(currentMessage);

            qDebug() << "Starting venice timer with id ..."+ QString::number(currentMessage->getMessageId());
            messageTimer->start(1000);

            qDebug() << "Message sent via sendVeniceMessage... ";
            this->clientSocket->waitForReadyRead();
            while (resubmissionTimers.find(currentMessage->getMessageId())!= resubmissionTimers.end()) {
                qDebug() << "Waiting for timer removal... ";
                waitingForAMessageTimer->start(200);
            }


        }

    }

    delete waitingForAMessageTimer;
    this->clientSocket->disconnectFromHost();

}

void FileTransferServer::sendNextVeniceMessage(){
    QWriteLocker locker(&this->lock);
    if(!this->fileMessages.isEmpty() || !this->resubmissionTimers.isEmpty()){

        QTimer* waitingForAMessageTimer = new QTimer();

        if(this->fileMessages.isEmpty())
        {
            waitingForAMessageTimer->start(200);
        }
        else
        {
            VeniceMessage* currentMessage = this->fileMessages.first();
            this->fileMessages.remove(0);

            qDebug() << "Sending message with id... " + QString::number(currentMessage->getMessageId());
            this->clientSocket->write(currentMessage->toJson());
            qDebug() << "Message sent... ";
            this->clientSocket->flush();

            qDebug() << "Creating Venice timer... ";
            VeniceTimer* messageTimer = new VeniceTimer(currentMessage);
            messageTimer->setSingleShot(true);
            int messageId = currentMessage->getMessageId();
            this->resubmissionTimers.insert(messageId,messageTimer);

            // We add the message again to the list of messages if the acknowledgement does not
            // arrive

            QObject::connect(messageTimer, &QTimer::timeout, [this, currentMessage]() {

                if(this->resubmissionTimers.find(currentMessage->getMessageId())!= this->resubmissionTimers.end()){
                    QWriteLocker locker(&this->lock);
                    this->fileMessages.insert(0, currentMessage);

                    QMap<int, VeniceTimer*>::const_iterator timerIterator = this->resubmissionTimers.find(currentMessage->getMessageId());
                    VeniceTimer* currentTimer = timerIterator.value();
                    this->resubmissionTimers.remove(currentMessage->getMessageId());

                    delete currentTimer;
                    qDebug() << "Ended code related to venice timer with id..."+ QString::number(currentMessage->getMessageId());
                }
                qDebug() << "Ended Venice timer with id ..."+ QString::number(currentMessage->getMessageId());

            });
            qDebug() << "Starting venice timer with id ..."+ QString::number(currentMessage->getMessageId());
            messageTimer->start(1000);
         }
    }
    else{
        this->clientSocket->disconnectFromHost();
    }


}


VeniceTimer* FileTransferServer::sendVeniceMessage(VeniceMessage* message){
    //QWriteLocker locker(&this->lock);

    qDebug() << "Sending message with id... " + QString::number(message->getMessageId());
    QByteArray serializedMessage;

    if(this->useProtobuf)
    {
        qDebug() << "Protobuf serialization... ";
        serializedMessage = message->toProtoBuf();
    }
    else
    {
        qDebug() << "JSON serialization... ";
        serializedMessage = message->toJson();
    }

    qDebug() << "Writing the message... ";

    this->clientSocket->write(serializedMessage);
    this->clientSocket->flush();
    this->clientSocket->waitForBytesWritten();

    qDebug() << "Message sent... ";

    qDebug() << "Creating Venice timer... ";
    VeniceTimer* messageTimer = new VeniceTimer(message);
    messageTimer->setSingleShot(true);
    int messageId = message->getMessageId();
    this->resubmissionTimers.insert(messageId,messageTimer);

    // We add the message again to the list of messages if the acknowledgement does not
    // arrive

    if(!QObject::connect(messageTimer, &QTimer::timeout, []() {

        qDebug() << "Starting venice timer code execution";


    }))
    {
        qDebug() << "Failed to connect signal and slot.";
    }
    else
    {
        qDebug() << "signal and slot connected!";
    }

    return messageTimer;

}

void FileTransferServer::listenForConnections()
{
    if(!this->listen(this->ipAddress, this->port))
    {
        qDebug() << "Issues configuring FileTransferService";
        throw ErrorStartingFileTransferServiceVeniceException();

    }
}


