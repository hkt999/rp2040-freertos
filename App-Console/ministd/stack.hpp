#pragma once

#include "vector.hpp"

namespace ministd {
	template <typename T> class stack {
		private:
			vector<T> m_data;

		public:
			bool empty() { return (m_data.size()==0); }
			void push(T item) { m_data.push_back(item); }
			void pop() { m_data.pop_back(); }
			T peek() { return (m_data.size() > 0) ? m_data[m_data.size() - 1] : T(); }
			size_t size() { return m_data.size(); }
	};

} // ministd
