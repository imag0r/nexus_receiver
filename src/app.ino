
#include "nexus_receiver.h"

#define RF_RECEIVER_PIN 2

void setup()
{
    nexus_receiver::instance().setup();
}

void loop()
{
    nexus_receiver::instance().loop();
}
