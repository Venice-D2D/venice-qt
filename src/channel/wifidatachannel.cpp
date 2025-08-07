#include "include/channel/wifidatachannel.h"
#include "include/tool/networkinformationmanager.h"


#include <QTcpServer>
#include <QProcess>
#include <QRegularExpression>

using namespace std;


WifiDataChannel::WifiDataChannel():DataChannel(QNetworkInterface::InterfaceType::Wifi)
{
    this->port = -1;
}

WifiDataChannel::~WifiDataChannel()
{
    //system("service NetworkManager start");
}

void WifiDataChannel::configure() throw()
{
    this->wifiInterface = NetworkInformationManager::searchActiveNetworkInterfaceByType(QNetworkInterface::InterfaceType::Wifi);
    this->networkInterfaceName = this->wifiInterface.name();
    qDebug() << "Wifi Adapter set!";

    this->searchForAvailablePort();
    qDebug() << "Wifi Port set!";

    this->searchForSSID();
    qDebug() << "SSID retrieved!";
}

void WifiDataChannel::restoreChannelConfiguration()
{
}

void WifiDataChannel::executeSystemCommand(const char* command)
{
    qDebug() << "Executing command: "<< command;
    int result = system(command);
    qDebug() << "Result of command execution: " << result;
}

int WifiDataChannel::getPort()
{
    return this->port;
}

QNetworkInterface WifiDataChannel::getWifiInterface()
{
    return this->wifiInterface;
}



QString WifiDataChannel::getSSID()
{
    return this->ssid;
}

QNetworkAddressEntry WifiDataChannel::getNetworkAddress()
{
    qDebug() << "Current Wifi Interface Entries" << this->wifiInterface.addressEntries().size();
    return this->wifiInterface.addressEntries().first();
}

void WifiDataChannel::searchForAvailablePort()
{
    QTcpServer server; //Server to check that the port is available

    int currentPort = STARTING_PORT;

    qDebug() << "Starting Ports checking "<< currentPort;

    while(!server.listen(this->getNetworkAddress().ip(), currentPort) && currentPort < ENDING_PORT)
    {
        qDebug() << "Port failed "<< currentPort;
        ++currentPort;
    }

    server.close();

    if(currentPort>=ENDING_PORT)
        throw NotAvailablePortFoundVeniceException();

    this->port = currentPort;

}

void WifiDataChannel::searchForSSID()
{

    const QString commandToGetSSID = "iwgetid "+this->networkInterfaceName+" -r";

    QProcess processCommandExecution;
    QStringList arguments;

    arguments << "-c" << commandToGetSSID;

    qDebug().noquote() << "Running command:" << arguments.join(" ");

    //We execute bash as program and pass the actual command via arguments
    processCommandExecution.start("/bin/bash", arguments);


    if(!processCommandExecution.waitForFinished())
    {
        QString error = processCommandExecution.errorString();
        qWarning() << "Process did not finish successfully:" << error;
        qDebug() << "Error running the command";

    }

    // Capture the ssid
    qDebug().noquote() << "Command to get the ssid"<< commandToGetSSID;
    this->ssid = processCommandExecution.readAllStandardOutput();

    qDebug() << "errors... " << processCommandExecution.readAllStandardError();

    qDebug().noquote() << "Result of the command for ssid"<< this->ssid;

    if(this->ssid.isNull() || this->ssid.isEmpty())
        throw NotSuitableWifiAdapterFoundVeniceException();

    this->ssid.remove(QRegularExpression("[\\r\\n]"));


}
