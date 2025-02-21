#ifndef FILETRANSFERSERVICE_H
#define FILETRANSFERSERVICE_H


#include <QTcpServer>
#include <QNetworkInterface>
#include <QFile>
#include <QMap>
#include <QVector>
#include <QReadWriteLock>

#include <string>

#include "include/network/venicemessage.h"
#include "include/event/venicetimer.h"

using namespace std;



/**
 * @brief The FileTransferService class enables file transfers via the Wifi Channel
 */
class FileTransferService:  public QTcpServer
{

public:
    /**
     * @brief FileTransferService constructor of the service which creates a TCP Server
     * @param parent of the service
     * @param filePath The path to the file to be shared
     * @param fileMessages Messages containing the file data
     * @param ipAddress The IP address for sharing the file
     * @param port The port used for listening
     */
    FileTransferService(QObject *parent, const char* filePath, QVector<VeniceMessage*> fileMessages, QHostAddress ipAddress, quint16 port) throw();

    /**
      * @brief ~FileTransferService destructor of the service that closes the TCP server
      */
    ~FileTransferService();


private:
    // The port for listening
    int port;

    // The file path
    //const char* filePath;

    // The IP address for sharing the file
    QHostAddress ipAddress;

    //The messages related to the file
    //vector<VeniceMessage> file_related_messages;

    //The file descriptor related to the file to be sent
    QFile fileToSend;

    //The socket to interact with the client receiving the file
    QTcpSocket *clientSocket;


    //The list of messages related to the file to send
    QVector<VeniceMessage*> fileMessages;

    QMap<int, VeniceTimer*> resubmissionTimers;

    //Lock used for thread safe management of file_messages and resubmissionTimers
    QReadWriteLock lock;

    //Use proto buffer protocol for serialization
    bool useProtobuf;



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
