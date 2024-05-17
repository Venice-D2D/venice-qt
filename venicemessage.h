#ifndef VENICEMESSAGE_H
#define VENICEMESSAGE_H

#include <vector>

using namespace std;

/**
 * @brief The VeniceMessage class representing a message service containg required information for service advertisement
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
     * @brief get_size returns the size in bytes of the message data
     * @return size in bytes
     */
    int get_size();

private:

    // The message identifier
    int messageId;

    // The acknolegement related to the message
    bool ack;

    // The message data
    vector<byte> data;
};

#endif // VENICEMESSAGE_H
