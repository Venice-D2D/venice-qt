#ifndef VENICESERVICE_H
#define VENICESERVICE_H

#include "veniceservice.h"
#include "venicemessage.h"

#include <QtCore/QThread>
#include <string>

using namespace std;


class VeniceService: public QThread
{
        Q_OBJECT
public:
    /**
     * @brief VeniceServiceThread constuctor
     * @param parent of the thread. It is a MainWindow instance
     */
    VeniceService(QObject *parent = nullptr, string filePath = "");

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

    /**
     * @brief runFileServiceProvider creates and run the venice service
     * @param thread executing the service
     */
    void runFileServiceProvider();

    /**
     * @brief readFileData read the file by separing into chunks of max_size
     * @param name The name of the file to be read
     * @param max_size The maximun size of chunks
     * @return A vector containing VeniceMessage with data with a size less or equals than max_size
     */
    vector<VeniceMessage> readFileData(const string& name, const int& max_size);


};

#endif // VENICESERVICE_H
