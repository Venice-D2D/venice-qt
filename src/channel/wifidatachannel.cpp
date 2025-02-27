#include "include/channel/wifidatachannel.h"


#include <QTcpServer>
#include <QProcess>




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
    this->wifiInterface = DataChannel::searchNetworkInterfaceByType(QNetworkInterface::InterfaceType::Wifi);
    qDebug() << "Wifi Adapter set!";

    this->searchForAvailablePort();
    qDebug() << "Wifi Port set!";

    this->searchForSSID();
    qDebug() << "SSID retrieved!";
}

void WifiDataChannel::restoreChannelConfiguration()
{
    system("systemctl start NetworkManager");
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
    //Get all wifi adapters with its ssid, device name and status (active = yes or not).
    //The colons names are removed (-t option) - Several results are possible:
    //eduroam:wlp3s0:yes
    //INRIA-interne:wlp3s0:no
    //INRIA-guest:wlp3s0:no
    QString nmcliWifiAdapterFilter = "nmcli -t -f ssid,device,active device wifi";

    //The IP of related to the channel
    QString ip = this->getNetworkAddress().ip().toString();

    //Filter the wifi adapters as inputs for only considering the ones with a given device name
    //The device name is getted by using the channel ip - Several results are possible.
    // - nmcli -f GENERAL.DEVICE,IP4.ADDRESS device show :
    //      GENERAL.DEVICE:                         wlp3s0
    //      IP4.ADDRESS[1]:                         192.168.1.20/24
    //      GENERAL.DEVICE:                         eno1
    //      IP4.ADDRESS[1]:                         193.51.235.150/25
    // - grep -B 1 <HOST_IP> (taking as input the previous nmcli output)
    //      GENERAL.DEVICE:                         wlp3s0
    //      IP4.ADDRESS[1]:                         192.168.1.20/24
    // - grep -E "GENERAL.DEVICE" taking as input the previous grep output)
    //      GENERAL.DEVICE:                         wlp3s0
    // - awk '{print $2}' taking as input the previous grep output)

    QString grepWifiAdaptersFilterByDeviceName = "grep $(nmcli -f GENERAL.DEVICE,IP4.ADDRESS device show "
                                                     "| grep -B 1 \""+ip+"\""+
                                                     " | grep -E \"GENERAL.DEVICE\"|awk '{print $2}')";

    //Filter the wifi adapters to get only the one that is active
    QString grepWifiAdapterFilterByStatus = "grep ':yes'";

    //Extract the ssid related to the active wifi adapter
    QString grepWifiAdapterSSIDFilter = "grep -o '^[^:]*'";

   const QString commandToGetSSID = nmcliWifiAdapterFilter+"|"+
                               grepWifiAdaptersFilterByDeviceName+"|"+
                               grepWifiAdapterFilterByStatus+"|"+
                               grepWifiAdapterSSIDFilter;

   //Command execution
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

}






