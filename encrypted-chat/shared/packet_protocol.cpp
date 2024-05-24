#include <iostream>
#include <iomanip>
#include <cstring>

#include "packet_protocol.h"

using namespace std;

/**
 * Default Constructor
*/
PacketProtocol::PacketProtocol() {
    // initPacket(-1, -1, 0, 0, nullptr);
    initPacket(0, 0, 0, 0, nullptr);
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
 * Parses values from a Packet array and sets member fields accordingly.
*/
void PacketProtocol::parsePacket() {
    // TODO: merge with setPacket()?
    this->user_id = 0x00000000 | ((uint32_t)packet[3] >> 5) | ((uint32_t)packet[2] << (8 - 5)) | ((uint32_t)packet[1] << (8 - 5 + 8)) | ((uint32_t)packet[0] << (8 - 5 + 16));
    this->op_id = packet[3] & 0b00011111;
    this->result = packet[4];
    this->data_len = 0x0000 | (uint16_t)packet[6] | ((uint16_t)packet[5] << 8);
    this->data = &packet[7];
}

/**
 * Helper function to print packet values in the console.
*/
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
 * Parses a message given a pointer to its first character, for a given number of bytes
 * @param data_len Length of data to read.
 * @param data The attached data.
 * @return the message as a string
*/
string PacketProtocol::parseData(uint16_t data_len, unsigned char *data) {
    string parsedString(reinterpret_cast<char*>(data), data_len);

    return parsedString;
}

/**
 * Sets the Packet member field given a Packet array.
 * @param data The attached data.
*/
void PacketProtocol::setPacket(unsigned char *data) {
    memcpy(packet, data, MAX_PACKET_SIZE);
}

/**
 * Sets a field in a packet array
 * @param data_len Length of data to read.
 * @param data The attached data.
 * @return the message as a string
*/
void PacketProtocol::setPktField(uint32_t val, int numBits, int actualOffset) {
    // First index in packet array to write to
    int block = actualOffset / 8;
    // Index in first block that we want to write to
    int start = actualOffset % 8;
    int totalBlocks = totalBlocks = (numBits / 8) + (numBits % 8 != 0);
    int lastBlock = block + totalBlocks - 1;

    // Case 1: We start writing to the first block, but not from the beginning of the block
    if (start != 0) {
        uint32_t temp = val;
        temp = val >> (numBits - (8 - start));
        packet[block] = packet[block] | temp;
        totalBlocks--;
    }

    // Case 2: We start writing from the beginning of the block
    // If we don't completely write to the last block, we shift val to the left so that its last 8 bits line up with the last block
    if ((numBits % 8) != 0) {
        val <<= (8 - (numBits % 8));
    }

    uint32_t bitMask = 0x000000FF;
    int blocksWritten = 0;

    // Write each block from highest index to lowest index in the packet array
    for (int i = totalBlocks; i > 0; i--) {
        int bitsWritten = 8 * blocksWritten;
        uint32_t valToWrite = ((val & (bitMask << (bitsWritten))) >> (bitsWritten));
        packet[lastBlock] = valToWrite;
        lastBlock--;
        blocksWritten++;
    }
}

//-------------------
// setters
//-------------------
void PacketProtocol::setUserId(uint32_t user_id) {
    this->user_id = user_id;
    // TODO: set field inside packet
    cout << "packet_protocol.cpp::PacketProtocol::setUserId(): Setting user_id to: " << user_id << endl;

    setPktField(user_id, 27, field_offsets[0]);
}

void PacketProtocol::setOpId(uint8_t op_id) {
    this->op_id = op_id;
    // TODO: set field inside packet
    cout << "packet_protocol.cpp::PacketProtocol::setOpId(): Setting op_id to: " << static_cast<int>(op_id) << endl;

    setPktField(op_id, 5, field_offsets[1]);
}

void PacketProtocol::setResult(uint8_t result) {
    this->result = result;
    // TODO: set field inside packet
    cout << "packet_protocol.cpp::PacketProtocol::setResult(): Setting result to: " << static_cast<int>(result) << endl;

    setPktField(result, 8, field_offsets[2]);
}

void PacketProtocol::setDataLen(uint16_t data_len) {
    this->data_len = data_len;
    // TODO: set field inside packet
    cout << "packet_protocol.cpp::PacketProtocol::setDataLen(): Setting data_len to: " << static_cast<int>(data_len) << endl;

    setPktField(data_len, 16, field_offsets[3]);
}

void PacketProtocol::setData(uint16_t data_len, unsigned char *data) {
    this->data = data;

    // this also sets this->data_len = data_len
    this->setDataLen(data_len);

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
