#pragma once

#include "util.h"

class RequestPacket {
private:
    // struct header {
    //     int user_id;
    //     Operation op_id;
    //     int data_len;
    //     bool result; // only used for response packet
    // };
    // header *hdr;

    uint8_t 
    unsigned char *data;

public:
    RequestPacket();
    RequestPacket(int user_id, Operation op_id, int data_len, bool result, unsigned char *data);
    void initPacket(int user_id, Operation op_id, int data_len, bool result, unsigned char *data);

    void printPacket();

    // getters & setters
    // void setHeader();
    void setUserId(int);
    void setOpId(Operation);
    void setDataLen(int);
    void setData(unsigned char *);

    header *getHeader();
    int getUserId();
    Operation getOpId();
    int getDataLen();
    unsigned char *getData();
};
