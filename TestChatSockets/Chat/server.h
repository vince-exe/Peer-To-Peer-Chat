#pragma once

#include "host_interface.h"

namespace Chat {
	class Server : public host_interface {
	private:
		/* create a shared ptr boost acceptor, for accepting new connection */
		std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor;

	public:
		/* standard constructor */
		Server(int port = 4000);

		/* listen for new connections */
		void listen();

		/* shutdown the server */
		void shutdown();
	};
}
