#pragma once

#include "client.h"

namespace ClientSide {
	/* main function for the client */
	void client_main();

	void send_operation(ClientSide::Client& client);

	void read_operation(ClientSide::Client& client);
}
