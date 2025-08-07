#ifndef FILETRANSFERSERVICE_H
#define FILETRANSFERSERVICE_H


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
#include "include/service/filetransferservice.h"

using namespace std;



/**
 * @brief The FileTransferService class enables file transfers via the Wifi Channel
 */
class FileTransferServer: public QTcpServer
{

public:
    /**
     * @brief FileTransferServer constructor of the TCP Server
     * @param parent of the server
     * @param fileMessages Messages containing the file data
     * @param ipAddress The IP address for sharing the file
     * @param port The port used for listening
     * @param fileTransferService The file transfer service related to the server
     */
    FileTransferServer(QObject *parent, QVector<VeniceMessage*> fileMessages, QHostAddress ipAddress, quint16 port, FileTransferService *fileTransferService);

    /**
      * @brief ~FileTransferService destructor of the service that closes the TCP server
      */
    ~FileTransferServer();

    /**
     * @brief listenForConnections Configures the server for listening
     */
    void listenForConnections();

    using QTcpServer::listen;


private:

    // The port for listening
    quint16 port;

    // The IP address for sharing the file
    QHostAddress ipAddress;

    //The messages related to the file
    //vector<VeniceMessage> file_related_messages;

    //The socket to interact with the client receiving the file
    QTcpSocket *clientSocket;


    //The list of messages related to the file to send
    QVector<VeniceMessage*> fileMessages;

    QMap<int, VeniceTimer*> resubmissionTimers;

    //Lock used for thread safe management of file_messages and resubmissionTimers
    QReadWriteLock lock;

    //Use proto buffer protocol for serialization
    bool useProtobuf;

    //The related File Transfer Service
    FileTransferService *fileTransferService;



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

#endif // FILETRANSFERSERVICE_H
