#ifndef VENICETIMER_H
#define VENICETIMER_H

#include <QTimer>
#include "include/network/venicemessage.h"

class VeniceTimer : public QTimer
{
public:
    /**
     * @brief VeniceTimer Create a time with the related message
     * @param message The related VeniceMessage
     */
    VeniceTimer(VeniceMessage* message);

    /**
     * @brief getMessage Return the VeniceMessage related to the timer
     * @return The VeniceMessage related to the timer
     */
    VeniceMessage* getMessage();

private:
    VeniceMessage* message;
};

#endif // VENICETIMER_H
