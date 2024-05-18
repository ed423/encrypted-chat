#include <iostream>
#include <iomanip>
#include <cstring>

using namespace std;

#include "packet_protocol.h"

/**
 * Default Constructor
*/
PacketProtocol::PacketProtocol() {
    initPacket(-1, -1, 0, 0, nullptr);
}

/**
 * Constructor. Creates an instance of PacketProtocol.
 * @param user_id Sender's user ID.
 * @param op_id Operation ID.
 * @param result Result code in the response packet.
 * @param data_len Length of the attached data
 * @param data The attached data.
*/
PacketProtocol::PacketProtocol(uint32_t user_id, uint8_t op_id, uint8_t result, uint16_t data_len, uint8_t *data) {
    initPacket(user_id, op_id, data_len, result, data);
}

/**
 * Sets the fields of PacketProtocol.
 * @param user_id Sender's user ID.
 * @param op_id Operation ID.
 * @param result Result code in the response packet.
 * @param data_len Length of the attached data
 * @param data The attached data.
*/
void PacketProtocol::initPacket(uint32_t user_id, uint8_t op_id, uint8_t result, uint16_t data_len, uint8_t *data) {
    setUserId(user_id);
    setOpId(op_id);
    setResult(result);
    setDataLen(data_len);
    setData(data_len, data);
}

/**
 * used for parsing received packets; we use this instead of the setters since here we don't want to change the contents of the packet array, we only extract the fields from it.
 * we have already read in the buffer at this point and stored it in pkt_protocol->packet using read(pkt_protocol->packet, num_bytes) and then we call parsePacket() to extract the fields.
*/
void PacketProtocol::parsePacket() {
    // uint32_t user_id; // 27 bits
    // uint8_t op_id; // 5 bits
    // uint8_t result; // 8 bits; only used for response
    // uint16_t data_len; // use the full 16 bits to make the data offset easier. we send over the header size + data_len
    // uint8_t *data;
    user_id = 0x00000000 & ((uint32_t)packet[3] >> 5) & ((uint32_t)packet[2] << (8 - 5)) & ((uint32_t)packet[1] << (8 - 5 + 8)) & ((uint32_t)packet[0] << (8 - 5 + 16));
    op_id = packet[3] & 0b00011111;
    result = packet[4];
    // data_len = 0x0000 & ((uint16_t)packet[6] >> 4) & ((uint16_t)packet[5] << 4);
    data_len = 0x0000 & (uint16_t)packet[6] & ((uint16_t)packet[5] << 8);
    data = &packet[7];
}

// void PacketProtocol::dumpPacket() {
//     cout << "//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
//     cout << "   PacketProtocol:\n"
//     << "user id: " << this->getUserId() << "\n" << "operation: " << getOperationName(this->getOpId())
//     << "\n" << "data length: " << this->getDataLen() << "\n" << endl;
//     // TODO: print data in a readable format (need a helper)
//     cout << "//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
// }

//-------------------
// setters
//-------------------
void PacketProtocol::setUserId(uint32_t user_id) {
    this->user_id = user_id;
    // TODO: set field inside packet
}

void PacketProtocol::setOpId(uint8_t op_id) {
    this->op_id = op_id;
    // TODO: set field inside packet
}

void PacketProtocol::setResult(uint8_t result) {
    this->result = result;
    // TODO: set field inside packet
}

void PacketProtocol::setDataLen(uint16_t data_len) {
    this->data_len = data_len;
    // TODO: set field inside packet
}

void PacketProtocol::setData(uint16_t data_len, unsigned char *data) {
    this->data = data;
    this->data_len = data_len;
    if (data != nullptr) memcpy(this->data, data, data_len); // this is just from 317 assignment, dk if need :/
    // TODO: set field inside packet
}

//-------------------
// getters
//-------------------
uint32_t PacketProtocol::getUserId() {
    return user_id;
}

uint8_t PacketProtocol::getOpId() {
    return op_id;
}

uint8_t PacketProtocol::getResult() {
    return result;
}

uint16_t PacketProtocol::getDataLen() {
    return data_len;
}

uint8_t *PacketProtocol::getData() {
    return data;
}
