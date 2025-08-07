#ifndef CHANNELMETADATA_H
#define CHANNELMETADATA_H

#include <QString>

class ChannelMetadata
{
public:
    /**
     *
     * @brief ChannelMetadata Constructor of the class
     * @param channelIdentifier The venice channel id
     * @param address The physical address related to the channel
     * @param networkIdentifier The id of the network, access point...
     * @param password The password to access the network, access point..
     * @param port The physical port related to the channel
     */
    ChannelMetadata(QString channelIdentifier, QString address, QString networkIdentifier, QString password, quint16 port);

    /**
     * @brief The destructor of the class
     */
    ~ChannelMetadata();


    /**
     * @brief toString Returns the channel metadata as a string
     * @return string with format <channelIdentifier>;<address>;<networkIdentifier>;<password>;<port>
     */
    QString toString();

    /**
     * @brief getChannelIdentifier Returns the Channel Identifier
     * @return the channel Identifier
     */
    QString getChannelIdentifier();

    /**
     * @brief getAddress Returns the address
     * @return the address
     */
    QString getAddress();

    /**
     * @brief getPort Returns the port
     * @return the port
     */
    quint16 getPort();

    /**
     * @brief getNetworkIdentifier Returns the network identifier
     * @return the network identifier
     */
    QString getNetworkIdentifier();

    /**
     * @brief getPassword Returns the password
     * @return the password
     */
    QString getPassword();


private:
    /**
     * @brief identifier The channel identifier used by Venice Service.
     */
    QString channelIdentifier;

    /**
     * @brief address Physical adress of the File Sender (server socket)
     */
    QString address;

    /**
     * @brief port Port used by the File Sender (server socket)
     */
    quint16 port;

    /**
     * @brief networkIdentifier Name of the network, access point... related to the channel
     */
    QString networkIdentifier;

    /**
     * @brief password Password required to connect to the network
     */
    QString password;

};

#endif // CHANNELMETADATA_H
