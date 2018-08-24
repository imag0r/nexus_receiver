#include <Arduino.h>

#include "nexus_receiver.h"

namespace {
    nexus_packet prev_packet;
}

void setup()
{
    Serial.begin(115200);

    nexus_receiver::instance().setup();
}

void loop()
{
    nexus_receiver::instance().loop();

    const auto packet = nexus_receiver::instance().packet();

    if (packet != prev_packet)
    {
        Serial.print("CH: ");
        Serial.print(packet.channel());
        Serial.print("\tTEMP: ");
        Serial.print(packet.temperature());
        Serial.print("\tHUM: ");
        Serial.println(packet.humidity());
        prev_packet = packet;
    }
    delay(100);
}
