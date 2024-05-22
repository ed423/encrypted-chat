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
 * @param data_len Length of the attached data.
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
 * @param data_len Length of the attached data.
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
 * user_id = 27 bits, op_id = 5 bits, result = 8 bits, data_len = 16 bits
*/
void PacketProtocol::parsePacket() {
    // TODO: merge with setPacket()?
    this->user_id = 0x00000000 | ((uint32_t)packet[3] >> 5) | ((uint32_t)packet[2] << (8 - 5)) | ((uint32_t)packet[1] << (8 - 5 + 8)) | ((uint32_t)packet[0] << (8 - 5 + 16));
    this->op_id = packet[3] & 0b00011111;
    this->result = packet[4];
    this->data_len = 0x0000 | (uint16_t)packet[6] | ((uint16_t)packet[5] << 8);
    this->data = &packet[7];
}

void PacketProtocol::dumpPacket() {
    cout << "\n//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "   PacketProtocol:\n"
    << "      User ID: " << this->getUserId() << "\n"
    << "      Operation Code: " << static_cast<Operation>(this->getOpId()) << "\n"
    << "      Result Code: " << static_cast<ResultCodes>(this->getResult()) << "\n"
    << "      Data Length: " << this->getDataLen() << "\n"
    << "      Data: " << this->getData() << endl;
    // TODO: print data in a readable format (need a helper)
    cout << "//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n" << endl;
}

/**
 * parses a message given a pointer to its first character, for a given number of bytes
 * @param data_len ...
 * @param data ...
 * @return the message as a string
*/
string PacketProtocol::parseData(uint16_t data_len, unsigned char *data) {
    string parsedString(reinterpret_cast<char*>(data), data_len);

    return parsedString;
}

/**
 * set the packet array in the PacketProtocol object when one is provided
*/
void PacketProtocol::setPacket(unsigned char *data) {
    memcpy(packet, data, MAX_PACKET_SIZE);
}

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
    packet[3] = static_cast<char>((fourth_part_user_id << 5) | (packet[3] & 0b00011111));
}

void PacketProtocol::setOpId(uint8_t op_id) {
    this->op_id = op_id;
    // TODO: set field inside packet
    cout << "packet_protocol.cpp::PacketProtocol::setOpId(): Setting op_id to: " << static_cast<int>(op_id) << endl;

    packet[3] = static_cast<char>((packet[3] & 0b11100000) | op_id);
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

    // this also sets this->data_len = data_len
    this->setDataLen(data_len);

    // TODO: set field inside packet

    // we only set data in packet when it is not null, to prevent dereferencing a nullptr
    if (data != nullptr) {
        // data_len + 1 to include the null terminator
        memcpy(&packet[7], data, data_len + 1);
    }
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
