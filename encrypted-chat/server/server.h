#pragma once
#include <set>

using namespace std;

#define SERVER_PORT 2000

class Server {

public:
    Server();
    ~Server();
    void initServer();

private:
    // Set of known users IDs
    set<int> users;
};