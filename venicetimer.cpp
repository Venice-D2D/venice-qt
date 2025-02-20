#include "venicetimer.h"

VeniceTimer::VeniceTimer(VeniceMessage* message):QTimer()
{
    this->message = message;
}

VeniceMessage* VeniceTimer::getMessage(){
    return this->message;
}
