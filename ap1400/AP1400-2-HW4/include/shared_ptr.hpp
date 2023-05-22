// #include "shared_ptr.h"

// SharedPtr::SharedPtr() {
//     this->_p = nullptr;
//     this->count = nullptr;
// }

// SharedPtr::SharedPtr(T* p) {
//     this->_p = p;
//     p = nullptr;
//     this->count = new int(1);
// }

// template <typename T> SharedPtr<T> make_shared(T object) {
//     return SharedPtr<T>(new T(object));
// }

// SharedPtr::~SharedPtr() {
//     this->_p = nullptr;
//     if (*(this->count) != 0)
//     {
//         *(this->count) -= 1;
//     }
//     this->count = nullptr;
// }

// SharedPtr::SharedPtr(const SharedPtr<T>& object) {
//     if (object.get() == nullptr)
//     {
//         SharedPtr();
//     } else 
//     {
//         this->_p = new T(*object.get());
//         this->count = new int(1);
//     }
    
// }

// SharedPtr<T> SharedPtr::operator=(const SharedPtr<T>& object) {
//     this->_p = object.get();
//     this->count = object.count;
//     this->count += 1;
    
// }

// int SharedPtr::use_count() {
//     return *(this->count);
// }

// T* SharedPtr::get() const {
//     return this->_p;
// }

// T& SharedPtr::operator*() {
//     if (this->_p != nullptr)
//     {
//         return *(this->_p);
//     } 
// }
// T* SharedPtr::operator->() {
//     return this->_p;
// }
// void SharedPtr::reset() {
//     delete this->_p;
//     this->_p = nullptr; 
//     delete this->count;
//     this->count = nullptr;
// }

// void SharedPtr::reset(T* p) {
//     delete this->_p;
//     this->_p = nullptr;
//     delete this->count;
//     this->count = nullptr;
//     this->_p = p;
//     p = nullptr;
//     this->count = new int(1);
// }

// operator SharedPtr::bool() const {
//     return this->_p != nullptr;
// }