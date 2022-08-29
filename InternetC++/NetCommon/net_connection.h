#pragma once

#include "net_common.h"
#include "net_message.h"
#include "net_tsqueu.h"


namespace olc {

	namespace net {

		template<typename T>
		class connection : public std::enable_shared_from_this<connection<T>> {
		public:
			connection() {}

			virtual ~connection() {}

		public:
			bool connectToServer() {};

			bool disconnect() {};

			bool IsConnected() const {};

		public:
			bool Send(const message<T>& msg) {};

		protected:
			/* each connection has a unique socket to a remote */
			asio::ip::tcp::socket m_socket;

			asio::io_context::io_context& m_asioContext;

			tsqueue<message<T>> m_qMessageOut;

			tsqueue<owned_message>& m_qMessageIn;
		};
	}
}