#pragma once

#include "util.h"
// every time we send a PacketProtocol, we also get a reponse PacketProtocol back, check its result field to see if operation was successful

using namespace std;

#define MAX_PACKET_SIZE 1480 // ??????????? MTU BYTE SIZE?????

class PacketProtocol {
private:
    uint32_t user_id; // 27 bits
    uint8_t op_id; // 5 bits
    uint8_t result; // 8 bits; only used for response
    uint16_t data_len; // use the full 16 bits to make the data offset easier. we send over the header size + data_len
    uint8_t *data;

    int field_offsets[5] = { 0, 26, 31, 39, 55 };

    void initPacket(uint32_t user_id, uint8_t op_id, uint8_t result, uint16_t data_len, uint8_t *data);

public:
    uint8_t packet[MAX_PACKET_SIZE]; // this is what we send over

    PacketProtocol();
    PacketProtocol(uint32_t user_id, uint8_t op_id, uint8_t result, uint16_t data_len, uint8_t *data);

    void parsePacket();
    void setPacket(unsigned char *data);
    string parseData(uint16_t data_len, unsigned char *data);
    void dumpPacket(); // helper for testing
    
    //----------------------
    // getters & setters
    //    * when we set a field, also set it inside the packet
    //----------------------
    void setUserId(uint32_t);
    void setOpId(uint8_t);
    void setResult(uint8_t);
    void setDataLen(uint16_t);
    void setData(uint16_t, unsigned char *);
    void setPktField(uint32_t val, int numBits, int actualOffset);
    
    uint32_t getUserId();
    uint8_t getOpId();
    uint8_t getResult();
    uint16_t getDataLen();
    uint8_t *getData();
};
