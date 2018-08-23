#pragma once

#include "nexus_packet.h"

class nexus_receiver
{
public:
    static nexus_receiver& instance();

    void setup(uint8_t receiver_pin = 2);

    void loop();

    const nexus_packet& packet() const;

private:
    unsigned long last_time_;
    uint8_t current_bit_;
    uint64_t packet_bits_;
    nexus_packet packet_;
    uint64_t history_[10];
    uint8_t current_history_;

    nexus_receiver();

    static void handle_interrupt_static();

    void handle_interrupt();

    void clear_history();

    bool has_historical_count(uint64_t bits, uint8_t min_count) const;

    void record_historical(uint64_t bits);
};