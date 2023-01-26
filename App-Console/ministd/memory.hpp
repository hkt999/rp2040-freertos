#pragma once

namespace ministd {

template <class T> class unique_ptr
{
    private:
        T * ptr = nullptr;

    public:
        // default constructor
        unique_ptr() : ptr(nullptr) { }
        unique_ptr(T * ptr) : ptr(ptr) { }
        unique_ptr(const unique_ptr & obj) = delete; // copy constructor is deleted
        unique_ptr& operator=(const unique_ptr & obj) = delete; // copy assignment is deleted

        // move constructor
        unique_ptr(unique_ptr && dyingObj) {
            // Transfer ownership of the memory pointed by dyingObj to this object
            this->ptr = dyingObj.ptr;
            dyingObj.ptr = nullptr; 
        }

        // move assignment
        void operator=(unique_ptr && dyingObj) {
            __cleanup__(); // cleanup any existing data
            // Transfer ownership of the memory pointed by dyingObj to this object
            this->ptr = dyingObj.ptr;
            dyingObj.ptr = nullptr;
        }

        // deferencing arrow operator
        T* operator->() { return this->ptr; }
        T& operator*() { return *(this->ptr); }

        // destructor
        ~unique_ptr() { __cleanup__(); }

    private:
        void __cleanup__() {
            if (ptr != nullptr)
                delete ptr;
        }
};

template <class T> class unique_ptr<T[]>
{
    private:
        T * ptr = nullptr;

    public:
        // default constructor
        unique_ptr() : ptr(nullptr) { }
        unique_ptr(T * ptr) : ptr(ptr) { }
        unique_ptr(const unique_ptr & obj) = delete; // copy constructor is deleted
        unique_ptr& operator=(const unique_ptr & obj) = delete; // copy assignment is deleted

        // move constructor
        unique_ptr(unique_ptr && dyingObj) {
            __cleanup__(); // cleanup any existing data

            // Transfer ownership of the memory pointed by dyingObj to this object
            this->ptr = dyingObj.ptr;
            dyingObj.ptr = nullptr;
        }

        // move assignment
        void operator=(unique_ptr && dyingObj) {
            __cleanup__(); // cleanup any existing data

            // Transfer ownership of the memory pointed by dyingObj to this object
            this->ptr = dyingObj.ptr;
            dyingObj.ptr = nullptr;
        }

        // deferencing arrow operator
        T* operator->() { return this->ptr; }
        T& operator*() { return *(this->ptr); }
        T& operator[](int index) { return this->ptr[index]; } // doesn't check out of bounds
        ~unique_ptr() { __cleanup__(); } // destructor

    private:
        void __cleanup__() {
            if (ptr != nullptr)
                delete[] ptr;
        }
};

// shared_ptr
template<class T> class shared_ptr
{
    private:
        T * ptr = nullptr;
        uint * refCount = nullptr;

    public:
        shared_ptr() : ptr(nullptr), refCount(new uint(0)) { } // default constructor
        shared_ptr(T * ptr) : ptr(ptr), refCount(new uint(1)) { } // constructor

        // copy constructor
        shared_ptr(const shared_ptr & obj) {
            this->ptr = obj.ptr; // share the underlying pointer
            this->refCount = obj.refCount;
            if (nullptr != obj.ptr) {
                (*this->refCount)++; // if the pointer is not null, increment the refCount
            }
        }

        // copy assignment
        shared_ptr& operator=(const shared_ptr & obj) {
            __cleanup__(); // cleanup any existing data
            
            // Assign incoming object's data to this object
            this->ptr = obj.ptr; // share the underlying pointer
            this->refCount = obj.refCount;
            if (nullptr != obj.ptr) {
                (*this->refCount)++; // if the pointer is not null, increment the refCount
            }
        }

        // move constructor
        shared_ptr(shared_ptr && dyingObj) {
            this->ptr = dyingObj.ptr; // share the underlying pointer
            this->refCount = dyingObj.refCount;
            dyingObj.ptr = dyingObj.refCount = nullptr; // clean the dying object
        }

        // move assignment
        shared_ptr& operator=(shared_ptr && dyingObj) {
            __cleanup__(); // cleanup any existing data
            
            this->ptr = dyingObj.ptr; // share the underlying pointer
            this->refCount = dyingObj.refCount;

            dyingObj.ptr = dyingObj.refCount = nullptr; // clean the dying object
        }

        uint get_count() const {
            return *refCount; // *this->refCount
        }

        T* get() const { return this->ptr; }
        T* operator->() const { return this->ptr; }
        T& operator*() const { return this->ptr; }
        // destructor
        ~shared_ptr() { __cleanup__(); }

    private:
        void __cleanup__() {
            (*refCount)--;
            if (*refCount == 0) {
                if (nullptr != ptr)
                    delete ptr;
                delete refCount;
            }
        }
};

} // namespace ministd