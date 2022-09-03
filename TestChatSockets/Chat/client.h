#pragma once

#include "host_interface.h"

namespace ClientSide {

	class Client : public host_interface {
	private:
		bool is_open;

	public:
		Client() = default;

		/* connect the socket to a specif ip and port*/
		bool connect(const std::string& ip, uint16_t port);

		/* shutdown the client */
		void shutdown();
	
		void setOpen(bool open);

		bool isOpen() const;
	};
}
