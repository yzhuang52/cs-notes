#ifndef SHARED_PTR
#define SHARED_PTR


template <typename T> class SharedPtr {
    public:
        int* count;
        SharedPtr() {
            this->_p = nullptr;
            this->count = new int(0);
        }
        SharedPtr(T* p) {
            this->_p = p;
            p = nullptr;
            this->count = new int(1);
        }
        SharedPtr(const SharedPtr<T>& object) {
            if (object.get() == nullptr)
            {
                SharedPtr();
            } else 
            {
                this->_p = object.get();
                this->count = object.count;
                *(this->count) += 1;
            }
            
        }
        ~SharedPtr() {
            this->_p = nullptr;
            if (*(this->count) != 0)
            {
                *(this->count) -= 1;
            }
        }
        int use_count() {
            return *(this->count);
        }
        T* get() const {
            return this->_p;
        }
        T* operator->() {
            return this->_p;
        }
        SharedPtr<T>& operator=(const SharedPtr<T>& object) {
            if (this != &object) {
                this->_p = object.get();
                this->count = object.count;
                *(this->count) += 1;   
                return *this;
            } else
            {
                return *this;
            }
        }

        void reset() {
            delete this->_p;
            this->_p = nullptr; 
            *(this->count) = 0;
        }
        void reset(T* p) {
            delete this->_p;
            this->_p = nullptr;
            delete this->count;
            this->_p = p;
            p = nullptr;
            this->count = new int(1);
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
template <typename T> SharedPtr<T> make_shared(T object) {
    return SharedPtr<T>(new T(object));
}

#endif //SHARED_PTR