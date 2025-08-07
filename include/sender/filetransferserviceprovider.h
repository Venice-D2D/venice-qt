#ifndef FILETRANSFERSERVICEPROVIDER_H
#define FILETRANSFERSERVICEPROVIDER_H

#include "include/network/venicemessage.h"
#include "include/channel/datachannel.h"
#include "include/channel/bootstrapchannel.h"
#include "include/channel/wifidatachannel.h"
#include "include/sender/venicebluetoothuuid.h"
#include "include/metadata/channelmetadata.h"


#include <QtCore/QThread>
#include <QString>
#include <QTcpSocket>

using namespace std;

class FileSender;

/**
 * @brief The VeniceService class orchestrates the service advertisement and data
 * exchange
 */
class FileTransferServiceProvider: public QThread
{
        Q_OBJECT
public:
    /**
     * @brief FileTransferServiceProvider Constructor of the class
     * @param dataChannel The channel to data transfer
     * @param BoostrapChannel The channel to expose the file service transfer
     * @param filePath The path of the file to be transfered
     * @param useProtobuf true indicates that protobuf is used for serialisation. If false, json is used instead
     * @param parent of the thread. It is a MainWindow instance
     */
    FileTransferServiceProvider(DataChannel *dataChannel, BootstrapChannel *bleBoostrapChannel, string filePath, bool useProtobuf, QObject *parent = nullptr);

    /**
      * @brief VeniceServiceThread destructor
      */
    ~FileTransferServiceProvider();

    /**
     * @brief run executes the thread
     */
    void run() override;

    /**
     * @brief configureBoostrapAdapter Configures the boostrap channel related to the service
     * @exception VeniceException if there is an issue configuration the channel
     */
    void configureBoostrapChannel() throw();

    /**
     * @brief WIFI_DATA_CHANNEL The id for the used data channel
     */
    static const QString WIFI_DATA_CHANNEL;

private:

    /**
     * @brief filePath The path of the file to be shared
     */
    string filePath;

    /**
     * @brief MAX_MESSAGE_SIZE Max message size in Venice
     */
    const int MAX_MESSAGE_SIZE = 1000;



    /**
     * @brief dataChannel The object to configure and manage the data channel
     */
    DataChannel *dataChannel;


    /**
     * @brief bleBootstrapChannel The object to configure and manage the bootstrap channel
     */
    BootstrapChannel *bootstrapChannel;

    /**
     * @brief channelMetadata The metadata related to the data channel
     */
    ChannelMetadata *channelMetadata=nullptr;

    /**
     * @brief fileSender The file sender
     */
    FileSender *fileSender=nullptr;

    /**
     * @brief useProtobuf Use protobuf for data serialisation
     */
    bool useProtobuf;

    /**
     * @brief runFileServiceProvider creates and run the venice service
     * @param thread executing the service
     */
    void runFileServiceProvider();

    /**
     * @brief readFileData read the file by separing into chunks of max_size
     * @param name The name of the file to be read
     * @param max_size The maximun size of chunks
     * @return A QVector containing VeniceMessages with data with a size less or equals than max_size
     */
    QVector<VeniceMessage*> readFileData(const string& name, const int& max_size);

    /**
     * @brief configureDataChannel Configures the data channel related to the service
     * @exception VeniceException if there is an issue configuration the channel
     */
    void configureDataChannel() throw();
};

#endif // FILETRANSFERSERVICEPROVIDER_H
