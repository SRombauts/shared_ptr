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
    shared_ptr(void) throw() : // nothrow
        px(NULL),
        pn(NULL)
    {
    }
    explicit shared_ptr(T* p) : // throw std::bad_alloc
        px(p),
        pn(NULL)
    {
        acquire();
    }
    shared_ptr(const shared_ptr& ptr) : // throw std::bad_alloc
        px(ptr.px),
        pn(ptr.pn)
    {
        acquire();
    }
    inline ~shared_ptr(void) throw() // nothrow
    {
        release();
    }
    inline void reset(void) throw() // nothrow
    {
        release();
    }
    void reset(T* p) throw() // nothrow
    {
        release();
        px = p;
        acquire();
    }

    shared_ptr& operator=(shared_ptr ptr) throw() // nothrow
    {
        swap(ptr);
        return *this;
    }
    void swap(shared_ptr& lhs) throw() // nothrow
    {
        std::swap(px, lhs.px);
        std::swap(pn, lhs.pn);
    }

    inline operator bool() const throw() // nothrow
    {
        return unique();
    }
    inline bool unique(void)  const throw() // nothrow
    {
        return (NULL != pn);
    }
    long use_count(void)  const throw() // nothrow
    {
        long count = 0;
        if (NULL != pn)
        {
            count = *pn;
        }
        return count;
    }

    inline T& operator*()  const throw() // nothrow
    {
        return *px;
    }
    inline T* operator->() const throw() // nothrow
    {
        return px;
    }
    inline T* get(void)  const throw() // nothrow
    {
        return px;
    }

    inline bool operator== (const shared_ptr& ptr) const
    {
        return (px == p.px)
    }
    inline bool operator== (const T* p) const
    {
        return (px == p)
    }
    inline bool operator!= (const shared_ptr& ptr) const
    {
        return (px != p.px)
    }
    inline bool operator!= (const T* p) const
    {
        return (px != p)
    }
    inline bool operator<= (const shared_ptr& ptr) const
    {
        return (px <= p.px)
    }
    inline bool operator<= (const T* p) const
    {
        return (px <= p)
    }
    inline bool operator< (const shared_ptr& ptr) const
    {
        return (px < p.px)
    }
    inline bool operator< (const T* p) const
    {
        return (px < p)
    }
    inline bool operator>= (const shared_ptr& ptr) const
    {
        return (px >= p.px)
    }
    inline bool operator>= (const T* p) const
    {
        return (px >= p)
    }
    inline bool operator> (const shared_ptr& ptr) const
    {
        return (px > p.px)
    }
    inline bool operator> (const T* p) const
    {
        return (px > p)
    }

private:
    void acquire(void)
    {
        if (NULL != px)
        {
            if (NULL == pn)
            {
                try
                {
                    pn = new long(1);
                }
                catch (std::bad_alloc&)
                {
                    delete px;
                    throw;
                }
            }
            else
            {
                ++(*pn);
            }
        }
    }
    void release(void) throw() // nothrow
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
