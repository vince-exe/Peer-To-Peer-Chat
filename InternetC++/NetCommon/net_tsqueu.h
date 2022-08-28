#pragma once

#include "net_common.h"

namespace olc {

	namespace net {
		
		template<typename T>
		class tsqueue {
		public:
			tsqueue() = default;

			tsqueue(const tsqueue<T>&) = delete;

			virtual ~tsqueue() { this->clear(); }

		public:
			/* return and maintains item at front of Queue */
			const T& front() {
				/* protect this queue by other threads, when it's doing this operation */
				std::scoped_lock lock(this->muxQueue);
				return this->deqQueue.front();
			}

			/* return and maintains item at back od Queue */
			const T& back() {
				std::scoped_lock lock(this->muxQueue);
				return this->deqQueue.back();
			}

			/* adds item to back of Queue */
			void push_back(const T& item) {
				std::scoped_lock lock(this->muxQueue);
				return this->deqQueue.emplace_back(std::move(item));
			}

			/* add item to front of Queue */
			void push_front(const T& item) {
				std::scoped_lock lock(this->muxQueue);
				this->deqQueue.emplace_front(std::move(item));
			}

			/* return true if Queue has no items */
			bool empty() {
				std::scoped_lock lock(this->muxQueue);
				return this->deqQueue.empty();
			}

			/* return the number of items in Queue */
			size_t count() {
				std::scoped_lock lock(this->muxQueue);
				return this->deqQueue.size();
			}

			/* clears Queue */
			void clear() {
				std::scoped_lock lock(this->muxQueue);
				this->deqQueue.clear();
			}

			/* remove and return item from back of Queue */
			T pop_back() {
				std::scoped_lock lock(this->muxQueue);
				auto t = std::move(this->deqQueue.back());
				this->deqQueue.pop_back();
				return t;
			}

			/* remove and return item from front of Queue */
			T pop_front() {
				std::scoped_lock lock(this->muxQueue);
				auto t = std::move(this->deqQueue.front());
				this->deqQueue.pop_front();
				return t;
			}

		protected:
			std::mutex muxQueue;
			std::deque<T>deqQueue;
		};
	}
}