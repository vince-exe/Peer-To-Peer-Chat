#pragma once

#include "net_common.h"

namespace olc {

	namespace net {

		/* 
			Message Header is sent at start of all messages. The template allow us to use 
			"enum class" to ensure that the message are valid at compile time
		*/
		template <typename T>
		struct message_header {
			T id{};
			uint32_t size = 0;
		};

		template <typename T>
		struct message {
			message_header<T> header{};
			std::vector<uint8_t> body;

			/* return the size of entire message packet in bytes */
			size_t size() const {
				return sizeof(message_header<T>) + body.size();
			}

			/* override for std::cout compatibility ( we use friend because we want to use it everywhere ) */
			friend std::ostream& operator << (std::ostream& os, const message<T>& msg) {
				os << "ID: " << int(msg.header.id) << " Size: " << msg.header.size;
				
				return os;
			}

			/* pushes data into the message buffer */
			template<typename DataType>
			friend message<T>& operator << (message<T>& msg, const DataType& data) {
				/* check that the type of data being pushed is trivially coyable */
				static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed");

				/* cache the current size of the vector */
				size_t i = msg.body.size();

				/* resize the vector by the size of the data being pushed */
				msg.body.resize((i + sizeof(DataType));

				/* 
					Physically copy the data into the newly allocated vector space 

					1) Il primo argomento è la destinazione che sarebbe l'ultima posizione del vettore ( quella appena allocata ) alla quale si accede con l'aritmetica dei puntatori aggiungendo al puntatore ritornato con data() la vecchia size del vettore
					2) la source cioè l'indirizzo di memoria dell dato da aggiungere che prendiamo con l'&
					3) la quantità di bytes da aggiungere 
				*/
				std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

				/* update the size of the vector */
				msg.header.size = msg.size();
			}
			
			/* pop data */
			template<typename DataType>
			friend message<T>& operator >> (message<T>& msg, DataType& data) {
				static_assert(std::is_standard_layout<DataType>::value, "Data is too complex");

				/* 
					cache the location toward the end of the vector where the pulled data starts 
					
					Salva la posizione verso la fine del vettore, dove l'area di memoria del dato da cacciare inizia
				*/
				size_t i = msg.body.size() - sizeof(DataType);

				/* 
					Physically copy the data from the vector into the user variable 
					
					1) La destinazione in questo caso è la variabile che l'utente passera ( il suo indirizzo di memoria )
					2) La sorgente ossia l'area di memoria dell'indirizzo da cacciare 
					3) I bytes da copiare
				*/
				std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

				/* reduce the vector size */
				msg.body.resize(i);

				/* update the size */
				msg.header.size = msg.size();

				return msg;
			}
		};
	}
}