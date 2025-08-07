#ifndef FILERECEIVER_H
#define FILERECEIVER_H

#include <QTcpSocket>

#include "include/metadata/channelmetadata.h"
#include "include/metadata/filemetadata.h"
#include "include/network/venicemessage.h"

class FileReceiver: public QTcpSocket
{
public:
    /**
     * @brief FileReceiver Constructor of the class
     * @param parent The parent of the object
     * @param channelMetadata Metadata related to the data channel
     * @param fileMetadata Metadata related to the file to be received
     */
    FileReceiver(QObject *parent, ChannelMetadata *channelMetadata, FileMetadata *fileMetadata);

    /**
     * Destructor of the class
     */
    ~FileReceiver();

    /**
     * @brief startFileReception Creates a connection with the file provider and retrieves the file
     */
    void startFileReception();

private:
    /**
     * @brief channelMetadata The channel metadata
     */
    ChannelMetadata *channelMetadata;

    /**
     * @brief fileMetadata The file metadata
     */
    FileMetadata *fileMetadata;

    /**
     * @brief useProtobuf Use proto buffer protocol for serialization
     */
    bool useProtobuf;

    /**
     * @brief fileMessages The venice messages containing the transfered file
     */
    QMap<int, VeniceMessage> fileMessages;

    /**
     * @brief currentDatalength Data length expected of the next file chunk (VEniceMessage) to be read
     */
    int currentDatalength = -1;

    /**
     * @brief saveReceivedFile Saves the receive file in the directory indicated by fileMetadata
     */
    void saveReceivedFile();

private slots:

    /**
     * @brief processConnectionToSender Print a message to indicate that the connection to server is done
     */
    void processConnectionToSender();

    /**
     * @brief processMessageReceived Read and process a received message
     */
    void processMessageReceived();

    /**
     * @brief dealWithDisconnection Print a message indicating the final of the connection with the file sender
     */
    void dealWithDisconnection();

    /**
     * @brief dealWithError Print the error related with the file sender connection
     * @param socketError The related error
     */
    void dealWithError(QAbstractSocket::SocketError socketError);

};

#endif // FILERECEIVER_H
