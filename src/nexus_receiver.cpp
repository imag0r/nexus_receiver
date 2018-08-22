#include "nexus_receiver.h"

#include <Arduino.h>

nexus_receiver& nexus_receiver::instance()
{
    static nexus_receiver inst;
    return inst;
}
    
nexus_receiver::nexus_receiver() : 
    last_time_(micros()),
    current_bit_(0),
    packet_bits_(0),
    packet_(),
    history_(),
    current_history_(0)
{
}

void nexus_receiver::setup()
{
    clear_history();

    Serial.begin(115200);

    const uint8_t nexus_receiver_pin = 2;
    pinMode(nexus_receiver_pin, OUTPUT);
    digitalWrite(nexus_receiver_pin, LOW);
    pinMode(nexus_receiver_pin, INPUT);
    digitalWrite(nexus_receiver_pin, LOW);

    attachInterrupt(nexus_receiver_pin - 2, &nexus_receiver::handle_interrupt_static, CHANGE);
}

void nexus_receiver::loop()
{
}

void nexus_receiver::handle_interrupt_static()
{
    nexus_receiver::instance().handle_interrupt();
}

void nexus_receiver::handle_interrupt()
{
    const auto time = micros();
    const auto duration = time - last_time_;
    last_time_ = time;
    
    const unsigned long duration_sync = 3500;
    const unsigned long duration_zero = 800;
    const unsigned long duration_one = 1800;
    const unsigned long min_packet_size = 36;
    const unsigned long max_packet_size = 37;
    const unsigned long max_bits = sizeof(packet_bits_) * 8;

    if ((duration > duration_sync) || (current_bit_ > max_bits))
    {
        if ((current_bit_ >= min_packet_size) && (current_bit_ <= max_packet_size))
        {
            record_historical(packet_bits_);
            if (has_historical_count(packet_bits_, 4))
            {
                packet_ = nexus_packet(packet_bits_);

                Serial.print("CH: ");
                Serial.print(packet_.channel());
                Serial.print("\tTEMP: ");
                Serial.print(packet_.temperature());
                Serial.print("\tHUM: ");
                Serial.println(packet_.humidity());

                clear_history();
            }
        }

        current_bit_ = 0;
    }
    else if (duration > duration_zero)
    {
        packet_bits_ <<= 1;
        if (duration > duration_one)
        {
            bitSet(packet_bits_, 0);
        }
        else
        {
            bitClear(packet_bits_, 0);
        }
        ++current_bit_;
    }
}

void nexus_receiver::clear_history()
{
    memset(history_, 0, sizeof(history_));
    current_history_ = 0;
}

bool nexus_receiver::has_historical_count(uint64_t bits, uint8_t min_count) const
{
    uint8_t count = 0;
    for (const auto& hist : history_)
    {
        if (hist == bits)
        {
            if (++count >= min_count)
            {
                return true;
            }
        }
    }
    return false;
}

void nexus_receiver::record_historical(uint64_t bits)
{
    history_[current_history_] = bits;
    if (++current_history_ >= (sizeof(history_) / sizeof(*history_)))
    {
        current_history_ = 0;
    }
}
