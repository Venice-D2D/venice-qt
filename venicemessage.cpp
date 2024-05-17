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

int VeniceMessage::get_size()
{
    return this->data.size();
}
