#pragma once
#include <set>

// using namespace std;

#define SERVER_PORT 12345

class Server {

public:
    Server();
    ~Server();
    void initServer();

private:
    // Set of known users IDs
    std::set<int> users;
};