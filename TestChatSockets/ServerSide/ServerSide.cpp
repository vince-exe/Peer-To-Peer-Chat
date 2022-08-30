#include <iostream>
#include <boost/asio.hpp>

std::string read(boost::asio::ip::tcp::socket& socket) {
    boost::asio::streambuf buffer;
    boost::asio::read_until(socket, buffer, "\n");
    std::string data = boost::asio::buffer_cast<const char*>(buffer.data());
    return data;
}

void send(boost::asio::ip::tcp::socket& socket, const std::string& message) {
    const std::string msg = message + "\n";
    boost::asio::write(socket, boost::asio::buffer(msg));
}

int main() {
    boost::asio::io_service service;

    /* listen for new connections */
    boost::asio::ip::tcp::acceptor acceptor_(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8000));
    /* create a new socket */
    boost::asio::ip::tcp::socket socket(service);
    /* wait for connection */
    acceptor_.accept(socket);

    std::string clientMsg = read(socket);
    /* read client message */
    std::cout << "\n Message From Client: " << clientMsg << std::endl;

    /* write client message */
    send(socket, "Hello From Server");

    return 0;
}