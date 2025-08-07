#include "include/network/venicemessage.h"
//#include <QtProtobuf/QProtobufSerializer>
//#include <QtProtobuf/QAbstractProtobufSerializer>

VeniceMessage::VeniceMessage(int messageId, bool ack, vector<byte> data)
{
    this->ack = ack;
    this->messageId = messageId;
    this->data = data;
}

VeniceMessage::VeniceMessage()
{
    this->ack = false;
    this->messageId = -1;
    this->data = vector<byte>();
}


VeniceMessage::VeniceMessage(VeniceMessageProto messageProto)
{
    this->ack = messageProto.ack();
    this->messageId = messageProto.messageid();
    string messageData = messageProto.data();


    this->data = vector<byte>();
    this->data.reserve(messageData.size());

    for (char currentChar : messageData) {
        this->data.push_back(static_cast<std::byte>(currentChar));
    }
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

vector<byte> VeniceMessage::getData(){
    return this->data;
}

QByteArray VeniceMessage::toProtoBuf(){

    VeniceMessageProto protoMessage;


    qDebug() << "[VeniceMessage] Adding data to protobuf message...";

    protoMessage.set_data(reinterpret_cast<const char*>(this->data.data()), this->data.size());


    protoMessage.set_messageid(this->messageId);
    protoMessage.set_ack(this->ack);

    std::string serializedMessage;
    if (!protoMessage.SerializeToString(&serializedMessage)) {

        qDebug() << "[VeniceMessage] Failed to serialize Protobuf object.";
        return QByteArray();
    }

    // Send the serialized data
    return QByteArray(serializedMessage.c_str(), serializedMessage.size());
}

VeniceMessage VeniceMessage::fromProtoBuf(const QByteArray &protoBufMessageData)
{
    VeniceMessageProto protoMessage;

    //QProtobufSerializer serializer;

    //serializer.deserialize(&protoMessage, protoBufMessageData);

    std::string serializedMessage(protoBufMessageData.constData(), protoBufMessageData.size());

    if(!(protoMessage.ParseFromString(serializedMessage))){//ParseFromArray(protoBufMessageData.constData(), protoBufMessageData.size()))){
        qWarning() << "[VeniceMessage] Failed to parse protobuf data";

        QByteArray hexDump = protoBufMessageData.toHex(' ');
        qDebug() << "[VeniceMessage] Raw bytes (hex):" << hexDump;

        return VeniceMessage();
    }

    /*if (serializer.lastError() != QAbstractProtobufSerializer::Error::None) {
        qWarning() << "[VeniceMessage] Error:" << serializer.lastErrorString();
    }*/

    return VeniceMessage(protoMessage);
}
