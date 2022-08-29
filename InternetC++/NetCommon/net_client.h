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
			sts::thread thrContext;

			/* hardware socket connected to the server */
			asio::ip::tcp::socket m_socket;

			/* the client has a single istance of "connection" object, which handles data transfer */
			std::unique_ptr<connection<T>> m_connection;

		private:
			/* this is the thread safe queue of incoming messages from server */
			tsqueue<owned_message<T>> m_qMessagesIn;

		public:
			/* connect to server with hostname/ip-address and port */
			bool connect(const std::string& host, const uint16_t port) const {

				return false;
			}

			void disconnect() {

			}

			bool isConnected() const {

				return false;
			}

			/* retrieve queue of message from server */
			tsqueue<owned_message<T>>& getIncomingMesages() {


			}
		};
	}
}