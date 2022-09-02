#include "server_main.h"

#include "server.h"

void ServerSide::server_main() {
    /* create the server */
    ServerSide::Server server(8000);

    /* listen for new connections */
    server.listen();

    /* start the threads */
    std::thread sendThread(ServerSide::send_operation, std::ref(server));
    std::thread readThread(ServerSide::read_operation, std::ref(server));

    while (server.isOpen()) { continue; }
    
    sendThread.join();
    readThread.join();

    /* shutdown the server */
    server.shutdown();
}

void ServerSide::read_operation(ServerSide::Server& server) {
    std::string message;

    while (server.isOpen()) {
        if (server.read_until("\n")) {
            message = server.getMessage();

            if (message == "!disconnect\n") {
                server.setOpen(false);
                return;
            }
            else {
                std::cout << "\n" << message << std::endl;
            }
        }
    }
}

void ServerSide::send_operation(ServerSide::Server& server) {
    std::string message;

    while (server.isOpen()) {
        std::getline(std::cin, message);
        
        if (!message.length()) { continue; }

        server.send(message + "\n");

        if (message == "!disconnect") {
            server.setOpen(false);
            return;
        }
    }
}
