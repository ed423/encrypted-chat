#include <iostream>
#include "../shared/packet_protocol.h"

// bad, remove later
#include "../shared/packet_protocol.cpp"

using namespace std;

int main() {
    PacketProtocol mock_packet_protocol{123456, 12, 123, 12345, nullptr};

    mock_packet_protocol.parsePacket();

    return 0;
}