#include "Client.h"

bool Client::connect(const char* ip, uint16_t port) {
	/* allocate the socket */
	this->socket = std::make_shared<boost::asio::ip::tcp::socket>(this->io_context);

	try {
		this->socket->connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), port));
	}
	/* catch connection exceptions */
	catch (boost::system::system_error const& e) {
		std::cerr << e.what() << std::endl;
		return false;
	}

	return true;
}
