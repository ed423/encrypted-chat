#pragma once

#include <map>
#include <string>

#define SERVER_PORT 12345

//------------------------------------------------------------------------------
// packets/requests/responses
//------------------------------------------------------------------------------
enum Operation {
    OPERATION_SEND_MSG,
    OPERATION_UPDATE_CHAT,
    // OPERATION_CREATE_SESSION,
    // OPERATION_JOIN_SESSION,
};

std::string getOperationName(Operation op);
// example use: Operation op = OPERATION_SEND_MSG; if (op == OPERATION_SEND_MSG) {...}

enum ResultCodes { // for response packets (use fail, success only for now)
    FAIL,
    SUCCESS,
};
