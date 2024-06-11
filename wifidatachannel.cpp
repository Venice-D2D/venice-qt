#include "wifidatachannel.h"

//#include <string.h>

//using namespace std;

WifiDataChannel::WifiDataChannel(const char * ip, const char * gateway, const char * netmask):DataChannel(QNetworkInterface::InterfaceType::Wifi)
{
    this->ip = ip;
    this->gateway = gateway;
    this->netmask = netmask;
    this->adhocNetworkName = "VeniceAdhocNetwork";
}

WifiDataChannel::~WifiDataChannel()
{
    system("service NetworkManager start");
}

void WifiDataChannel::configureChannel()
{
    if(this->networkInterfaceName.isNull() || this->networkInterfaceName.isEmpty())
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

        /*FILE *pipe = popen(cmd, "r");
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
        sprintf(cmd,"ip link set %s down",networkInterfaceName);
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
        memset(cmd,0X00,64);


    }
}

void WifiDataChannel::restoreChannelConfiguration()
{
    system("systemctl start NetworkManager");
}

const char* WifiDataChannel::getAdhocNetworkName()
{
    return this->adhocNetworkName;
}

void WifiDataChannel::executeSystemCommand(const char* command)
{
    qDebug() << "Executing command: "<< command;
    int result = system(command);
    qDebug() << "Result of command execution: " << result;
}
