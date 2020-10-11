/*
 * intrusive_ptr.hpp
 *
 *  Created on: Apr 25, 2019
 *      Author: bryan.flynt
 */

#ifndef INTRUSIVE_PTR_HPP_
#define INTRUSIVE_PTR_HPP_

#include <cassert>     // assert
#include <cstddef>     // std::nullptr_t
#include <functional>  // std::less, std::hash
#include <type_traits> // std::remove_extent<T>


namespace xstd {

/// Smart pointer that retains shared ownership through a pointer
/**
 * xstd::intrusive_ptr is a smart pointer that retains shared ownership
 * of an object through a pointer. Several intrusiv_ptr objects may own
 * the same object. The object is destroyed and its memory deallocated
 * when either of the following happens:
 *
 * - the last remaining intrusive_ptr owning the object is destroyed
 * - the last remaining intrusive_ptr owning the object is assigned
 *   another pointer via operator= or reset().
 *
 * The difference to the std::shared_ptr is the intrusive_ptr version
 * attaches the reference counting to the object thereby insuring the
 * memory is closer and a slight performance optimization can be made.
 * There are two ways to attach the reference counting using the
 * supporting class.
 *
 * The simplest is to inherit from the xstd::intrusive_base class:
 * \code
 * class Animal : public xstd::intrusive_base<Animal>{
 *    ...
 * };
 *
 * class Dog : public Animal {
 *    ...
 * };
 *
 * class Cat : public Animal {
 *    ...
 * };
 *
 * xstd::intrusive_ptr<Animal> vec[2];
 * vec[0] = new Dog();
 * vec[1] = new Cat();
 * \endcode
 *
 * The main drawback to this simple method is it requires
 * reference counting to be added to the base class. This may not
 * be desirable and therefore a second class, xstd::reference_count
 * can be used to only add reference counting to the derived classes.
 *
 * \code *
 * class Animal {
 *    ...
 * };
 *
 * class Dog : public reference_count<Animal> {
 *    ...
 * };
 * class Cat : public reference_count<Animal> {
 *    ...
 * };
 *
 * using reference_counted_base = reference_count<Animal>;
 *
 * xstd::intrusive_ptr<reference_counted_base> vec[2];
 * vec[0] = new Dog();
 * vec[1] = new Cat();
 * \endcode
 */
template<typename T>
struct intrusive_ptr {

	using element_type = typename std::remove_extent<T>::type;


	constexpr intrusive_ptr() noexcept : data_ptr_(nullptr){}

    intrusive_ptr( T* p, bool add_ref = true ): data_ptr_(p) {
        if( (data_ptr_ != nullptr) && add_ref ){
        	intrusive_ptr_add_ref(data_ptr_);
        }
    }

    template<typename U>
    intrusive_ptr(intrusive_ptr<U> const& other): data_ptr_(other.get()){
    	if(data_ptr_ != nullptr){
    		intrusive_ptr_add_ref(data_ptr_);
    	}
    }

    intrusive_ptr(intrusive_ptr const& rhs): data_ptr_( rhs.data_ptr_ ){
        if(data_ptr_ != nullptr) intrusive_ptr_add_ref( data_ptr_ );
    }

    ~intrusive_ptr(){
        if(data_ptr_ != nullptr){
        	intrusive_ptr_release(data_ptr_);
        }
    }

    template<class U>
    intrusive_ptr& operator=(intrusive_ptr<U> const& rhs){
        this_type(rhs).swap(*this);
        return *this;
    }

    intrusive_ptr(intrusive_ptr&& rhs) noexcept : data_ptr_(rhs.data_ptr_){
        rhs.data_ptr_ = nullptr;
    }

    intrusive_ptr& operator=(intrusive_ptr&& rhs) noexcept{
        this_type(static_cast<intrusive_ptr&&>(rhs)).swap(*this);
        return *this;
    }

    template<class U> friend class intrusive_ptr;
    template<class U>
    intrusive_ptr(intrusive_ptr<U>&& rhs) noexcept : data_ptr_(rhs.data_ptr_){
    	rhs.data_ptr_ = nullptr;
    }

    template<class U>
    intrusive_ptr& operator=(intrusive_ptr<U>&& rhs) noexcept{
        this_type(static_cast<intrusive_ptr<U>&&>(rhs)).swap(*this);
        return *this;
    }

