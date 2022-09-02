#include "client_main.h"

#include "Client.h"

void Chat::client_main() {
    Chat::Client client;

    if (!client.connect("127.0.0.1", 8000)) { return; };

    /* check if the client successfully sent the message */
    if (!client.send("Hello From Client Vincenzo Caliendo \n")) {
        std::cerr << client.getErr().message() << std::endl;
    }

    /* check if the client successfully read the message */
    if (!client.read_until("\n")) {
        std::cerr << client.getErr().message() << std::endl;
    }

    std::cout << "\nMessage From Server: " << client.getMessage() << std::endl;
    std::cout << "\nReaden Bytes: " << client.getReadBytes() << std::endl;

    client.shutdown();
}