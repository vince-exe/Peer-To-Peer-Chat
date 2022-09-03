#include "server_main.h"

#include "server.h"
#include "utilities.h"

void ServerSide::server_main() {

    int port = ChatUtilities::getPort(ChatUtilities::MIN_PORT, ChatUtilities::MAX_PORT);

    /* create the server */
    ServerSide::Server server(port);

    system("CLS");
    std::cout << "\n[ Chat ]: Start listening for new connections..." << std::endl;
    /* listen for new connections */
    server.listen();

    system("CLS");
    std::cout << "\nA client has successfully joined the chat. Type [ !disconnect ] to exit." << std::endl;

    /* clear the buffer */
    while (std::getchar() != '\n');

    /* send the nickname to the client and receive the nickname from it */
    if (server.send(ChatUtilities::takeNickName(ChatUtilities::MIN_NICKNAME_LEN, ChatUtilities::MAX_NICKANAME_LEN) + "\n") && server.read_until("\n")) {
        std::string nick = server.getMessage();
        ChatUtilities::rmvEndl(nick);
        ChatUtilities::clientNickName = nick;
    }
    else {
        std::cout << "\n[ ERROR ]: " << server.getErr().message() << std::endl;

        /* disconnect the server */
        server.send(ChatUtilities::disconnectMsg + "\n");
        server.shutdown(); return;
    }
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

            /* if the client wants to disconnect */
            if (message == ChatUtilities::disconnectMsg + "\n") {
                server.setOpen(false);
                std::cout << "\n[ " << ChatUtilities::clientNickName << " ]: left the chat, Press any key to continue..." << std::endl;
                return;
            }
            else {
                std::cout << "\n[ " << ChatUtilities::clientNickName << " ]: " << message;
            }
        }
        else {
            std::cout << "\n[ ERROR ]: " << server.getErr().message() << std::endl;
            return;
        }
    }
}

void ServerSide::send_operation(ServerSide::Server& server) {
    std::string message;
    std::mutex mutex;

    while (server.isOpen()) {
        mutex.lock();
        std::getline(std::cin, message);
        mutex.unlock();
        if (!message.length()) { continue; }

        server.send(message + "\n");

        if (message == ChatUtilities::disconnectMsg) {
            server.setOpen(false);
            std::cout << "\nWaiting for the other side, to close the connection..." << std::endl;
            return;
        }
    }
}
