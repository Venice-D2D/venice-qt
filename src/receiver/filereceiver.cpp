#include "include/receiver/filereceiver.h"
#include "include/network/venicemessage.h"

#include <QFile>

FileReceiver::FileReceiver(QObject *parent, ChannelMetadata *channelMetadata, FileMetadata *fileMetadata, bool useProtobuf):QTcpSocket(parent){
    this->channelMetadata = channelMetadata;
    this->fileMetadata = fileMetadata;

    this->useProtobuf = useProtobuf;


    connect(this, &QTcpSocket::connected, this, &FileReceiver::processConnectionToSender);
    connect(this, &QTcpSocket::readyRead, this, &FileReceiver::processMessageReceived);
    connect(this, &QTcpSocket::disconnected, this, &FileReceiver::dealWithDisconnection);
    connect(this, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
            this, &FileReceiver::dealWithError);
}


FileReceiver::~FileReceiver(){
}

void FileReceiver::startFileReception(){

    QHostAddress address(this->channelMetadata->getAddress());

    qDebug() << "[FileReceiver] Connecting to host... " << this->channelMetadata->getAddress() <<" " << this->channelMetadata->getPort();
    this->connectToHost(address, this->channelMetadata->getPort());

}

void FileReceiver::processConnectionToSender()
{
    qDebug() << "[FileReceiver] Connected to server.";
    // Optionally send something: socket->write("Hello Server");
}

void FileReceiver::processMessageReceived()
{
    //QByteArray data = this->readAll();

    if(this->currentDatalength==-1)
    {
        if (this->bytesAvailable() < 4)
            return;

        QByteArray lenghtBytes = this->read(4);
        this->currentDatalength= (unsigned char)lenghtBytes[0] << 24 |
                     (unsigned char)lenghtBytes[1] << 16 |
                     (unsigned char)lenghtBytes[2] << 8 |
                     (unsigned char)lenghtBytes[3];
    }



    qDebug() << "[FileReceiver] data length:" << this->currentDatalength;

    if(this->bytesAvailable() < this->currentDatalength)
    {
        qDebug() << "[FileReceiver] waiting to get at least" << this->currentDatalength << " bytes";
        return;

    }


    QByteArray data = this->read(this->currentDatalength);

    this->currentDatalength = -1;


    qDebug() << "[FileReceiver] Received data:" << data;

    VeniceMessage message;

    if(this->useProtobuf)
        message = VeniceMessage::fromProtoBuf(data);
    else
        message = VeniceMessage::fromJson(data);

    if(message.getMessageId()!=-1)
    {

        if(!this->fileMessages.contains(message.getMessageId()))
            this->fileMessages.insert(message.getMessageId(), message);

        qDebug() << "[FileReceiver] Creating Ack for Message with id " << message.getMessageId();
        VeniceMessage ackMessage(message.getMessageId(), true, vector<byte>());


        qDebug() << "[FileReceiver] Sending Ack Message with id " << ackMessage.getMessageId();
        if(this->useProtobuf)
            this->write(ackMessage.toProtoBuf());
        else
            this->write(ackMessage.toJson().append('\n'));
        //this->flush();
        this->waitForBytesWritten();

        if(this->fileMessages.size() == this->fileMetadata->getChunkCount())
        {
            qDebug() << "[FileReceiver] File data received, creating file";

            this->saveReceivedFile();

            this->disconnectFromHost();

        }
            //Raise an event to the GUI show a message
    }




}

void FileReceiver::dealWithDisconnection()
{
    qDebug() << "[FileReceiver] Disconnected from server.";
}

void FileReceiver::dealWithError(QAbstractSocket::SocketError socketError)
{
    qWarning() << "[FileReceiver] Socket error:" << socketError << this->errorString();
}

void FileReceiver::saveReceivedFile()
{
    QFile receivedFile(this->fileMetadata->getFullFilePath());

    if (receivedFile.open(QIODevice::WriteOnly)) {


        for (auto messageIterator = fileMessages.begin(); messageIterator != fileMessages.end(); ++messageIterator) {
            qDebug() << "[FileReceiver] Creating file with message ID :" << messageIterator.value().getMessageId();

            vector<byte> currentFileChunk = messageIterator.value().getData();

            receivedFile.write(reinterpret_cast<const char*>(currentFileChunk.data()), currentFileChunk.size());
        }

        receivedFile.close();

        qDebug() << "[FileReceiver] File saved : "<<receivedFile.fileName();

    }
    else
    {
        qWarning() << "[FileReceiver]  Cannot create file:" << receivedFile.errorString();
        return;
    }
}
