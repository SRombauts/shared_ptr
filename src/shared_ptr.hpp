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

// can be replaced by other error mecanism
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
    /// @brief Default constructor
    shared_ptr(void) throw() : // never throws
        px(NULL),
        pn(NULL)
    {
    }
    /// @brief Constructor with the provided pointer to manage
    explicit shared_ptr(T* p) : // can throw std::bad_alloc
        px(p),
        pn(NULL)
    {
        acquire();
    }
    /// @brief Copy constructor (used by the copy-and-swap idiom)
    shared_ptr(const shared_ptr& ptr) : // can throw std::bad_alloc
        px(ptr.px),
        pn(ptr.pn)
    {
        acquire();
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
    void reset(T* p) throw() // can throw std::bad_alloc
    {
        SHARED_ASSERT(p == 0 || p != px); // auto-reset not allowed
        release();
        px = p;
        pn = NULL;
        acquire();
    }

    /// @brief Swap method for the copy-and-swap idiom (copy constructor and swap method)
    void swap(shared_ptr& lhs) throw() // never throws
    {
        // TODO enable ustl::swap by define
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
        SHARED_ASSERT(px != 0);
        return *px;
    }
    inline T* operator->() const throw() // never throws
    {
        SHARED_ASSERT(px != 0);
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
    void acquire(void) // can throw std::bad_alloc
    {
        if (NULL != px)
        {
            if (NULL == pn)
            {
                try
                {
                    pn = new long(1); // can throw std::bad_alloc
                }
                catch (std::bad_alloc&)
                {
                    delete px;
                    px = NULL;
                    throw; // rethrow the std::bad_alloc
                }
            }
            else
            {
                ++(*pn);
            }
        }
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
                px = NULL;
                delete pn;
                pn = NULL;
            }
        }
    }

private:
    T*      px; //!< Native pointer
    long*   pn; //!< Reference counter
};

