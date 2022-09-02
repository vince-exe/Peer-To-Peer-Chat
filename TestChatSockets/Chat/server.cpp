#include "server.h"

ServerSide::Server::Server(int port) {
	/* allocate the memory for the server socket */
	this->socket = std::make_shared<boost::asio::ip::tcp::socket>(this->io_context);

	/* allocate the memory for the acceptor */
	this->acceptor = std::make_shared<boost::asio::ip::tcp::acceptor>(this->io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
}

void ServerSide::Server::listen() {
	/* start listening for new connection */
	this->acceptor->accept(*this->socket);
	this->is_open = true;
}

void ServerSide::Server::shutdown() {
	this->acceptor->close();
	this->socket->close();
	this->is_open = false;
}

void ServerSide::Server::setOpen(bool status) {
	this->is_open = status;
}

bool ServerSide::Server::isOpen() const {
	return this->is_open;
}
