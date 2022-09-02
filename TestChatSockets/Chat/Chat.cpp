#include <iostream>

#include "server_main.h"
#include "client_main.h"

enum MainOptions {
	StartServer = 1,
	ConnectClient,
	ExitProgram,
};

int main() {
	int opt;
	bool run = true;

	do {
		std::cout << "\n1)Start Server\n2)Connect\n3)Exit\n\nInsert option: ";
		std::cin >> opt;

		switch (opt) {
		case MainOptions::StartServer:
			Chat::server_main();
			break;

		case MainOptions::ConnectClient:
			Chat::client_main();
			break;

		case MainOptions::ExitProgram:
			std::cout << "\nThank you!!";
			run = false;
			break;

		default:
			std::cout << "\nInsert a valid option" << std::endl;
			break;
		}
	} while (run);

	return 0;
}