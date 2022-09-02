#include "host_interface.h"

bool host_interface::send(const std::string& msg) {
	boost::asio::write(*this->socket, boost::asio::buffer(msg), this->er);

	return (this->er) ? false : true;
}

bool host_interface::read_until(const std::string& del) {
	this->readBytes = boost::asio::read_until(*this->socket, this->readBuff, del, this->er);
	
	/* check if the readen bytes are 0 or an errro occurred */
	return (this->er) ? false : true;
}

const char* host_interface::getMessage() {
	const char* data = boost::asio::buffer_cast<const char*>(this->readBuff.data());

	/* clear the read buffer */
	this->readBuff.consume(this->readBuff.size() + 1);
	return data;
}

boost::system::error_code host_interface::getErr() {
	return this->er;
}

size_t host_interface::getReadBytes() const {
	return this->readBytes;
}