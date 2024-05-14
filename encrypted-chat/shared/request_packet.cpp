#include <iostream>
#include <iomanip>
#include <cstring>

using namespace std;

#include "request_packet.h"

RequestPacket::RequestPacket() {

}

RequestPacket::RequestPacket(int user_id, Operation op_id, int data_len, bool result, unsigned char *data) {
    initPacket(user_id, op_id, data_len, result, data);
}

void RequestPacket::initPacket(int user_id, Operation op_id, int data_len, bool result, unsigned char *data) {
    this->hdr = new header;
    this->hdr->user_id = user_id;
    this->hdr->op_id = op_id;
    this->hdr->data_len = data_len;
    this->hdr->result = result;
    // this->data = data;
    if (data != nullptr) memcpy(this->data, data, data_len);
}

void RequestPacket::printPacket() {
    cout << "//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "   Packet:\n"
    << "user id: " << this->getUserId() << "\n" << "operation: " << getOperationName(this->getOpId())
    << "\n" << "data length: " << this->getDataLen() << "\n" << endl;
    // TODO: print data in a readable format (need a helper)
    cout << "//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
}

//------------------------------------------------------------------------------
// getters & setters
//------------------------------------------------------------------------------
void RequestPacket::setUserId(int user_id) {
    hdr->user_id = user_id;
}

void RequestPacket::setOpId(Operation op_id) {
    hdr->op_id = op_id;
}

void RequestPacket::setDataLen(int data_len) {
    hdr->data_len = data_len;
}

void RequestPacket::setData(unsigned char *data) {
    this->data = data;
}

header * RequestPacket::getHeader() {
    return hdr;
}

unsigned char * RequestPacket::getData() {
    return data;
}

int RequestPacket::getUserId() {
    return hdr->user_id;
}

Operation RequestPacket::getOpId() {
    return hdr->op_id;
}

int RequestPacket::getDataLen() {
    return hdr->data_len;
}
