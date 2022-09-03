#include "Client.h"

bool ClientSide::Client::connect(const char* ip, uint16_t port) {
	/* allocate the socket */
	this->socket = std::make_shared<boost::asio::ip::tcp::socket>(this->io_context);

	try {
		this->socket->connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), port));
	}
	/* catch connection exceptions */
	catch (boost::system::system_error const& e) {
		return false;
	}

	this->is_open = true;
	return true;
}

void ClientSide::Client::shutdown() {
	this->socket->close();
}

void ClientSide::Client::setOpen(bool open) {
	this->is_open = open;
}

bool ClientSide::Client::isOpen() const {
	return this->is_open;
}
