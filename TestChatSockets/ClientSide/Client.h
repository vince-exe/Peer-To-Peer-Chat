#pragma once

#include <iostream>
#include <memory>

#include <boost/asio.hpp>

class Client {
public:
	/* used to check if an operation wrong */
	static boost::system::error_code er;

private:
	boost::asio::io_context io_context;

	std::shared_ptr<boost::asio::ip::tcp::socket> socket;

	const char* ip = nullptr;

	uint16_t port ;

public:
	Client() = default;

	/* connect the socket to a specif ip and port*/
	bool connect(boost::asio::ip::tcp::socket& socket, const char* ip, uint16_t port);

	/* send a message */
	bool send(boost::asio::ip::tcp::socket& socket, const std::string& msg, boost::system::error_code er);

	/* read a message */
	std::string read(boost::asio::ip::tcp::socket& socket, boost::system::error_code er);

	/* return the message read */
	const char* getMessage();
};
