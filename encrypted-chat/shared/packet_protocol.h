#pragma once

#include "util.h"

using namespace std;

#define MAX_PACKET_SIZE 1480

/**
 * Defines the structure of the network packet. Every time we send a PacketProtocol, 
 * we also get a reponse PacketProtocol back, check its result field to see if operation 
 * was successful.
*/
class PacketProtocol {
private:
    uint32_t user_id; // 27 bits
    uint8_t op_id; // 5 bits
    uint8_t result; // 8 bits; only used for response
    uint16_t data_len; // 16 bits; we send over the header size + data_len
    uint8_t *data;

    uint8_t packet[MAX_PACKET_SIZE]; // this is what we send over
    int field_offsets[5] = { 0, 27, 32, 40, 56 };

    void initPacket(uint32_t user_id, uint8_t op_id, uint8_t result, uint16_t data_len, uint8_t *data);
    void setPktField(uint32_t val, int numBits, int actualOffset);
    void parsePacket();

public:

    PacketProtocol();
    PacketProtocol(uint32_t user_id, uint8_t op_id, uint8_t result, uint16_t data_len, uint8_t *data);

    void dumpPacket();
    void printFields();
    
    //----------------------
    // getters & setters
    //----------------------
    void setUserId(uint32_t);
    void setOpId(uint8_t);
    void setResult(uint8_t);
    void setDataLen(uint16_t);
    void setData(uint16_t, unsigned char *);
    void setPacket(unsigned char *data);
    
    uint32_t getUserId();
    uint8_t getOpId();
    uint8_t getResult();
    uint16_t getDataLen();
    char *getData();
    uint8_t *getPkt();
};
