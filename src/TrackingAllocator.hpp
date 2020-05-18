//
// Created by denis on 28.07.19.
//

#ifndef VEC_LIB_TRACKINGALLOCATOR_HPP
#define VEC_LIB_TRACKINGALLOCATOR_HPP

#include <iostream>
#include <new>

/*Just the implementation of the standard function for perfect forwarding*/
template < class T >
T &&my_forward(std::remove_reference_t< T > &obj);


template < class T >
class TrackingAllocator {
public:
    using value_type = T;
    using pointer = T *; // here the rule of deducting pointers work
    using reference = T &;

    TrackingAllocator() = default;
    ~TrackingAllocator() = default;

    template < class U >
    struct rebind {
        using other_alloc =  TrackingAllocator< U >;
    };

    template < class U >
    TrackingAllocator(const TrackingAllocator< U > &alloc) {}

    template < class U >
    TrackingAllocator< T > &operator=(const TrackingAllocator< U > &alloc) {}



    pointer allocate(std::size_t num_objects) const {
        ++count_allocations;
        return static_cast<pointer>(::operator new(sizeof(T) * num_objects));
    }

    void deallocate(pointer ptr, std::size_t num_objects) const {
        ::operator delete(ptr);
    }

    template <
            class U,
            class... Args
             >
    void construct(U *ptr, Args &&... args) const {
        new(ptr) U(my_forward< Args >(args)...);
    }

    template < class U >
    void destroy(U *ptr) const {
        ptr->~U();
    }

    std::size_t GetNumberAllocations() const {
        return count_allocations;
    }


private:
    static std::size_t count_allocations;

};

template < class T >
T &&my_forward(std::remove_reference_t< T > &obj) {
    return static_cast<T &&>(obj);
}

template < class T >
typename std::size_t TrackingAllocator< T >::count_allocations = 0;

#endif //VEC_LIB_TRACKINGALLOCATOR_HPP
