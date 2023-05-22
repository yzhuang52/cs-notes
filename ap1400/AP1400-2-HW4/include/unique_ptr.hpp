// #include "unique_ptr.h"

// UniquePtr::UniquePtr() {
//     this->_p = nullptr;
// }

// UniquePtr::UniquePtr(T* p) {
//     this->_p = p;
//     p = nullptr;
// }

// UniquePtr::UniquePtr(const T& object) {
//     throw std::logic_error("unique pointer not allowed to copy\n");
// }

// void UniquePtr::operator=(const UniquePtr<T>& object) {
//     throw std::logic_error("unique pointer not allowed to copy\n");
// }

// UniquePtr::~UniquePtr() {
//     delete this->_p;
//     this->_p = nullptr;
// }

// T* UniquePtr::get() {
//     return this->_p;
// }

// T* operator->() {
//     return this->_p;
// }

// void UniquePtr::reset() {
//     delete this->_p;
//     this->_p = nullptr;
// }

// void UniquePtr::reset(T* p) {
//     delete this->_p;
//     this->_p = p;
//     p = nullptr;
// }

// T* UniquePtr::release() {
//     T* temp = this->_p;
//     this->_p = nullptr;
//     return temp;
// }

// T& UniquePtr::operator*() {
//     return *(this->_p);
// }

// template <typename T> UniquePtr<T> make_unique(T object) {
//     return UniquePtr<T>(object);
// }

// operator UniquePtr::bool() const {
//     return this->_p != nullptr;
// }