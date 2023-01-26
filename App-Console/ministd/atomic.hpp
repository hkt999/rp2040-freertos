#pragma once

#include <pthread.h>

namespace ministd {
    template <typename T> class atomic {
        protected:
            pthread_mutex_t m_mutex;
            T m_data;

        public:
            atomic() {
                pthread_mutex_init(&m_mutex, nullptr);
            }
            atomic(const atomic &src) {
                pthread_mutex_init(&m_mutex, nullptr);
                *this = src;
            }
            atomic(T v) {
                pthread_mutex_init(&m_mutex, nullptr);
                m_data = v;
            }
            ~atomic() {
                pthread_mutex_destroy(&m_mutex);
            }

            T &operator=(const T &src) {
                pthread_mutex_lock(&m_mutex);
                this->m_data = src;
                pthread_mutex_unlock(&m_mutex);
                return *this;
            }
            T operator++(int) {
                pthread_mutex_lock(&m_mutex);
                m_data++;
                pthread_mutex_unlock(&m_mutex);
                return m_data;
            }
            T &operator++() {
                pthread_mutex_lock(&m_mutex);
                T v = ++m_data;
                pthread_mutex_unlock(&m_mutex);
                return v;
            }
            T operator--(int) { 
                pthread_mutex_lock(&m_mutex);
                m_data--;
                pthread_mutex_unlock(&m_mutex);
                return m_data;
            }
            T &operator--() {
                pthread_mutex_lock(&m_mutex);
                T v = --m_data;
                pthread_mutex_unlock(&m_mutex);
                return v;
            }
            T operator+(const T& src) {
                return m_data + src;
            }
            T operator-(const T& src) {
                return m_data - src;
            }
            T &operator+=(T &src) {
                pthread_mutex_lock(&m_mutex);
                m_data += src;
                pthread_mutex_unlock(&m_mutex);
                return *this;
            }
            T &operator-=(T &src) {
                pthread_mutex_lock(&m_mutex);
                m_data -= src;
                pthread_mutex_unlock(&m_mutex);
                return *this;
            }
            inline T &value() {
                return m_data;
            }
    };

    typedef atomic<bool> atomic_bool;
    typedef atomic<int> atomic_int;
    typedef atomic<uint8_t> atomic_uint8;
    typedef atomic<uint16_t> atomic_uint16;
    typedef atomic<uint32_t> atomic_uint32;
    typedef atomic<uint64_t> atomic_uint64;
    typedef atomic<int8_t> atomic_int8;
    typedef atomic<int16_t> atomic_int16;
    typedef atomic<int32_t> atomic_int32;
    typedef atomic<int64_t> atomic_int64;
    typedef atomic<float> atomic_float;
    typedef atomic<double> atomic_double;
};