#ifndef UNIQUE_PTR
#define UNIQUE_PTR

template <typename T> class UniquePtr {
    public:
        UniquePtr();
        UniquePtr(T* p);
        ~UniquePtr();
        T* get();
        T* operator->();
        void reset();
        void reset(T* p);
        T* release();
    private:
        T* _p;
}

template <typename T> class<T> make_unique(T object);
T* operator*(class<T> object);




#endif //UNIQUE_PTR