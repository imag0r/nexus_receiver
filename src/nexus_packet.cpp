#include "nexus_packet.h"

nexus_packet::nexus_packet() :
    bits_(0)
{
}

nexus_packet::nexus_packet(uint64_t bits) :
    bits_(bits & 0xFFFFFFFFF)   // take only 36bits
{
}

bool nexus_packet::valid() const
{
    const auto temp = temperature();
    return (humidity() <= 100) && (temp >= -200) && (temp <= 500);
}

uint8_t nexus_packet::id() const
{
    return (bits_ >> 28) & 0xFF;
}

uint8_t nexus_packet::channel() const
{
    return (bits_ >> 24) & 0x03;
}

uint8_t nexus_packet::battery() const
{
    return ((bits_ >> 24) & 0x08) ? 1 : 0;
}

int16_t nexus_packet::temperature() const
{
    return (bits_ >> 12) & 0xFFF;
}

uint8_t nexus_packet::humidity() const
{
    return bits_ & 0xFF;
}

bool nexus_packet::operator==(const nexus_packet &packet) const
{
    return bits_ == packet.bits_;
}

bool nexus_packet::operator!=(const nexus_packet &packet) const
{
    return !operator==(packet);
}

bool nexus_packet::operator<(const nexus_packet &packet) const
{
    return bits_ < packet.bits_;
}
