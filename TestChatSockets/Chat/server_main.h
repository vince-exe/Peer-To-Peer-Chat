#pragma once

#include <iostream>
#include <boost/asio.hpp>

#include "server.h"

namespace ServerSide {
	
	void server_main();

	void read_operation(ServerSide::Server& server);
	
	void send_operation(ServerSide::Server& server);
}
