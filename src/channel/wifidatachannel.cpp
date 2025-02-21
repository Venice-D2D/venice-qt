#include <QTcpServer>
#include <QProcess>

#include "include/channel/wifidatachannel.h"
#include "include/exception/notsuitablewifiadapterfoundveniceexception.h"
#include "include/exception/notavailableportfoundveniceexception.h"


using namespace std;

/*WifiDataChannel::WifiDataChannel(const char * ip, const char * gateway, const char * netmask):DataChannel(QNetworkInterface::InterfaceType::Wifi)
{
    this->ip = ip;
    this->gateway = gateway;
    this->netmask = netmask;
    this->adhocNetworkName = "VeniceAdhocNetwork";
}*/

WifiDataChannel::WifiDataChannel():DataChannel(QNetworkInterface::InterfaceType::Wifi)
{
    this->port = -1;
}

WifiDataChannel::~WifiDataChannel()
{
    //system("service NetworkManager start");
}

void WifiDataChannel::configureChannel()
{
    this->wifiInterface = DataChannel::searchNetworkInterfaceByType(QNetworkInterface::InterfaceType::Wifi);
    qDebug() << "Wifi Adapter set!";

    this->searchForAvailablePort();
    qDebug() << "Wifi Port set!";

    this->searchForSSID();
    qDebug() << "SSID retrieved!";


    /*if(this->networkInterfaceName.isNull() || this->networkInterfaceName.isEmpty())
    {
        qDebug() << "There is no Wifi adapter to configure an ad-hoc network!";
    }
    else
    {
        qDebug() << "Configuring Wifi adapter for creating an ad-hoc network...";

        char cmd[128];


        //network interface name
        const char* networkInterfaceName = {this->networkInterfaceName.toStdString().c_str()};

        //stop the Network manager
        sprintf(cmd,"systemctl stop NetworkManager");
        this->executeSystemCommand(cmd);

        FILE *pipe = popen(cmd, "r");
        if (pipe) {
            char buffer[128];
            while (!feof(pipe)) {
                if (fgets(buffer, 128, pipe) != nullptr) {
                    // Process output line by line (here, printing)
                    printf("%s", buffer);
                }
            }
            pclose(pipe);
        }
        else
             qDebug() << "Error with command: "<< cmd;*/


        /*int result = system(cmd);
        memset(cmd,0x00,64);
        qDebug() << "Result of command execution " << result;*/

        //link down command in Linux
        /*sprintf(cmd,"ip link set %s down",networkInterfaceName);
        this->executeSystemCommand(cmd);
        memset(cmd,0X00,64);

        //set the network interface to ad-hoc mode
        sprintf(cmd,"iwconfig %s mode Ad-hoc",networkInterfaceName);
        this->executeSystemCommand(cmd);
        memset(cmd,0X00,64);

        //set the network name
        sprintf(cmd,"iwconfig %s essid %s",networkInterfaceName, this->adhocNetworkName);
        this->executeSystemCommand(cmd);
        memset(cmd,0X00,64);

        //remove security
        sprintf(cmd,"iwconfig %s key off ",networkInterfaceName);
        this->executeSystemCommand(cmd);
        memset(cmd,0X00,64);

        //command to set ip address, netmask
        sprintf(cmd,"ifconfig %s %s netmask %s",networkInterfaceName,this->ip,this->netmask);
        this->executeSystemCommand(cmd);
        memset(cmd,0X00,64);

        //command to set gateway
        //sprintf(cmd,"route add default gw %s %s",this->gateway,networkInterfaceName);
        //qDebug() << "Executing command "<< cmd;
        //system(cmd);

        //memset(cmd,0X00,64);

        //link up commanda
        sprintf(cmd,"ip link set %s up",networkInterfaceName);
        qDebug() << "\nExecuting command: "<< cmd;
        system(cmd);
        memset(cmd,0X00,64);*/


    //}
}

void WifiDataChannel::restoreChannelConfiguration()
{
    system("systemctl start NetworkManager");
}

/*const char* WifiDataChannel::getAdhocNetworkName()
{
    return this->adhocNetworkName;
}*/

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

void WifiDataChannel::searchForAvailablePort() throw()
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

void WifiDataChannel::searchForSSID() throw()
{
    //Get all wifi adapters with its ssid, device name and status (active = yes or not).
    //The colons names are removed (-t option) - Several results are possible:
    //eduroam:wlp3s0:yes
    //INRIA-interne:wlp3s0:no
    //INRIA-guest:wlp3s0:no
    QString nmcliWifiAdapterFilter = "nmcli -t -f ssid,device,active device wifi";

    //The IP of related to the channel
    QString ip = this->getNetworkAddress().ip().toString(); //.toStdString(); //toUtf8().constData();

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






