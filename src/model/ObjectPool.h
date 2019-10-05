#pragma once

#include <queue>
#include <thread>
#include <mutex>

namespace vc {
	template<typename T>
	class ObjectPool {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::queue<T*> objects;
			mutable std::mutex mutex;


		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ObjectPool(int size) {
				for(int counter = 0; counter < size; counter++) {
					this->objects.push(new T());
				}
			}
			~ObjectPool() {
				while(objects.size() != 0) {
					T* obj = objects.front();
					delete obj;

					objects.pop();
				}
			}
			ObjectPool& operator=(const ObjectPool&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			T* getElement() {
				std::lock_guard<std::mutex> lock1(mutex);

				if(objects.size() == 0) {
					return new T();
				}
				T* obj = objects.front();
				objects.pop();
				return obj;
			}
			void returnElement(T* elem) {
				std::lock_guard<std::mutex> lock1(mutex);


				objects.push(elem);
			}
			std::size_t size() {
				return objects.size();
			}

	};
}