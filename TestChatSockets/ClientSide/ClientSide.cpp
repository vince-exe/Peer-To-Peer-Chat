#include <iostream>
#include <boost/asio.hpp>

#include "Client.h"

int main() {
    Client client;
    boost::asio::io_context context;
    boost::asio::ip::tcp::socket socket(context);

    if (!client.connect(socket, "127.0.0.1", 8000)) { return 1; };

    client.send(socket, "Hello From Client Vincenzo Caliendo \n", Client::er);
    if (Client::er) {
        std::cerr << "Can't send message" << std::endl;
        return 1;
    }

    std::string msg = client.read(socket, Client::er);
    if (Client::er) {
        std::cerr << "Can't read message" << std::endl;
        return 1;
    }
    
    std::cout << "\nMessage From Server: " << msg << std::endl;

    return 0;
}