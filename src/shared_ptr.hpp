/**
 * @file  shared_ptr.hpp
 * @brief shared_ptr is a minimal implementation of smart pointer, a subset of the C++11 std::shared_ptr or boost::shared_ptr.
 *
 * Copyright (c) 2013 Sebastien Rombauts (sebastien.rombauts@gmail.com)
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
 * @brief minimal implementation of smart pointer, a subset of the C++11 std::shared_ptr or boost::shared_ptr.
 *
 * shared_ptr is a smart pointer retaining ownership of an object through a provided pointer,
 * and sharing this ownership with a reference counter.
 * It destroys the object when the last shared pointer pointing to it is destroyed or reset.
 */
template<class T>
class shared_ptr
{
public:
   typedef T element_type;

   /// @brief Default constructor
    shared_ptr(void) throw() : // never throws
        px(NULL),
        pn(NULL)
    {
    }
    /// @brief Constructor with the provided pointer to manage
    explicit shared_ptr(T* p) : // may throw std::bad_alloc
      //px(p), would be unsafe as acquire() may throw, which would call release() in destructor
        pn(NULL)
    {
        acquire(p);   // may throw std::bad_alloc
    }
    /// @brief Constructor to share ownership. Warning : to be used for pointer_cast only ! (does not manage two separate <T> and <U> pointers)
    template <class U>
    shared_ptr(const shared_ptr<U>& ptr, T* p) :
     //px(p), would be unsafe as acquire() may throw, which would call release() in destructor
       pn(ptr.pn)
    {
       acquire(p);   // may throw std::bad_alloc
    }
    /// @brief Copy constructor to convert from another pointer type
    template <class U>
    shared_ptr(const shared_ptr<U>& ptr) throw() : // never throws (see comment below)
      //px(ptr.px),
        pn(ptr.pn)
    {
        SHARED_ASSERT((NULL == ptr.px) || (NULL != ptr.pn)); // must be cohérent : no allocation allowed in this path
        acquire(static_cast<typename shared_ptr<T>::element_type*>(ptr.px));   // will never throw std::bad_alloc
    }
    /// @brief Copy constructor (used by the copy-and-swap idiom)
    shared_ptr(const shared_ptr& ptr) throw() : // never throws (see comment below)
       //px(ptr.px),
        pn(ptr.pn)
    {
        SHARED_ASSERT((NULL == ptr.px) || (NULL != ptr.pn)); // must be cohérent : no allocation allowed in this path
        acquire(ptr.px);   // will never throw std::bad_alloc
    }
    /// @brief Assignment operator using the copy-and-swap idiom (copy constructor and swap method)
    shared_ptr& operator=(shared_ptr ptr) throw() // never throws
    {
        swap(ptr);
        return *this;
    }
    /// @brief the destructor releases its ownership
    inline ~shared_ptr(void) throw() // never throws
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
        px = NULL; // px = p would be unsafe as acquire() may throw, which would call release() in destructor
        pn = NULL;
        acquire(p); // may throw std::bad_alloc
    }

    /// @brief Swap method for the copy-and-swap idiom (copy constructor and swap method)
    void swap(shared_ptr& lhs) throw() // never throws
    {
        // Would be nice to enable use of ustl::swap by define
        std::swap(px, lhs.px);
        std::swap(pn, lhs.pn);
    }

    // reference counter operations :
    inline operator bool() const throw() // never throws
    {
        return (0 < use_count());
    }
    inline bool unique(void)  const throw() // never throws
    {
        return (1 == use_count());
    }
    long use_count(void)  const throw() // never throws
    {
        long count = 0;
        if (NULL != pn)
        {
            count = *pn;
        }
        return count;
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
        // no assert, car return NULL
        return px;
    }

    // comparaison operators
    inline bool operator== (const shared_ptr& ptr) const throw() // never throws
    {
        return (px == ptr.px);
    }
    inline bool operator== (const T* p) const throw() // never throws
    {
        return (px == p);
    }
    inline bool operator!= (const shared_ptr& ptr) const throw() // never throws
    {
        return (px != ptr.px);
    }
    inline bool operator!= (const T* p) const throw() // never throws
    {
        return (px != p);
    }
    inline bool operator<= (const shared_ptr& ptr) const throw() // never throws
    {
        return (px <= ptr.px);
    }
    inline bool operator<= (const T* p) const throw() // never throws
    {
        return (px <= p);
    }
    inline bool operator< (const shared_ptr& ptr) const throw() // never throws
    {
        return (px < ptr.px);
    }
    inline bool operator< (const T* p) const throw() // never throws
    {
        return (px < p);
    }
    inline bool operator>= (const shared_ptr& ptr) const throw() // never throws
    {
        return (px >= ptr.px);
    }
    inline bool operator>= (const T* p) const throw() // never throws
    {
        return (px >= p);
    }
    inline bool operator> (const shared_ptr& ptr) const throw() // never throws
    {
        return (px > ptr.px);
    }
    inline bool operator> (const T* p) const throw() // never throws
    {
        return (px > p);
    }

private:
    /// @brief acquire/share the ownership of the px pointer, initializing the reference counter
    void acquire(T* p) // may throw std::bad_alloc
    {
        if (NULL != p)
        {
            if (NULL == pn)
            {
                try
                {
                    pn = new long(1); // may throw std::bad_alloc
                }
                catch (std::bad_alloc&)
                {
                    delete p;
                    throw; // rethrow the std::bad_alloc
                }
            }
            else
            {
                ++(*pn);
            }
        }
        // here it is safe to acquire the ownership of the provided raw pointer, where exception cannot be thrown any more
        px = p;
    }

    /// @brief release the ownership of the px pointer, destroying the object when appropriate
    void release(void) throw() // never throws
    {
        if (NULL != pn)
        {
            --(*pn);
            if (0 == *pn)
            {
                delete px;
                delete pn;
            }
            px = NULL;
            pn = NULL;
        }
    }

private:
    // This allow pointer_cast functions to share the reference counter between different shared_ptr types
    template<class U>
    friend class shared_ptr;

private:
    T*      px; //!< Native pointer
    long*   pn; //!< Reference counter
};


// static cast of shared_ptr
template<class T, class U>
shared_ptr<T> static_pointer_cast(const shared_ptr<U>& ptr) // never throws
{
    return shared_ptr<T>(ptr, static_cast<typename shared_ptr<T>::element_type*>(ptr.get()));
}

// dynamic cast of shared_ptr
template<class T, class U>
shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& ptr) // never throws
{
    T* p = dynamic_cast<typename shared_ptr<T>::element_type*>(ptr.get());
    if (NULL != p)
    {
        return shared_ptr<T>(ptr, p);
    }
    else
    {
        return shared_ptr<T>();
    }
}
