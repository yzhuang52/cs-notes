#ifndef SHARED_PTR
#define SHARED_PTR


template <typename T> class SharedPtr {
    public:
        SharedPtr();
        SharedPtr(T* p);
        SharedPtr(const SharedPtr<T>& object);
        ~SharedPtr();
        int use_count();
        T* get();
        T* operator*();
        T* operator->();
        SharedPtr<T> operator=(const SharedPtr<T>& object);
        void reset();
        void reset(T* p);
        
    private:
        T* _p;
        int count;
}
template <typename T> SharedPtr<T> make_shared(T object);

#endif //SHARED_PTR