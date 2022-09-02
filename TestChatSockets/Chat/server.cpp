#include "server.h"

Chat::Server::Server(int port) {
	/* allocate the memory for the server socket */
	this->socket = std::make_shared<boost::asio::ip::tcp::socket>(this->io_context);

	/* allocate the memory for the acceptor */
	this->acceptor = std::make_shared<boost::asio::ip::tcp::acceptor>(this->io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
}

void Chat::Server::listen() {
	/* start listening for new connection */
	this->acceptor->accept(*this->socket);
}

void Chat::Server::shutdown() {
	this->acceptor->close();
	this->socket->close();
}
