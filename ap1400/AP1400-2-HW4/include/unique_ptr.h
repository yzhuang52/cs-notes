#ifndef UNIQUE_PTR
#define UNIQUE_PTR
#include <stdexcept>

template <typename T> class UniquePtr {
    public:
        UniquePtr() {
            this->_p = nullptr;
        }
        UniquePtr(T* p) {
            this->_p = p;
            p = nullptr;
        }
        UniquePtr(const UniquePtr<T>& object) {
            throw std::logic_error("unique pointer not allowed to copy\n");
        }
        void operator=(const UniquePtr<T>& object) {
            throw std::logic_error("unique pointer not allowed to copy\n");
        }
        ~UniquePtr() {
            delete this->_p;
            this->_p = nullptr;
        }
        T* get() {
            return this->_p;
        }
        T* operator->() {
            return this->_p;
        }
        void reset() {
            delete this->_p;
            this->_p = nullptr;
        }
        void reset(T* p) {
            delete this->_p;
            this->_p = p;
            p = nullptr;
        }
        T* release() {
            T* temp = this->_p;
            this->_p = nullptr;
            return temp;
        }
        T& operator*() {
            return *(this->_p);
        }
        operator bool() const {
            return this->_p != nullptr;
        }
    private:
        T* _p;
};

template <typename T> UniquePtr<T> make_unique(T object) {
    return UniquePtr<T>(new T(object));
}


#endif //UNIQUE_PTR