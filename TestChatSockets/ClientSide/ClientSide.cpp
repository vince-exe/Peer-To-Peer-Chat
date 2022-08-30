#include <iostream>
#include <boost/asio.hpp>

int main() {
    boost::asio::io_service io_service;
    /* socket creation */
    boost::asio::ip::tcp::socket socket(io_service);
    /* connection */
    socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8000));

    /* request message from client */
    const std::string msg = "Hello From Client \n";

    boost::system::error_code er;
    boost::asio::write(socket, boost::asio::buffer(msg), er);

    if (er) {
        std::cout << "\n[ FATAL ERROR ]: " << er.message() << std::endl;
    }

    /* getting response from server */
    boost::asio::streambuf recvBuffer;
    boost::asio::read(socket, recvBuffer, boost::asio::transfer_all(), er);

    if (er && er != boost::asio::error::eof) {
        std::cout << "\n [ FATAL ERROR ]: " << er.message() << std::endl;
    }
    else {
        const char* data = boost::asio::buffer_cast<const char*>(recvBuffer.data());
        std::cout << "\nMessage From Server: " << data << std::endl;
    }

    return 0;
}