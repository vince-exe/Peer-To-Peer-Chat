#pragma once

#include "net_common.h"
#include "net_message.h"
#include "net_tsqueu.h"
#include "net_connection.h"


namespace olc {

	namespace net {
		
		template <typename T>
		class client_interface {
		protected:
			/* asio context handle the data transfer */
			asio::io_context m_context;
			
			/* thread to execute the context commands, without blocking the code */
			std::thread thrContext;

			/* hardware socket connected to the server */
			asio::ip::tcp::socket m_socket;

			/* the client has a single istance of "connection" object, which handles data transfer */
			std::unique_ptr<connection<T>> m_connection;

		private:
			/* this is the thread safe queue of incoming messages from server */
			tsqueue<owned_message<T>> m_qMessagesIn;

		public:
			client_interface() : m_socket(m_context) {

			}
			
			virtual ~client_interface() {
				this->disconnect();
			}

			/* connect to server with hostname/ip-address and port */
			bool connect(const std::string& host, const uint16_t port) const {

				try {

					/* create connection */
					this->m_connection = std::make_unique<connection<T>>();

					/* resolve hostname/ip-address into tangiable physical address */
					asio::ip::tcp::resolver resolver(this->m_context);
					auto m_endpoints = resolver.resolve(host, std::to_string(port));

					/* tell to the connection object to connect to the server */
					this->m_connection->connectToServer(m_endpoints);

					/* start context thread */
					this->thrContext = std::thread([this]() { m_context.run(); });

				}
				catch (std::exception& e) {
					std::cerr << "Client Exception: " << e.what() << "\n";
					return false;
				}

				return true;
			}

			void disconnect() {
				if (this->isConnected()) {
					this->m_connection->disconnect();
				}

				this->m_context.stop();
				if (this->thrContext.joinable()) {
					this->thrContext.join();
				}

				this->m_connection.release();
			}
		

			bool isConnected() const {
				if (this->m_connection) 
					return this->m_connection->isConnected();
				else 
					return false;
			}

			/* retrieve queue of message from server */
			tsqueue<owned_message<T>>& getIncomingMesages() {
				return this->m_qMessagesIn;
			}
		};
	}
}