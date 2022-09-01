#include "Client.h"
	
bool Client::connect(const char* ip, uint16_t port) {
	/* allocate the socket */
	this->socket = std::make_shared<boost::asio::ip::tcp::socket>(this->io_context);
					
	try {
		this->socket->connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), port));
	
		this->ip = ip; this->port = port;
	}
	/* catch connection exceptions */
	catch (boost::system::system_error const& e) {
		std::cerr << e.what() << std::endl;
		return false;
	}

	return true;
}

bool Client::send(const std::string& msg) {
	boost::asio::write(*this->socket, boost::asio::buffer(msg), this->er);

	return (this->er) ? false : true;
}

bool Client::read_until(const std::string& del) {
	this->readBytes = boost::asio::read_until(*this->socket, this->readBuff, del, this->er);
	
	/* check if the readen bytes are 0 or an errro occurred */
	return (!this->readBytes || this->er) ? false : true;
}

const char* Client::getMessage() {
	const char* data = boost::asio::buffer_cast<const char*>(this->readBuff.data());

	/* clear the read buffer */
	this->readBuff.consume(this->readBytes);

	return data;
}

boost::system::error_code Client::getErr() {
	return this->er;
}

size_t Client::getReadBytes() const {
	return this->readBytes;
}
