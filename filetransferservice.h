#ifndef FILETRANSFERSERVICE_H
#define FILETRANSFERSERVICE_H


#include <QTcpServer>
#include <QNetworkInterface>

#include <string>

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
     * @param ipAddress The IP address for sharing the file
     * @param port The port used for listening
     */
    FileTransferService(QObject *parent, string filePath, QHostAddress ipAddress, quint16 port);

    /**
      * @brief ~FileTransferService destructor of the service that closes the TCP server
      */
    ~FileTransferService();


private:
    // The port for listening
    int port;

    // The file path
    string filePath;

    // The IP address for sharing the file
    QHostAddress ipAddress;


};

#endif // FILETRANSFERSERVICE_H
