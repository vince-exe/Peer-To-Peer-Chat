#include "client_main.h"

#include "Client.h"
#include "utilities.h"

void ClientSide::client_main() {
    ClientSide::Client client;

    if (!client.connect("127.0.0.1", 8000)) { return; };
    
    std::cout << "\nSuccessfully connected to the Server" << std::endl;
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

    while (client.isOpen()) {
        std::cout << "\n> ";
        std::getline(std::cin, message);

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
