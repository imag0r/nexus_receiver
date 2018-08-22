#pragma once

#include <stdint.h>

class nexus_packet
{
public:
    nexus_packet();

    nexus_packet(uint64_t bits);

    bool valid() const;

    uint8_t id() const;

    uint8_t channel() const;

    uint8_t battery() const;

    int16_t temperature() const;

    uint8_t humidity() const;

private:
    uint64_t bits_;
};