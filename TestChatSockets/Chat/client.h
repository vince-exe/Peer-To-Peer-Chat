#pragma once

#include "host_interface.h"

class Client : public host_interface {
public:
	Client() = default;

	/* connect the socket to a specif ip and port*/
	bool connect(const char* ip, uint16_t port);

};


