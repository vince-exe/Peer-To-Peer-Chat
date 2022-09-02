#include "client_main.h"

#include "Client.h"

void ClientSide::client_main() {
    ClientSide::Client client;

    if (!client.connect("127.0.0.1", 8000)) { return; };
    
    /* start the threads */
    std::thread readThread(ClientSide::read_operation, std::ref(client));
    std::thread writeThread(ClientSide::send_operation, std::ref(client));

    while (client.isOpen()) { continue; }

    readThread.join();
    writeThread.join();

    /* shutdown the client */
    client.shutdown();
}

void ClientSide::send_operation(ClientSide::Client& client) {
    std::string message;

    while (client.isOpen()) {
        std::getline(std::cin, message);

        if (!message.length()) { continue; }

        client.send(message + "\n");

        if (message == "!disconnect") {
            client.setOpen(false);
            return;
        }
    }
}

void ClientSide::read_operation(ClientSide::Client& client) {
    std::string message;

    while (client.isOpen()) {
        if (client.read_until("\n")) {
            message = client.getMessage();

            if (message == "!disconnect\n") {
                client.setOpen(false);
                return;
            }
            else {
                std::cout << "\n" << message << std::endl;
            }
        }
    }
}
