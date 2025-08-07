#include "include/sender/filesender.h"
#include "include/exception/errorstartingfiletransferserviceveniceexception.h"
#include "external/protobuf/cpp_proto/venice.pb.h"

//#include <QProtobufSerializer>

FileSender::FileSender(QObject *parent, QVector<VeniceMessage*> fileMessages, QHostAddress ipAddress, quint16 port, FileTransferServiceProvider *fileTransferServiceProvider, bool useProtobuf): QTcpServer(parent)
{
    this->ipAddress = ipAddress;
    this->port = port;
    this->fileTransferServiceProvider = fileTransferServiceProvider;
    this->useProtobuf = useProtobuf;

    this->listenForConnections();

    qDebug() << "[FileSender] Listening on "<< this->serverAddress() << this->serverPort() << this->serverError();

    this->fileMessages = fileMessages;

    this->clientSocket = new QTcpSocket(this);

    this->resubmissionTimers = {};

}

FileSender::~FileSender()
{
    delete this->clientSocket;
    this->close();
}

void FileSender::incomingConnection(qintptr socketDescriptor){
    qDebug() << "[FileSender] We got a connection...";

    // Set the socket descriptor
    if (this->clientSocket->setSocketDescriptor(socketDescriptor)) {
        qDebug() << "[FileSender] Usage of port "<< QString::number(this->clientSocket->localPort());
        qDebug() << "[FileSender] Usage of adress "<< this->clientSocket->localAddress();

        connect(this->clientSocket, &QTcpSocket::errorOccurred,
                    this, &FileSender::onError);

        if(connect(this->clientSocket, &QTcpSocket::readyRead, this, &FileSender::onDataReadyToBeRead))
            qDebug() << "[FileSender] onDataReadyToBeRead connected!!";

        connect(this->clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);

        qDebug() << "[FileSender] New connection established!";

        //qDebug() << "Re-start the file transfer service";
        //qDebug() << "Quit";
        //this->fileTransferService->quit();
        //qDebug() << "Wait";
        //this->fileTransferService->wait();
        //qDebug() << "Start";
        //this->fileTransferService->start();

        qDebug() << "[FileSender] sending the messages";
        this->sendVeniceMessages();
        this->clientSocket->disconnectFromHost();

    } else {
        qDebug() << "[FileSender] Client connection not accepted...";
        delete clientSocket; // Cleanup in case of an error
    }
}

void FileSender::onBytesWritten(__attribute__((unused))qint64 bytes) {
    this->sendNextVeniceMessage();
}

void FileSender::onError(QAbstractSocket::SocketError socketError) {
    qCritical() << "[FileSender] Socket error:" << socketError;
}

void FileSender::onDataReadyToBeRead(){

    if (this->clientSocket->state() == QTcpSocket::ConnectedState) {
        qDebug() << "[FileSender] Socket is connected.";
    } else {
        qDebug() << "[FileSender] Socket is not connected. Current state:" << this->clientSocket->state();
        qDebug() << this->clientSocket->errorString();
    }

    qDebug() << "[FileSender] New data received...";

    QByteArray data = this->clientSocket->readAll();  // Read the incoming data
    qDebug() << "[FileSender] Received:" << data;

    VeniceMessage message;

    if(this->useProtobuf)
        message = VeniceMessage::fromProtoBuf(data);
    else
        message = VeniceMessage::fromJson(data);
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

void FileSender::sendVeniceMessages(){

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

            qDebug() << "[FileSender] Starting venice timer with id ..."+ QString::number(currentMessage->getMessageId());
            messageTimer->start(1000);

            qDebug() << "[FileSender] Message sent via sendVeniceMessage... ";
            this->clientSocket->waitForReadyRead();
            while (this->resubmissionTimers.find(currentMessage->getMessageId())!= resubmissionTimers.end()) {
                qDebug() << "[FileSender] Waiting for timer removal... ";
                waitingForAMessageTimer->start(200);
            }


        }

    }

    delete waitingForAMessageTimer;
    this->clientSocket->disconnectFromHost();

}

void FileSender::sendNextVeniceMessage(){
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

            qDebug() << "[FileSender] Sending message with id... " + QString::number(currentMessage->getMessageId());
            if(this->useProtobuf)
                this->clientSocket->write(currentMessage->toProtoBuf());
            else
                this->clientSocket->write(currentMessage->toJson());
            qDebug() << "[FileSender] Message sent... ";
            this->clientSocket->flush();

            qDebug() << "[FileSender] Creating Venice timer... ";
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
                    qDebug() << "[FileSender] Ended code related to venice timer with id..."+ QString::number(currentMessage->getMessageId());
                }
                qDebug() << "[FileSender] Ended Venice timer with id ..."+ QString::number(currentMessage->getMessageId());

            });
            qDebug() << "[FileSender] Starting venice timer with id ..."+ QString::number(currentMessage->getMessageId());
            messageTimer->start(1000);
         }
    }
    else{
        this->clientSocket->disconnectFromHost();
    }


}


VeniceTimer* FileSender::sendVeniceMessage(VeniceMessage* message){
    //QWriteLocker locker(&this->lock);

    qDebug() << "[FileSender] Sending message with id... " + QString::number(message->getMessageId());
    QByteArray serializedMessage;

    if(this->useProtobuf)
    {
        qDebug() << "[FileSender] Protobuf serialization... ";
        serializedMessage = message->toProtoBuf();
    }
    else
    {
        qDebug() << "[FileSender] JSON serialization... ";
        serializedMessage = message->toJson();
    }

    qDebug() << "[FileSender] Writing the message... ";

    this->clientSocket->write(serializedMessage);
    this->clientSocket->flush();
    this->clientSocket->waitForBytesWritten();

    qDebug() << "[FileSender] Message sent... ";

    qDebug() << "[FileSender] Creating Venice timer... ";
    VeniceTimer* messageTimer = new VeniceTimer(message);
    messageTimer->setSingleShot(true);
    int messageId = message->getMessageId();
    this->resubmissionTimers.insert(messageId,messageTimer);

    // We add the message again to the list of messages if the acknowledgement does not
    // arrive

    if(!QObject::connect(messageTimer, &QTimer::timeout, []() {

        qDebug() << "[FileSender] Starting venice timer code execution";


    }))
    {
        qDebug() << "[FileSender] Failed to connect signal and slot.";
    }
    else
    {
        qDebug() << "[FileSender] signal and slot connected!";
    }

    return messageTimer;

}

void FileSender::listenForConnections()
{
    if(!this->listen(this->ipAddress, this->port))
    {
        qDebug() << "[FileSender] Issues configuring FileTransferService";
        throw ErrorStartingFileTransferServiceVeniceException();

    }
}


