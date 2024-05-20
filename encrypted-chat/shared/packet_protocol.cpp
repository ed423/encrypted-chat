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
    initPacket(user_id, op_id, result, data_len, data);
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

    // Sally
    // user_id = 0x00000000 & ((uint32_t)packet[3] >> 5) & ((uint32_t)packet[2] << (8 - 5)) & ((uint32_t)packet[1] << (8 - 5 + 8)) & ((uint32_t)packet[0] << (8 - 5 + 16));
    // op_id = packet[3] & 0b00011111;
    // result = packet[4];
    // // data_len = 0x0000 & ((uint16_t)packet[6] >> 4) & ((uint16_t)packet[5] << 4);
    // data_len = 0x0000 & (uint16_t)packet[6] & ((uint16_t)packet[5] << 8);
    // data = &packet[7];

    // Ed
    user_id = 0x00000000 | ((uint32_t)packet[3] >> 5) | ((uint32_t)packet[2] << (8 - 5)) | ((uint32_t)packet[1] << (8 - 5 + 8)) | ((uint32_t)packet[0] << (8 - 5 + 16));
    op_id = packet[3] & 0b00011111;
    result = packet[4];
    data_len = 0x0000 | (uint16_t)packet[6] | ((uint16_t)packet[5] << 8);
    data = &packet[7];

    cout << "packet_protocol.cpp::PacketProtocol::parsePacket(): Values parsed from packet: " << endl;
    cout << "user_id: " << static_cast<int>(user_id) << endl;
    cout << "op_id: " << static_cast<int>(op_id) << endl;
    cout << "result: " << static_cast<int>(result) << endl;
    cout << "data_len: " << static_cast<int>(data_len) << endl;
    cout << "data: " << *data << endl;

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

    cout << "packet_protocol.cpp::PacketProtocol::setUserId(): Setting user_id to: " << user_id << endl;

    uint8_t first_part_user_id = (user_id >> (3 + 8 + 8)) & 0xFF;
    uint8_t second_part_user_id = (user_id >> (3 + 8)) & 0xFF;
    uint8_t third_part_user_id = (user_id >> 3) & 0xFF;
    uint8_t fourth_part_user_id = (user_id & 0b111) & 0xFF;

    packet[0] = static_cast<char>(first_part_user_id);
    packet[1] = static_cast<char>(second_part_user_id);
    packet[2] = static_cast<char>(third_part_user_id);
    packet[3] = static_cast<char>((fourth_part_user_id << 5) | packet[3]);
}

void PacketProtocol::setOpId(uint8_t op_id) {
    this->op_id = op_id;
    // TODO: set field inside packet
    cout << "packet_protocol.cpp::PacketProtocol::setOpId(): Setting op_id to: " << static_cast<int>(op_id) << endl;

    packet[3] = static_cast<char>((packet[3] & 0b11111000) | op_id);
}

void PacketProtocol::setResult(uint8_t result) {
    this->result = result;
    // TODO: set field inside packet
    cout << "packet_protocol.cpp::PacketProtocol::setResult(): Setting result to: " << static_cast<int>(result) << endl;

    packet[4] = static_cast<char>(result & 0xFF);
}

void PacketProtocol::setDataLen(uint16_t data_len) {
    this->data_len = data_len;
    // TODO: set field inside packet
    cout << "packet_protocol.cpp::PacketProtocol::setDataLen(): Setting data_len to: " << static_cast<int>(data_len) << endl;

    uint8_t first_part_data = (data_len >> 8) & 0xFF;
    uint8_t second_part_data = data_len & 0xFF;

    // explicit type conversion just in case, since packet is a char array
    packet[5] = static_cast<char>(first_part_data);
    packet[6] = static_cast<char>(second_part_data);
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
