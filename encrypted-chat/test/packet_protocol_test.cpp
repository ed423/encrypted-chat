#include <iostream>
#include "../shared/packet_protocol.h"

// bad, remove later
#include "../shared/packet_protocol.cpp"

using namespace std;

void testBasicSendAndReceive();
void testSetPktField();

int main() {
    cout << "packet_protocol_test.cpp::main(): BEGINNING TEST SUITE" << endl;
    // comment out what you're not currently testing - easier to test :)
    testBasicSendAndReceive();
    testSetPktField();
    return 0;
}

void testSetPktField() {
    cout << "\n//================================================================" << endl;
    cout << "testSetPktField():\n" << endl;

    // Create a PacketProtocol object to simulate one that is created by a sender (client)
    PacketProtocol pkt{};

    uint32_t user_id = 5; // 0b101
    Operation op_id = OPERATION_UPDATE_CHAT;
    ResultCodes res = SUCCESS;
    uint8_t msg[] = "hello world this is a string to test that the message functionality works";
    unsigned long msg_len = sizeof(msg)/sizeof(char);

    pkt.setUserId(user_id);
    cout << "1\n" << endl;
    pkt.dumpPacket();
    pkt.printFields();

    pkt.setOpId(op_id);
    cout << "2\n" << endl;
    pkt.dumpPacket();
    pkt.printFields();

    pkt.setResult(res);
    cout << "3\n" << endl;
    pkt.dumpPacket();
    pkt.printFields();

    pkt.setData(msg_len, msg);
    cout << "4\n" << endl;
    cout << "data len: " << msg_len << "\n" << endl;
    pkt.dumpPacket();
    pkt.printFields();

    cout << "\nEND testSetPktField()" << endl;
    cout << "//================================================================" << endl;
}

void testBasicSendAndReceive() {
    cout << "\n//================================================================" << endl;
    cout << "testSendAndReceive(): TEST CASE 1: Simulating a packet being sent from the client to the server, without any socket operations\n" << endl;

    // Create a PacketProtocol object to simulate one that is created by a sender (client)
    PacketProtocol mock_send_packet_protocol{};

    uint32_t mock_user_id = 1234567;
    // Operation mock_op_id = OPERATION_SEND_MSG; // should test different values for each field
    Operation mock_op_id = OPERATION_UPDATE_CHAT;
    ResultCodes mock_result_code = SUCCESS;
    uint8_t mock_string[] = "hello world this is a string to test that the message functionality works";
    unsigned long mock_string_length = sizeof(mock_string)/sizeof(char);

    // cout << "\nManually setting packet values: " << endl;
    // cout << "User ID: " << mock_user_id << endl;
    // cout << "Operation ID: " << static_cast<int>(mock_op_id) << endl;
    // cout << "Result Code: " << static_cast<int>(mock_result_code) << endl;
    // cout << "Data: " << mock_string << "\n" << endl;

    // Set the fields in the sender's PacketProtocol object
    mock_send_packet_protocol.setUserId(mock_user_id);
    mock_send_packet_protocol.setOpId(mock_op_id);
    mock_send_packet_protocol.setResult(mock_result_code);
    mock_send_packet_protocol.setData(mock_string_length, mock_string);

    mock_send_packet_protocol.printFields();
    mock_send_packet_protocol.dumpPacket();

    // Pointer to the sender's packet array (the buffer that we want to send to the server)
    uint8_t *packet_array_from_sender = mock_send_packet_protocol.getPkt();

    // Create a new PacketProtocol object, to simulate the creation of one when we receive a buffer (packet array)
    PacketProtocol mock_receive_packet_protocol{};

    // Set the packet array in the new object as the received buffer
    mock_receive_packet_protocol.setPacket(packet_array_from_sender);
    cout << "\nReceived packet value: " << endl;

    // Extract values from the packet array and set class fields in the receiver's PacketProtocol object
    // mock_receive_packet_protocol.parsePacket(); // moved parsePacket() to inside setPacket() so users don't have to call both methods

    // Print values stored in the receiver's PacketProtocol object fields
    mock_receive_packet_protocol.printFields();

    // Print the message received by the receiver
    // string parsedData = mock_receive_packet_protocol.parseData(mock_receive_packet_protocol.getDataLen(), mock_receive_packet_protocol.getData());
    string parsedData = mock_receive_packet_protocol.getData();

    // Check that the message in the receiver's packet array is the same as the one that was sent by the sender
    cout << "\nExpected message value: " << mock_string << endl;
    cout << "Actual message value: " << parsedData << endl;

    cout << "\nEND TEST CASE 1" << endl;
    cout << "//================================================================" << endl;
}
