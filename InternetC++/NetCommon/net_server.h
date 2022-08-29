#pragma once

#include "net_common.h"
#include "net_tsqueu.h"
#include "net_message.h"
#include "net_connection.h"

namespace olc {
	
	namespace net {

		template<typename T>
		class server_interface {
		public:
			server_interface(uint16_t port) : m_asioAcceptor(m_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
			{
			
			}

			virtual ~server_interface() {
				this->stop();
			}

			bool start() {
				try {
					/* give some work to the asio context */
					this->waitForClientConnection();

					/* start the context thread */
					this->m_threadContext = std::thread([this]() { m_asioContext.run(); });


				}
				catch (std::exception& e) {
					std::cerr << "[ SERVER ] Exception: " << e.what() << "\n";

					return false;
				}

				std::cout << "[ SERVER ] Started" << "\n";
				return true;
			}

			void stop() {
				/* request the context to stop */
				this->m_asioContext.stop();

				if (this->m_threadContext.joinable()) {
					this->m_threadContext.join();
				}

				std::cout << "[ SERVER ] Stopped " << "\n"; 
			}

			/* async wait for connection */
			void waitForClientConnection() {
				this->m_asioAcceptor.async_accept([this](std::error_code ec, asio::ip::tcp::socket socket) {
					if (!ec) {
						std::cout << "[ SERVER ] New Connection: " << socket.remote_endpoint() << "\n"; 

						/* create a new connection */
						std::shared_ptr<connection<T>> newConn =
							std::make_shared<connection<T>>(connection<T>::owner::server, m_asioContext, std::move(socket), m_qMessagesIn);
						
						/* give the user a chance to deny connection */
						if (onClientConnect(newConn)) {
							/* connection allowed, so add the connection to the connections container */
							m_deqConnection.push_back(std::move(newConn));
							/* increment the id of the connected client */
							m_deqConnection.back()->ConnectToClient(nIDCounter++);
						}
						else {
							std::cout << "[ SERVER ] Connection denied" << "\n";
						}
					}
					else {
						/* an error has occurred */
						std::cout << "[ SERVER ] New Connection Error: " << ec.message() < "\n";
					}

					/* prime the asio context with more work ( listening to other sockets ), otherwise as soon as a client connect, the asio context will finish his job and will exit */
					waitForClientConnection();
				})
			}

			/* send a message to a specific client */
			void messageClient(std::shared_ptr<connection<T>> client, const message<T>& msg) {
				if (client && client->IsConnected()) {
					client->Send(msg);
				}
				else {
					this->onClientDisconnect(client);
					client.reset();
					/* erase the connection from the connection queue */
					this->m_deqConnection.erase(std::remove(this->m_deqConnection.begin(), this->m_deqConnection.end(), client), this->m_deqConnection.end());
				}
			}
			
			/* send a message to all the clients */
			void messageAllClients(const message<T>& msg, std::shared_ptr<connection<T>> pIgnoreClient) {
				bool invalidClientExists = false;

				for (auto& client : this->m_deqConnection) {
					/* check if the client is connected */
					if (client && client->IsConnected()) {

						if (client != pIgnoreClient) {
							client->Send(msg);
						}
					}
					else {
						/* the clients couldn't be contacted, so assume it has disconnected */
						this->onClientDisconnect(client);
						client.reset();
						invalidClientExists = true;
					}s
				}
				/* remove all the invalid clients */
				if (invalidClientExists) {
					this->m_deqConnection.erase(
						std::remove(m_deqConnection.begin(), m_deqConnection.end(), nullptr), m_deqConnection.end());
				}
			}

			void Update(size_t nMaxMessages = -1) {
				;
			}

		/* this methods are gonna be "virtual" to give the change to the library user to change them */
		protected:
			/* called when a client connects */
			virtual bool onClientConnect(std::shared_ptr<connection<T>> client) {

			}

			/* called when a client disconnects */
			virtual void onClientDisconnect(std::shared_ptr<connection<T>> client) {

			}

			/* called when a client message arrives */
			virtual void onMessage(std::shared_ptr<connection<T>> client, message<T>& msg) {

			}

		protected;
			/* thread safe queue for incoming message packets */
			tsqueue<owned_message<T>> m_qMessagesIn;

			/* container of active validated connections */
			std::deque<std::shared_ptr<connection<T>>> m_deqConnection;

			asio::io_context m_asioContext;
			std::thread m_threadContext;

			asio::ip::tcp::acceptor m_asioAcceptor; 

			/* clients will be identified in the "wider system" via an ID */
			uint32_t nIDCounter = 10000;
 		};
	}
}