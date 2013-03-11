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


// TODO : find a short original namespace
namespace minimal
{

/**
 * @brief minimal implementation of smart pointer, a subset of the C++11 std::shared_ptr or boost::shared_ptr.
 *
 * TODO
 */
template<class T>
class shared_ptr
{
public:
    shared_ptr(void)    throw(); // nothrow
    shared_ptr(T*)      throw(); // nothrow
    ~shared_ptr(void)   throw(); // nothrow
    void reset(void)    throw(); // nothrow
private:
    T* ptr;
};


}  // namespace minimal
