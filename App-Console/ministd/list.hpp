#pragma once

#include <stdlib.h>

namespace ministd {
	template <typename T> class ListNode {
		public:
			T data;
			ListNode *next;
			ListNode *prev;

		public:
			ListNode(T &data, ListNode *next, ListNode *prev):data(data), next(next), prev(prev) {
				if (prev) {
					prev->next = this;
				}
				if (next) {
					next->prev = this;
				}
			}
			ListNode() { this->next = NULL; this->prev = NULL; }
			ListNode(const ListNode &src) {
				data = src.data;
				next = src.next;
				prev = src.prev;
			}
			~ListNode() {}
	};

	template <typename T> class list {
		protected:
			ListNode<T> *m_head;
			ListNode<T> *m_tail;
			size_t m_count;

		public:
			list() {
				m_head = m_tail = nullptr;
				m_count = 0;
			}

			~list() {}

		public:
			list &operator=(const list &src) {
				if (src.m_head != NULL) {
					ListNode<T> *node = src.m_head;
					while (node != src.m_tail) {
						push_back(node->data);
						node = node->next;
					}
				}
				return *this;
			}
			void push_back(T &obj) {
				ListNode<T> *node = new ListNode<T>(obj, nullptr, m_tail);
				if (m_head == nullptr && m_tail == nullptr) {
					m_head = m_tail = node;
				} else {
					m_tail->next = node;
					m_tail = node;
				}
				m_count++;
			}
			void push_front(T &obj) {
				ListNode<T> *node = new ListNode<T>(obj, m_head, nullptr);
				if (m_head == nullptr && m_tail == nullptr) {
					m_head = m_tail = node;
				} else {
					m_head->prev = node;
					m_head = node;
				}
				m_count++;
			}
			void pop_back() {
				if (m_tail != nullptr) {
					if (m_head == m_tail) {
						delete m_head;
						m_head = m_tail = nullptr;
					} else {
						ListNode<T> *node = m_tail;
						m_tail = m_tail->prev;
						m_tail->next = nullptr;
						delete node;
					}
					m_count--;
				}
			}
			void pop_front() {
				if (m_head != nullptr) {
					if (m_head == m_tail) {
						delete m_head;
						m_head = m_tail = nullptr;
					} else {
						ListNode<T> *node = m_head;
						m_head = m_head->next;
						m_head->prev = nullptr;
						delete node;
					}
					m_count--;
				}
			}
			void clear() {
				ListNode<T> *node = m_head;
				while (node != nullptr) {
					ListNode<T> *next = node->next;
					delete node;
					node = next;
				}
				m_head = m_tail = nullptr;
				m_count = 0;
			}
			inline size_t size() {
				return m_count;
			}

            class iterator {
                private:
                    ListNode<T> *m_listNode;
            
                public:
                    iterator() : m_listNode(nullptr) {}
                    iterator(const iterator &i) { m_listNode = i.m_listNode; }
					iterator(ListNode<T> *node) { m_listNode = node; }
                    iterator &operator=(const iterator& it) { m_listNode = it.m_listNode; return *this; }
                    bool operator==(const iterator& rhs) const { return (m_listNode == rhs.m_listNode); }
                    bool operator!=(const iterator& rhs) const { return (m_listNode != rhs.m_listNode); }
                    T &operator*() const { return m_listNode->data; }
                    T *operator->() { return m_listNode ? &m_listNode->data : NULL; }
                    iterator& operator++() { m_listNode = m_listNode ? m_listNode->next : NULL; return *this; }
                    iterator operator++(int) { iterator tmp(*this); ++(*this); return tmp; }
					iterator& operator--() { m_listNode = m_listNode ? m_listNode->prev : NULL; return *this; }
					iterator operator--(int) { iterator tmp(*this); --(*this); return tmp;}
				friend class list<T>;
            };
            
            iterator begin() const {
                return iterator(this->m_head);
            }
            iterator end() const {
                return iterator(NULL);
            }
			void erase(iterator &it) {
				if (it.m_listNode != NULL) {
					if (it.m_listNode == m_head) {
						pop_front();
					} else if (it.m_listNode == m_tail) {
						pop_back();
					} else {
						ListNode<T> *node = it.m_listNode;
						node->prev->next = node->next;
						node->next->prev = node->prev;
						delete node;
						m_count--;
					}
				}
			}
	};
}