    intrusive_ptr& operator=(intrusive_ptr const& rhs){
        this_type(rhs).swap(*this);
        return *this;
    }

    intrusive_ptr& operator=(T* rhs){
        this_type(rhs).swap(*this);
        return *this;
    }

    void reset(){
        this_type().swap(*this);
    }

    void reset(T* rhs ){
        this_type(rhs).swap(*this);
    }

    void reset(T* rhs, bool add_ref ){
        this_type(rhs,add_ref).swap(*this);
    }

    T* get() const noexcept{
        return data_ptr_;
    }

    T* detach() noexcept{
        T* ret = data_ptr_;
        data_ptr_ = nullptr;
        return ret;
    }

    T& operator*() const noexcept{
        assert( data_ptr_ != nullptr );
        return *data_ptr_;
    }

    T* operator->() const noexcept{
        assert( data_ptr_ != nullptr );
        return data_ptr_;
    }

    void swap(intrusive_ptr& rhs) noexcept{
        T* tmp = data_ptr_;
        data_ptr_ = rhs.data_ptr_;
        rhs.data_ptr_ = tmp;
    }

    /// Implicit conversion to bool
	operator bool() const{
		return (data_ptr_ != nullptr);
	}

	std::size_t use_count() const noexcept {
		return (data_ptr_) ? data_ptr_->use_count() : 0;
	}

private:
	using this_type = intrusive_ptr;

	T* data_ptr_;
};


template<class T, class U> inline bool operator==(intrusive_ptr<T> const& a, intrusive_ptr<U> const& b) noexcept{
    return a.get() == b.get();
}

template<class T, class U> inline bool operator!=(intrusive_ptr<T> const& a, intrusive_ptr<U> const& b) noexcept{
    return a.get() != b.get();
}

template<class T, class U> inline bool operator==(intrusive_ptr<T> const& a, U* b) noexcept{
    return a.get() == b;
}

template<class T, class U> inline bool operator!=(intrusive_ptr<T> const& a, U* b) noexcept{
    return a.get() != b;
}

template<class T, class U> inline bool operator==(T* a, intrusive_ptr<U> const& b) noexcept{
    return a == b.get();
}

template<class T, class U> inline bool operator!=(T* a, intrusive_ptr<U> const& b) noexcept{
    return a != b.get();
}

template<class T> inline bool operator==(intrusive_ptr<T> const& p, std::nullptr_t) noexcept{
    return p.get() == nullptr;
}

template<class T> inline bool operator==(std::nullptr_t, intrusive_ptr<T> const& p) noexcept{
    return p.get() == nullptr;
}

template<class T> inline bool operator!=(intrusive_ptr<T> const& p, std::nullptr_t) noexcept{
    return p.get() != nullptr;
}

template<class T> inline bool operator!=(std::nullptr_t, intrusive_ptr<T> const& p) noexcept{
    return p.get() != nullptr;
}

template<class T> inline bool operator<(intrusive_ptr<T> const& a, intrusive_ptr<T> const& b) noexcept{
    return std::less<T*>()(a.get(), b.get());
}

template<class T> void swap(intrusive_ptr<T>& lhs, intrusive_ptr<T>& rhs) noexcept{
    lhs.swap(rhs);
}

template<class T> T* get_pointer(intrusive_ptr<T> const& p) noexcept{
    return p.get();
}

template<class T, class U> intrusive_ptr<T> static_pointer_cast(intrusive_ptr<U> const& p){
    return static_cast<T*>(p.get());
}

template<class T, class U> intrusive_ptr<T> const_pointer_cast(intrusive_ptr<U> const& p){
    return const_cast<T*>(p.get());
}

template<class T, class U> intrusive_ptr<T> dynamic_pointer_cast(intrusive_ptr<U> const& p){
    return dynamic_cast<T*>(p.get());
}

template<class Y> std::ostream & operator<<(std::ostream & os, intrusive_ptr<Y> const& p){
    os << p.get();
    return os;
}

} /* namespace xstd */



namespace std{
template<typename T>
struct hash<::xstd::intrusive_ptr<T>>{
	using argument_type = ::xstd::intrusive_ptr<T>;
	using result_type   = std::size_t;
	using key_type      = typename argument_type::element_type*;

	result_type operator()(argument_type const& ptr) const noexcept{
		return std::hash<key_type>(ptr->get());
	}
};
}



#endif /* INTRUSIVE_PTR_HPP_ */
