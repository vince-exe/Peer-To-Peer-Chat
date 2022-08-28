#include <iostream>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

/* we are telling to the asio library to not include the BOOST framework that comes with this library */
#define ASIO_STANDALONE

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

std::vector<char> vBuffer(20 * 1024);

void grabSameData(asio::ip::tcp::socket& socket) {
	socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()), [&] /* capture all the external variables by reference */
		(std::error_code ec, std::size_t lenght) {

			if (!ec) {
				std::cout << "\nRead" << lenght << " bytes\n\n";

				for (int i = 0; i < lenght; i++) {
					std::cout << vBuffer[i];
				}

				grabSameData(socket);
			}
		});
}

int main() {
	/* handle 13 types of errors */
	asio::error_code errorHandler;
	
	/* a context - essentialy the platform specific interface */
	asio::io_context context;
	
	/* give some fake tasks to asio so the context doesn't finish */
	asio::io_context::work idleWork(context);

	/* start the context in his own thread */
	std::thread thrContext = std::thread([&]() {context.run(); });

	/* get the address of somewhere we wish connect to */
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34", errorHandler), 80);

	asio::ip::tcp::socket socket(context);

	/* try to connect */
	socket.connect(endpoint, errorHandler);

	if (!errorHandler) {
		std::cout << "\nSuccessfully connected";
	}
	else {
		std::cout << "\nFailed to connect [ " << errorHandler.message() << " ]";
	}
	
	if (socket.is_open()) {

		grabSameData(socket);

		std::string srequest =
			"GET /index.html HTTP/1.1\r\n"
			"Host: example.com\r\n"
			"Connection: close\r\n\r\n";

		socket.write_some(asio::buffer(srequest.data(), srequest.size()), errorHandler);

		using namespace std::chrono_literals;
		std::this_thread::sleep_for(2000ms);
	}
	return 0;
}