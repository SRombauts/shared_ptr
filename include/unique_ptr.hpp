/**
 * @file  unique_ptr.hpp
 * @brief unique_ptr is a fake implementation to use in place of a C++11 std::unique_ptr when compiling on an older compiler.
 *
 * @see http://www.cplusplus.com/reference/memory/unique_ptr/
 *
 * Copyright (c) 2014 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#pragma once

#include <cstddef>      // NULL
#include <algorithm>    // std::swap

// can be replaced by other error mechanism
#include <cassert>
#define SHARED_ASSERT(x)    assert(x)


/**
 * @brief fake implementation to use in place of a C++11 std::move() when compiling on an older compiler.
 *
 * @see http://www.cplusplus.com/reference/utility/move/
*/
template <typename T>
inline T& move(T& v)
{
    return v;
}


/**
 * @brief minimal implementation of smart pointer, a subset of the C++11 std::unique_ptr or boost::unique_ptr.
 *
 * unique_ptr is a smart pointer retaining ownership of an object through a provided pointer,
 * and sharing this ownership with a reference counter.
 * It destroys the object when the last shared pointer pointing to it is destroyed or reset.
 */
template<class T>
class unique_ptr
{
public:
    /// The type of the managed object, aliased as member type
    typedef T element_type;

    /// @brief Default constructor
    unique_ptr(void) throw() : // never throws
        px(NULL)
    {
    }
    /// @brief Constructor with the provided pointer to manage
    explicit unique_ptr(T* p) // may throw std::bad_alloc
      //px(p), would be unsafe as acquire() may throw, which would call release() in destructor
    {
        acquire(p);   // may throw std::bad_alloc
    }
    /// @brief Copy constructor to convert from another pointer type
    template <class U>
    unique_ptr(unique_ptr<U>& ptr) throw() // never throws (see comment below)
      //px(ptr.px)
    {
        acquire(static_cast<typename unique_ptr<T>::element_type*>(ptr.px));   // will never throw std::bad_alloc
        ptr.px = NULL; // ownership transfered
    }
    /// @brief Copy constructor (used by the copy-and-swap idiom)
    unique_ptr(unique_ptr& ptr) throw() // never throws (see comment below)
      //px(ptr.px)
    {
        acquire(ptr.px);   // will never throw std::bad_alloc
        ptr.px = NULL; // ownership transfered
    }
    /// @brief Assignment operator using the copy-and-swap idiom (copy constructor and swap method)
    unique_ptr& operator=(unique_ptr ptr) throw() // never throws
    {
        swap(ptr);
        return *this;
    }
    /// @brief the destructor releases its ownership
    inline ~unique_ptr(void) throw() // never throws
    {
        release();
    }
    /// @brief this reset releases its ownership
    inline void reset(void) throw() // never throws
    {
        release();
    }
    /// @brief this reset release its ownership and re-acquire another one
    void reset(T* p) throw() // may throw std::bad_alloc
    {
        SHARED_ASSERT((NULL == p) || (px != p)); // auto-reset not allowed
        release();
        acquire(p); // may throw std::bad_alloc
    }

    /// @brief Swap method for the copy-and-swap idiom (copy constructor and swap method)
    void swap(unique_ptr& lhs) throw() // never throws
    {
        // Would be nice to enable use of ustl::swap by define
        std::swap(px, lhs.px);
    }

    // reference counter operations :
    inline operator bool() const throw() // never throws
    {
        return (NULL != px); // TODO nullptr
    }

    // underlying pointer operations :
    inline T& operator*()  const throw() // never throws
    {
        SHARED_ASSERT(NULL != px);
        return *px;
    }
    inline T* operator->() const throw() // never throws
    {
        SHARED_ASSERT(NULL != px);
        return px;
    }
    inline T* get(void)  const throw() // never throws
    {
        // no assert, can return NULL
        return px;
    }

private:
    /// @brief acquire/share the ownership of the px pointer, initializing the reference counter
    inline void acquire(T* p) // may throw std::bad_alloc
    {
        px = p; // here it is safe to acquire the ownership of the provided raw pointer, where exception cannot be thrown any more
    }

    /// @brief release the ownership of the px pointer, destroying the object when appropriate
    inline void release(void) throw() // never throws
    {
        delete px;
        px = NULL;
    }

private:
    // This allow pointer_cast functions to share the reference counter between different unique_ptr types
    template<class U>
    friend class unique_ptr;

private:
    T* px; //!< Native pointer
};


// comparaison operators
template<class T, class U> inline bool operator==(const unique_ptr<T>& l, const unique_ptr<U>& r) throw() // never throws
{
    return (l.get() == r.get());
}
template<class T, class U> inline bool operator!=(const unique_ptr<T>& l, const unique_ptr<U>& r) throw() // never throws
{
    return (l.get() != r.get());
}
template<class T, class U> inline bool operator<=(const unique_ptr<T>& l, const unique_ptr<U>& r) throw() // never throws
{
    return (l.get() <= r.get());
}
template<class T, class U> inline bool operator<(const unique_ptr<T>& l, const unique_ptr<U>& r) throw() // never throws
{
    return (l.get() < r.get());
}
template<class T, class U> inline bool operator>=(const unique_ptr<T>& l, const unique_ptr<U>& r) throw() // never throws
{
    return (l.get() >= r.get());
}
template<class T, class U> inline bool operator>(const unique_ptr<T>& l, const unique_ptr<U>& r) throw() // never throws
{
    return (l.get() > r.get());
}



// static cast of unique_ptr
template<class T, class U>
unique_ptr<T> static_pointer_cast(const unique_ptr<U>& ptr) // never throws
{
    return unique_ptr<T>(ptr, static_cast<typename unique_ptr<T>::element_type*>(ptr.get()));
}

// dynamic cast of unique_ptr
template<class T, class U>
unique_ptr<T> dynamic_pointer_cast(const unique_ptr<U>& ptr) // never throws
{
    T* p = dynamic_cast<typename unique_ptr<T>::element_type*>(ptr.get());
    if (NULL != p)
    {
        return unique_ptr<T>(ptr, p);
    }
    else
    {
        return unique_ptr<T>();
    }
}
