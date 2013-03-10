shared_ptr
----------

shared_ptr is a minimal implementation of smart pointer, a subset of the C++11 std::shared_ptr or boost::shared_ptr.

### License

Copyright (c) 2013 Sébastien Rombauts (sebastien.rombauts@gmail.com)

Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
or copy at http://opensource.org/licenses/MIT)

### The goals of this minimal shared_ptr are:

- to replace the C++11 std::shared_ptr and boost::shared_ptr where they are not availlable
- to keep dependencies to a minimum (STL)
- to be portable
- to be light and fast
- to be monothreaded (not thread-safe)
- to use a permissive MIT license, similar to BSD or Boost, for proprietary/commercial usage

### Limitations
- not thread-safe
- not safe if the raw pointer stored is not of the same type as the template parameter

### Suported platforms:

Developements and tests are done under the following OSs :
- Debian 7 (testing)
- Ubuntu 12.10
- Windows XP/7/8
And following IDEs/Compilers
- GCC 4.7.x with a provided Makefile
- Eclipse CDT under Linux, using the provided Makefile
- Visual Studio Express 2008/2010/2012 for testing compatibility purpose

### Depandancies:

 - a STL implementation (even an old one, like those provided with VC6/eVC4 should work)

To use it in your project, you only need to include the shared_ptr.hpp source files
in your project code base.

## Getting started
### About std::shared_ptr:
- http://en.cppreference.com/w/cpp/memory/shared_ptr
- http://www.cplusplus.com/reference/memory/shared_ptr/

### About boost::shared_ptr:
- http://www.boost.org/libs/smart_ptr/shared_ptr.htm

### First sample demonstrates how to create a shared_ptr to a class Xxx:

The Xxx class:
```C++
class Xxx
{
public:
    Xxx(void);
    ...
    doSomething(size_t len);
    ...
};
```

shared_ptr usage:
```C++
void func(void)
{
    // Create an empty (ie. NULL) p1 shared_ptr
    shared_ptr<Xxx> xPtr;

    if (true)
    {
        // Create a new Xxx object, and give its ownership to the yPtr shared_ptr
        shared_ptr<Xxx> yPtr(new Xxx());
        
        // Access members functions/variables like with a raw pointer
        yPtr->doSomething(1024);
        
        // Share ownership by making a copy of the shared_ptr (the reference counter reachs 2)
        xPtr = yPtr;
        
    } // yPtr is destroyed, but yPtr retains the ownership of the object

    ...   
    
} // xPtr is destroyed, the reference counter drops to 0 thus the object is destroyed and the memory freed
```

