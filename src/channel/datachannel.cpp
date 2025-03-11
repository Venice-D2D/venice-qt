#include "include/channel/datachannel.h"
#include "include/exception/notsuitablewifiadapterfoundveniceexception.h"

DataChannel::DataChannel(QNetworkInterface::InterfaceType networkInterfaceType)
{
    this->networkInterfaceType = networkInterfaceType;

    //this->networkInterfaceName = DataChannel::getNetworkInterfaceNameByType(this->networkInterfaceType);
}

DataChannel::~DataChannel()
{

}
