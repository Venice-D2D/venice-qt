#ifndef VENICESERVICE_H
#define VENICESERVICE_H

#include "include/network/venicemessage.h"
#include "include/channel/datachannel.h"
#include "include/channel/bootstrapchannel.h"
#include "include/channel/wifidatachannel.h"
#include "include/service/venicebluetoothuuid.h"
#include "include/service/filetransferserver.h"


#include <QtCore/QThread>
#include <string>
#include <QTcpSocket>

using namespace std;

/**
 * @brief The VeniceService class orchestrates the service advertisement and data
 * exchange
 */
class FileTransferService: public QThread
{
        Q_OBJECT
public:
    /**
     * @brief VeniceServiceThread constructor
     * @param dataChannel
     * @param boostrapChannel
     * @param filePath The path of the file to be transfered
     * @param parent of the thread. It is a MainWindow instance
     */
    FileTransferService(DataChannel *dataChannel, BootstrapChannel *boostrapChannel, string filePath, QObject *parent = nullptr);

    /**
      * @brief VeniceServiceThread destructor
      */
    ~FileTransferService();

    /**
     * @brief run executes the thread
     */
    void run() override;

private:

    // The path of the file to be shared
    string filePath;

    //Max message size in Venice
    const int MAX_MESSAGE_SIZE = 1000;
    const string WIFI_DATA_CHANNEL = "wifi_data_channel";

    //The object to configure and manage the data channel
    DataChannel *dataChannel;

    //The object to configure and manage the bootstrap channel
    BootstrapChannel *bootstrapChannel;

    //TCP Server used to deal with file transfer
    QTcpServer tcpServer(nullptr_t);

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

    /**
     * @brief configureBoostrapAdapter Configures the boostrap channel related to the service
     * @exception VeniceException if there is an issue configuration the channel
     */
    void configureBoostrapChannel() throw();


};

#endif // VENICESERVICE_H
