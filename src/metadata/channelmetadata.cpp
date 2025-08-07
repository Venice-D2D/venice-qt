#include "include/metadata/channelmetadata.h"

ChannelMetadata::ChannelMetadata(QString channelIdentifier, QString address, QString networkIdentifier, QString password, quint16 port) {

    this->channelIdentifier = channelIdentifier;
    this->address = address;
    this->networkIdentifier = networkIdentifier;
    this->password = password;
    this->port = port;
}


ChannelMetadata::~ChannelMetadata(){}


QString ChannelMetadata::toString(){
    QString channelMetadataString = this->channelIdentifier+";"+this->address+";"+this->networkIdentifier+";"+this->password+";"+QString::number(this->port);

    if(this->password=="")
        channelMetadataString = this->channelIdentifier+";"+this->address+";"+this->networkIdentifier+";"+QString::number(this->port);

    return channelMetadataString;
}


QString ChannelMetadata::getChannelIdentifier(){
    return this->channelIdentifier;

}

QString ChannelMetadata::getAddress(){
    return this->address;
}

quint16 ChannelMetadata::getPort(){
    return this->port;
}

QString ChannelMetadata::getNetworkIdentifier(){
    return this->networkIdentifier;
}

QString ChannelMetadata::getPassword(){
    return this->password;
}
