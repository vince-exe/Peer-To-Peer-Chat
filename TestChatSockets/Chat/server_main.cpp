#include "server_main.h"

#include "server.h"

void Chat::server_main() {
    /* create the server */
    Chat::Server server(8000);

    /* listen for new connections */
    server.listen();

    /* read the messsage */
    if (!server.read_until("\n")) {
        std::cerr << "\n" << server.getErr().message() << std::endl;
    }
    else {
        std::cout << "\nMessage From Client: " << server.getMessage() << std::endl;
        std::cout<<"\nBytes Read: " << server.getReadBytes() << std::endl;
    }
    
    /* sent message */
    if (!server.send("Hello client!! \n")) {
        std::cerr << "\n" << server.getErr().message() << std::endl;
    }

    server.shutdown();
}
