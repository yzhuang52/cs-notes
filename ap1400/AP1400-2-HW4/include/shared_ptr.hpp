#include "shared_ptr.h"

SharedPtr::SharedPtr() {
    this->_p = nullptr;
}

SharedPtr::SharedPtr(T* p) {
    this->_p = p;
    p = nullptr;
    this->count = 1;
}

template <typename T> SharedPtr<T> make_shared(T object) {
    return SharedPtr(new T(object));
}

SharedPtr::~SharedPtr() {
    if (this->count == 0)
    {
        delete this._p;
        this._p = nullptr;
    }
}

SharedPtr::SharedPtr(const SharedPtr<T>& object) {
    this->_p = new T(*object);
    this->count = 1;
}

SharedPtr<T> SharedPtr::operator=(const SharedPtr<T>& object) {
    this->_p = object.get();
    this->count = object.use_count();
    this->count += 1;
    
}

int SharedPtr::use_count() {
    return this->count;
}

