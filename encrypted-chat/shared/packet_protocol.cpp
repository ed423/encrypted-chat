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

void PacketProtocol::setPktField(uint32_t val, int numBits, int actualOffset) {
    // cout << "packet_protocol.cpp::setPktField(): before writing to offset " << actualOffset << ": " << endl;
    // cout << "packet_protocol.cpp::setPktField(): packet[0] originally contains " << static_cast<int>(packet[0]) << endl;
    // cout << "packet_protocol.cpp::setPktField(): packet[1] originally contains " << static_cast<int>(packet[1]) << endl;
    // cout << "packet_protocol.cpp::setPktField(): packet[2] originally contains " << static_cast<int>(packet[2]) << endl;
    // cout << "packet_protocol.cpp::setPktField(): packet[3] originally contains " << static_cast<int>(packet[3]) << endl;
    // cout << "packet_protocol.cpp::setPktField(): packet[4] originally contains " << static_cast<int>(packet[4]) << endl;
    // cout << "packet_protocol.cpp::setPktField(): packet[5] originally contains " << static_cast<int>(packet[5]) << endl;
    // cout << "packet_protocol.cpp::setPktField(): packet[6] originally contains " << static_cast<int>(packet[6]) << "\n\n" << endl;

    // 1. Get the index of the packet array (block) that we want to start filling at
    int block = actualOffset / 8;
    cout << "\n\npacket_protocol.cpp::setPktField(): block: " << block << endl;

    // 2. Get the index of the first bit that we want to start writing to
    int start = actualOffset % 8;
    cout << "packet_protocol.cpp::setPktField(): start: " << start << endl;

    // 3. Get the total number of blocks that we need to write to, using ceiling division
    int totalBlocks = totalBlocks = (numBits / 8) + (numBits % 8 != 0);
    cout << "packet_protocol.cpp::setPktField(): totalBlocks: " << totalBlocks << endl;

    // Rightmost block to modify
    int lastBlock = block + totalBlocks - 1;
    cout << "packet_protocol.cpp::setPktField(): lastBlock: " << lastBlock << endl;

    // 4. Handle first block
    uint32_t temp = val;

    // Only keep bits that we want to use to fill the first block, and move them all the way to the right
    // We only have to deal with this case if start isn't 0 (start is the index in the first block that we start writing to)
    if (start != 0) {
        temp = val >> (numBits - (8 - start));
        cout << "packet_protocol.cpp::setPktField(): FIRST BLOCK writing " << static_cast<int>(temp) << " to packet[" << block << "]" << endl;

        packet[block] = packet[block] | temp;
        totalBlocks--;
    }

    // 5. Handle remaining blocks
    // Bitshift to the left so that the rightmost 8 bits in val line up with the last block that we're writing to
    cout << "packet_protocol.cpp::setPktField(): numBits: " << numBits << endl;
    cout << "packet_protocol.cpp::setPktField(): numBits % 8: " << numBits % 8 << endl;
    if ((numBits % 8) != 0) {
        cout << "packet_protocol.cpp::setPktField(): shifting last block bits left by: " << (8 - (numBits % 8)) << endl;
        val = val << (8 - (numBits % 8));
    }
    cout << "packet_protocol.cpp::setPktField(): val is: " << val << endl;

    uint32_t bitMask = 0x000000FF;

    int blocksWritten = 0;

    for (int i = totalBlocks; i > 0; i--) {
        uint32_t valToWrite = ((val & bitMask) >> (8 * blocksWritten));
        cout << "packet_protocol.cpp::setPktField(): writing " << static_cast<int>(valToWrite) << " to packet[" << lastBlock << "]" << endl;

        // WHY does this work when we use valToWrite, but it doesn't work when we set it to packet[lastBlock] | valToWrite?
        packet[lastBlock] = valToWrite;
        // Move mask left by 8 to get to next block
        lastBlock--;
        bitMask = bitMask << 8;
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

    // uint8_t first_part_user_id = (user_id >> (3 + 8 + 8)) & 0xFF;
    // uint8_t second_part_user_id = (user_id >> (3 + 8)) & 0xFF;
    // uint8_t third_part_user_id = (user_id >> 3) & 0xFF;
    // uint8_t fourth_part_user_id = (user_id & 0b111) & 0xFF;


    // packet[0] = static_cast<char>(first_part_user_id);
    // packet[1] = static_cast<char>(second_part_user_id);
    // packet[2] = static_cast<char>(third_part_user_id);
    // packet[3] = static_cast<char>((fourth_part_user_id << 5) | (packet[3] & 0b00011111));

    setPktField(user_id, 27, field_offsets[0]);
}

void PacketProtocol::setOpId(uint8_t op_id) {
    this->op_id = op_id;
    // TODO: set field inside packet
    cout << "packet_protocol.cpp::PacketProtocol::setOpId(): Setting op_id to: " << static_cast<int>(op_id) << endl;

    // packet[3] = static_cast<char>((packet[3] & 0b11100000) | op_id);
    setPktField(op_id, 5, field_offsets[1]);
}

void PacketProtocol::setResult(uint8_t result) {
    this->result = result;
    // TODO: set field inside packet
    cout << "packet_protocol.cpp::PacketProtocol::setResult(): Setting result to: " << static_cast<int>(result) << endl;

    // packet[4] = static_cast<char>(result & 0xFF);
    setPktField(result, 8, field_offsets[2]);
}

void PacketProtocol::setDataLen(uint16_t data_len) {
    this->data_len = data_len;
    // TODO: set field inside packet
    cout << "packet_protocol.cpp::PacketProtocol::setDataLen(): Setting data_len to: " << static_cast<int>(data_len) << endl;

    // uint8_t first_part_data = (data_len >> 8) & 0xFF;
    // uint8_t second_part_data = data_len & 0xFF;

    // // explicit type conversion just in case, since packet is a char array
    // packet[5] = static_cast<char>(first_part_data);
    // packet[6] = static_cast<char>(second_part_data);

    setPktField(data_len, 16, field_offsets[3]);
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
