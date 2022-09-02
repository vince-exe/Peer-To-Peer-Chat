#pragma once

#include "host_interface.h"

namespace Chat {
	class Client : public host_interface {
	public:
		Client() = default;

		/* connect the socket to a specif ip and port*/
		bool connect(const char* ip, uint16_t port);

		/* shutdown the client */
		void shutdown();
	};
}
