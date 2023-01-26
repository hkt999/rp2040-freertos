#pragma once

namespace ministd {
	template <typename K, typename V> class pair {
		public:
			K first;
			V second;

		public:
			pair(K _first, V _second):first(_first), second(_second) {}
			pair(const pair &p) { first = p.first; second = p.second; }
			pair &operator=(pair &p) { first = p.first; second = p.second; return *this; }
	};
}
