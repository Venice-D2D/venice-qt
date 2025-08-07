#ifndef FILESENDER_H
#define FILESENDER_H


#include <QTcpServer>
#include <QNetworkInterface>
#include <QFile>
#include <QMap>
#include <QVector>
#include <QReadWriteLock>
#include <QTcpSocket>

#include <string>

#include "include/network/venicemessage.h"
#include "include/event/venicetimer.h"
#include "include/sender/filetransferserviceprovider.h"
#include "include/metadata/channelmetadata.h"

using namespace std;



/**
 * @brief The FileTransferService class enables file transfers via the Wifi Channel
 */
class FileSender: public QTcpServer
{

public:
    /**
     * @brief FileSender constructor of the TCP Server for sending files
     * @param parent of the server
     * @param fileMessages Messages containing the file data
     * @param channelMetadata The IP address and port for sharing the file
     * @param fileTransferService The file transfer service related to the sender
     * @param useProtobuf true indicates that protobuf is used for serialisation. If false, json is used instead
     */
    FileSender(QObject *parent, QVector<VeniceMessage*> fileMessages, ChannelMetadata *channelMetadata, FileTransferServiceProvider *fileTransferServiceProvider, bool useProtobuf);

    /**
      * @brief ~FileTransferService destructor of the service that closes the TCP server
      */
    ~FileSender();

    /**
     * @brief listenForConnections Configures the server for listening
     */
    void listenForConnections();

    using QTcpServer::listen;


private:

    /**
     * @brief channelMetadata The information related to the data channel
     */
    ChannelMetadata *channelMetadata;


    /**
     * @brief clientSocket The socket to interact with the client receiving the file
     */
    QTcpSocket *clientSocket;

    /**
     * @brief fileMessages The list of messages related to the file to send
     */
    QVector<VeniceMessage*> fileMessages;

    /**
     * @brief resubmissionTimers Timer for resending a message if not ack
     */
    QMap<int, VeniceTimer*> resubmissionTimers;

    /**
     * @brief lock Lock used for thread safe management of file_messages and resubmissionTimers
     */
    QReadWriteLock lock;

    /**
     * @brief useProtobuf Use proto buffer protocol for serialization
     */
    bool useProtobuf;

    /**
     * @brief fileTransferServiceProvider The related File Transfer Service
     */
    FileTransferServiceProvider *fileTransferServiceProvider;



private slots:
    void onBytesWritten(qint64 bytes);
    void onError(QAbstractSocket::SocketError socketError);
    void onDataReadyToBeRead();

protected:

    void incomingConnection(qintptr socketDescriptor) override;

    /**
     * @brief sendVeniceMessage Send the next available message and configure a timer for waiting the message acknoledgement
     */
    void sendNextVeniceMessage();

    /**
     * @brief sendVeniceMessages Send messages related to file. It considers not only fileMessages vector but also resubmission timers map
     */
    void sendVeniceMessages();

    VeniceTimer* sendVeniceMessage(VeniceMessage* message);


};

#endif // FILESENDER_H
