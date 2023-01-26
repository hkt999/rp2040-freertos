#pragma once

#include <cstring>
#include <stdlib.h>
#include <ctype.h>

namespace ministd {
	class string {
		protected:
			char *m_s;
			size_t m_bufSize;

		public:
			string() { m_s = NULL; m_bufSize = 0; }
			string(const string &src) {
				if (src.m_s) {
					m_s = strdup(src.m_s);
					m_bufSize = size()+1;
				} else {
					m_s = NULL;
					m_bufSize = 0;
				}
			}
			string(const char *src, bool inplace = false ) {
			    m_s = inplace ? (char *)src : strdup(src);
				m_bufSize = strlen(m_s)+1;
			}
			string(const char *src, size_t size) {
					m_s = (char *)malloc(size+1);
					memcpy(m_s, src, size);
					m_s[size] = '\0';
					m_bufSize = size;
			}
			~string() { if (m_s) free(m_s); }
			string &toUpper() {
				if (m_s) {
					for (size_t i = 0; i < size(); i++) {
						m_s[i] = toupper(m_s[i]);
					}
				}
				return *this;
			}
			string &operator=(const string &src) {
				if (m_s) free(m_s);
				if (src.m_s) {
					m_s = strdup(src.m_s);
					m_bufSize = strlen(m_s) + 1;
				} else {
					m_s = nullptr;
					m_bufSize = 0;
				}
				return *this;
			}
			bool operator==(const string &src) {
				if (m_s && src.m_s) {
					return strcmp(m_s, src.m_s) == 0;
				} else {
					return m_s == src.m_s;
				}
			}
			bool operator<(const string &src) {
				return (m_s && src.m_s) ? (strcmp(m_s, src.m_s) < 0) : (m_s < src.m_s);
			}
			bool operator>(const string &src) {
				return (m_s && src.m_s) ? (strcmp(m_s, src.m_s) > 0) : (m_s > src.m_s);
			}
		public:
			int push_back(char ch) {
				if (m_bufSize == 0) {
					m_s = (char *)malloc(sizeof(char)*4);
					m_s[0] = ch;
					m_bufSize = 4;
				} else {
					if (size() >= m_bufSize-1) {
						m_s = (char *)realloc(m_s, m_bufSize*2);
						m_s[m_bufSize] = ch;
						m_bufSize++;
					}
					size_t l = length();
					m_s[l] = ch;
					m_s[l+1] = 0;
				}
				return size();
            }
			string &operator+=(const char ch) {
				push_back(ch);
				return *this;
			}
			const char *c_str() const { return m_s ? m_s : ""; }
			size_t length() const { return m_s ? strlen(m_s) : 0; }
			size_t size() const { return length(); }


	};
}