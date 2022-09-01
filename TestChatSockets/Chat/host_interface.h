#pragma once

#include <boost/asio.hpp>

#include <iostream>
#include <memory>

class host_interface {
protected:
	/* context to execure the socket work */
	boost::asio::io_context io_context;

	/* buffer used to store the read informations */
	boost::asio::streambuf readBuff;

	/* used to check if an operation went wrong */
	boost::system::error_code er;

	/* bytes readen by the readBuff */
	size_t readBytes;

	/* socket used to comunicate */
	std::shared_ptr<boost::asio::ip::tcp::socket> socket;

public:
	host_interface() = default;

	/* send a message */
	bool send(const std::string& msg);

	/* store the sent bytes in an internal buffer */
	bool read_until(const::std::string& del);

	/*
		return the content of the internal buffer

		Warning: Use this method only if the 'read' operation went fine
	*/
	const char* getMessage();

	/* return the boost::system::error_code object */
	boost::system::error_code getErr();

	/*
		return the number of readen bytes

		Warning: Use this method only if the 'read' operation went fine
	*/
	size_t getReadBytes() const;
};
