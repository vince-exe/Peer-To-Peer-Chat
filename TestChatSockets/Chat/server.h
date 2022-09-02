#pragma once

#include "host_interface.h"
		
namespace ServerSide {
	
	class Server : public host_interface {
	private:
		/* create a shared ptr boost acceptor, for accepting new connection */
		std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor;
		
		/* used to check the status of the server */
		bool is_open;

	public:
		/* standard constructor */
		Server(int port = 4000);

		/* listen for new connections */
		void listen();

		/* shutdown the server */
		void shutdown();
		
		/* set the status of the server */
		void setOpen(bool status);

		bool isOpen() const;
	};
}
