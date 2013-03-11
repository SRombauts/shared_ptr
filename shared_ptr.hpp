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
 * TODO
 */
template<class T>
class shared_ptr
{
public:
    inline shared_ptr(void) throw() : // nothrow
        px(NULL),
        pn(NULL)
    {
    }
    inline explicit shared_ptr(T* p) : // throw std::bad_alloc
        px(p),
        pn(NULL)
    {
        acquire();
    }
    inline shared_ptr(const shared_ptr& ptr) : // throw std::bad_alloc
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
    inline void reset(T* p) throw() // nothrow
    {
        release();
        px = p;
        acquire();
    }

    inline shared_ptr& operator= (shared_ptr ptr) throw() // nothrow
    {
        swap(ptr);
        return *this;
    }

    inline operator bool() const throw() // nothrow
    {
        return (NULL != px);
    }

    inline T& operator*()  const throw() // nothrow
    {
        return *px;
    }
    inline T* operator->() const throw() // nothrow
    {
        return px;
    }

private:
    inline void acquire(void)
    {
        if (NULL != px)
        {
            if (NULL == pn)
            {
                try
                {
                    pn = new unsigned int(1);
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
    inline void swap(shared_ptr& lhs) throw() // nothrow
    {
        std::swap(px, lhs.px);
        std::swap(pn, lhs.pn);
    }
    inline void release(void) throw() // nothrow
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
    T*              px;
    unsigned int*   pn;
};
