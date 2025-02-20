#include "venicemessage.h"

VeniceMessage::VeniceMessage(int messageId, bool ack, vector<byte> data)
{
    this->ack = ack;
    this->messageId = messageId;
    this->data = data;
}

VeniceMessage::~VeniceMessage()
{
}

int VeniceMessage::getSize()
{
    return this->data.size();
}

int VeniceMessage::getMessageId()
{
    return this->messageId;
}

QByteArray VeniceMessage::toJson(){
    QJsonObject jsonRepresentation;
    jsonRepresentation["ack"] = this->ack;
    jsonRepresentation["messageId"] = this->messageId;

    QByteArray dataByteArray(reinterpret_cast<const char*>(this->data.data()), static_cast<int>(this->data.size()));
    jsonRepresentation["data"] = QString::fromLatin1(dataByteArray.toBase64());

    QJsonDocument jsonDocument(jsonRepresentation);
    return jsonDocument.toJson();
}

// Deserialize from JSON
VeniceMessage VeniceMessage::fromJson(const QByteArray &messageJson) {
    QJsonDocument jsonDocument = QJsonDocument::fromJson(messageJson);
    QJsonObject jsonRepresentation = jsonDocument.object();

    QByteArray dataByteArray = QByteArray::fromBase64(jsonRepresentation["data"].toString().toLatin1());
    vector<byte> data = std::vector<std::byte>(reinterpret_cast<const std::byte*>(dataByteArray.data()),
                                             reinterpret_cast<const std::byte*>(dataByteArray.data() + dataByteArray.size()));

    VeniceMessage message(jsonRepresentation["messageId"].toInt(), jsonRepresentation["ack"].toBool(), data);

    return message;
}

bool VeniceMessage::isAck(){
    return this->ack;
}

QByteArray VeniceMessage::toProtoBuf(){

    VeniceMessageProto protoMessage;

    //protoMessage.set_data(this->data);

    /*for(const byte &current_chunk : this->data){
        protoMessage.add_data(current_chunk);
    }*/

    qDebug() << "Adding data to protobuf message...";

    protoMessage.set_data(reinterpret_cast<const char*>(this->data.data()), this->data.size());

    /*for (const byte &current_chunk : this->data) {

        qDebug() << "Adding chunk to protobuf message...";
        protoMessage.add_data(reinterpret_cast<const char*>(current_chunk));
    }*/


    protoMessage.set_messageid(this->messageId);
    protoMessage.set_ack(this->ack);

    std::string serializedMessage;
    if (!protoMessage.SerializeToString(&serializedMessage)) {

        qDebug() << "Failed to serialize Protobuf object.";
        return QByteArray();
    }

    // Send the serialized data
    return QByteArray(serializedMessage.c_str(), serializedMessage.size());

}
