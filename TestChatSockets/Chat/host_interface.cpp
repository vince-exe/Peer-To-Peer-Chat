#include "host_interface.h"

bool host_interface::send(const std::string& msg) {
	boost::asio::write(*this->socket, boost::asio::buffer(msg), this->er);

	return (this->er) ? false : true;
}

bool host_interface::read_until(const std::string& del) {
	boost::asio::streambuf buff;

	this->readBytes = boost::asio::read_until(*this->socket, buff, del, this->er);
	this->message = boost::asio::buffer_cast<const char*>(buff.data());

	/* check if the readen bytes are 0 or an errro occurred */
	return (this->er || !this->readBytes) ? false : true;
}

std::string host_interface::getMessage() {
	return this->message;
}

boost::system::error_code host_interface::getErr() {
	return this->er;
}

size_t host_interface::getReadBytes() const {
	return this->readBytes;
}