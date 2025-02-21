#ifndef VENICEMESSAGE_H
#define VENICEMESSAGE_H

#include <vector>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <QJsonArray>
#include <QDebug>

#include "external/protobuf/cpp_proto/venice.pb.h"

using namespace std;

/**
 * @brief The VeniceMessage class representing a message service containing required information for data exchange
 * between venice devices
 */
class VeniceMessage
{
public:
    /**
     * @brief VeniceMessage constructor
     * @param messageId The message identifier
     * @param ack The acknolegement related to the message
     * @param data The data contained by the message
     */
    VeniceMessage(int messageId, bool ack, vector<byte> data);

    /**
      * @brief VeniceMessage destructor
     */
    ~VeniceMessage();

    /**
     * @brief getSize returns the size in bytes of the message data
     * @return size in bytes
     */
    int getSize();

    /**
     * @brief VeniceMessage::toJson Serielization of the message to json format
     * @return Representation of the message in QByteArray format
     */
    QByteArray toJson();

    /**
     * @brief VeniceMessage::toProtoBuf Serielization of the message to protobuf format
     * @return
     */
    QByteArray toProtoBuf();

    /**
     * @brief isAck Return ack which indicates if the current message is an acknowlegement
     * @return True if acknowledgement, False otherwise
     */
    bool isAck();


    /**
     * @brief getMessageId Return the id of the message
     * @return The id of the message
     */
    int getMessageId();


    /**
     * @brief VeniceMessage::fromJson Deserielization of of the message
     * @param jsonMessageData The ocject represention in json
     * @return The message represented by the json document
     */
    static VeniceMessage fromJson(const QByteArray &jsonMessageData);

private:

    // The message identifier
    int messageId;

    // Indicates if it is an acknowlegement message
    bool ack;

    // The message data
    vector<byte> data;
};

#endif // VENICEMESSAGE_H
