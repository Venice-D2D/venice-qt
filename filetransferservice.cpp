#include "filetransferservice.h"
#include "errorstartingfiletransferserviceveniceexception.h"
#include "external/protobuf/cpp_proto/venice.pb.h"

#include <QTcpSocket>


FileTransferService::FileTransferService(QObject *parent, const char* filePath, QVector<VeniceMessage*> fileMessages, QHostAddress ipAddress, quint16 port) throw(): QTcpServer(parent)
{
    //this->file_related_messages = file_related_messages;

    //QHostAddress address("127.0.0.1");
    if(!this->listen(ipAddress, port))
    {
        qDebug() << "Issues starting FileTransferService";
        throw ErrorStartingFileTransferServiceVeniceException();

    }
    qDebug() << "Listening on "<< this->serverAddress() << this->serverPort() << this->serverError();

    this->fileToSend.setFileName(filePath);

    this->fileMessages = fileMessages;

    this->clientSocket = new QTcpSocket(this);

    this->resubmissionTimers = {};

    this->useProtobuf = true;

}

FileTransferService::~FileTransferService()
{
    delete this->clientSocket;
    this->close();
}


void FileTransferService::incomingConnection(qintptr socketDescriptor){
    qDebug() << "We got a connection...";

    /*if (!this->fileToSend.open(QIODevice::ReadOnly)) {
        qCritical() << "Failed to open file:" << this->fileToSend.errorString();
        return;
    }*/

    // Set the socket descriptor
    if (this->clientSocket->setSocketDescriptor(socketDescriptor)) {

        //connect(this->clientSocket, &QTcpSocket::bytesWritten, this, &FileTransferService::onBytesWritten);
        qDebug() << "Usage of port "<< QString::number(this->clientSocket->localPort());
        qDebug() << "Usage of adress "<< this->clientSocket->localAddress();

        connect(this->clientSocket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error),
                    this, &FileTransferService::onError);

        if(connect(this->clientSocket, &QTcpSocket::readyRead, this, &FileTransferService::onDataReadyToBeRead))
            qDebug() << "onDataReadyToBeRead connected!!";
        /*connect(clientSocket, &QTcpSocket::readyRead, this, [clientSocket]() {
            //QByteArray data = clientSocket->readAll();  // Read the incoming data
            //qDebug() << "Received:" << data;

            // Echo the data back to the client
            clientSocket->write(QByteArray('test'));
        });*/

        connect(this->clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);

        qDebug() << "New connection established!";

        // Send the first message
        qDebug() << "Sending messages: "+QString::number(this->fileToSend.size());
        //this->sendNextVeniceMessage();
        this->sendVeniceMessages();


        //QByteArray fileSizeHeader = QByteArray::number(this->fileToSend.size()) + "\n";
        //this->clientSocket->write(fileSizeHeader);
    } else {
        qDebug() << "Client connection not accepted...";
        delete clientSocket; // Cleanup in case of an error
    }
}

void FileTransferService::onBytesWritten(qint64 bytes) {
    this->sendNextVeniceMessage();
}

void FileTransferService::onError(QAbstractSocket::SocketError socketError) {
    qCritical() << "Socket error:" << socketError;
}

void FileTransferService::onDataReadyToBeRead(){

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

void FileTransferService::sendVeniceMessages(){

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
                //waitingForAMessageTimer->start(200);
                //this->onDataReadyToBeRead();
            }


        }

    }

    delete waitingForAMessageTimer;
    this->clientSocket->disconnectFromHost();

}

void FileTransferService::sendNextVeniceMessage(){
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


VeniceTimer* FileTransferService::sendVeniceMessage(VeniceMessage* message){
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

    /*if(!QObject::connect(messageTimer, &QTimer::timeout, [this, message]() {

        qDebug() << "Starting venice timer code execution with id ..."+ QString::number(message->getMessageId());
        if(this->resubmissionTimers.find(message->getMessageId())!= this->resubmissionTimers.end()){
            //QWriteLocker locker(&this->lock);
            this->fileMessages.insert(0, message);

            QMap<int, VeniceTimer*>::const_iterator timerIterator = this->resubmissionTimers.find(message->getMessageId());
            VeniceTimer* currentTimer = timerIterator.value();
            this->resubmissionTimers.remove(message->getMessageId());

            delete currentTimer;
            qDebug() << "Ended code related to venice timer with id..."+ QString::number(message->getMessageId());
        }
        qDebug() << "Ended Venice timer with id ..."+ QString::number(message->getMessageId());

    }))
    {
        qDebug() << "Failed to connect signal and slot.";
    }
    else
    {
        qDebug() << "signal and slot connected!";
    }*/

    return messageTimer;

}


