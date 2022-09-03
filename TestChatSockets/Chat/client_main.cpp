#include "client_main.h"

#include "Client.h"
#include "utilities.h"

#include <thread>
#include <mutex>

void ClientSide::client_main() {
    ClientSide::Client client;

    std::string ipAddress;
    int port;

    std::cout << "\n[ Ip Address ]: ";
    std::cin >> ipAddress;

    port = ChatUtilities::getPort(ChatUtilities::MIN_PORT, ChatUtilities::MAX_PORT);

    system("CLS");
    if (!client.connect(ipAddress, port)) { 
        std::cout << "\n[ ERROR ]: The server isn't reachable, "; system("PAUSE");
        return;
    };
    
    std::cout << "\n[ Chat ]: Successfully connected to the Server. Type [ !disconnect ] to exit." << std::endl;
    /* clear the buffer */
    while (std::getchar() != '\n');

    /* get the nickname */
    std::string nickName = ChatUtilities::takeNickName(ChatUtilities::MIN_NICKNAME_LEN, ChatUtilities::MAX_NICKANAME_LEN);
    
    /* get the nickname of the server from it and send the client nickaname to the server */
    if (client.read_until("\n") && client.send(nickName + "\n")) {
        std::string nick = client.getMessage();
        ChatUtilities::rmvEndl(nick);
        ChatUtilities::serverNickName = nick;
    }
    else {
        std::cout << "\n[ ERROR ]: " << client.getErr().message() << std::endl;

        /* disconnect the client */
        client.send(ChatUtilities::clientNickName + "\n");
        client.shutdown(); return;
    }
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
    std::mutex mutex;

    while (client.isOpen()) {
        mutex.lock();
        std::getline(std::cin, message);
        mutex.unlock();
        if (!message.length()) { continue; }

        client.send(message + "\n");

        if (message == ChatUtilities::disconnectMsg) {
            client.setOpen(false);
            std::cout << "\nWaiting for the other side, to close the connection..." << std::endl;
            return;
        }
    }
}

void ClientSide::read_operation(ClientSide::Client& client) {
    std::string message;

    while (client.isOpen()) {
        if (client.read_until("\n")) {
            message = client.getMessage();

            /* if the server wants to disconnect */
            if (message == ChatUtilities::disconnectMsg + "\n") {
                client.setOpen(false);
                std::cout << "\n[ " << ChatUtilities::serverNickName << " ]: left the chat, Press any key to continue..." << std::endl;
                return;
            }
            else {
                std::cout << "\n[ " << ChatUtilities::serverNickName << " ]: " << message;
            }
        }
        else {
            std::cout << "\n[ ERROR ]: " << client.getErr().message() << std::endl;
            return;
        }
    }
}
