#include "Client.h"

/* definining the error code variable */
boost::system::error_code Client::er;

bool Client::connect(boost::asio::ip::tcp::socket& socket, const char* ip, uint16_t port) {
	/* connect the socket */
	try {
		socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), port));
	
		this->ip = ip; this->port = port;
	}
	/* catch connection exceptions */
	catch (boost::system::system_error const& e) {
		std::cerr << e.what() << std::endl;
		return false;
	}

	return true;
}

bool Client::send(boost::asio::ip::tcp::socket& socket, const std::string& msg, boost::system::error_code er) {
	boost::asio::write(socket, boost::asio::buffer(msg), er);

	return (er) ? false : true;
}

std::string Client::read(boost::asio::ip::tcp::socket& socket, boost::system::error_code er) {
	boost::asio::streambuf buffer;
	boost::asio::read_until(socket, buffer, "\n");
	std::string data = boost::asio::buffer_cast<const char*>(buffer.data());
	return data;
	/* check if his an empty message or if an error occured */
	// return (!rBytes || er) ? false : true;
}

const char* Client::getMessage() {
	/*boost::asio::streambuf streamBuffer;
	const char* data = boost::asio::buffer_cast<const char*>(this->buffer->data());

	return data;*/
	return nullptr;
}
